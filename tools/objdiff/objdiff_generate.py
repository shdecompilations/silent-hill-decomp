import json
import logging
import re
from argparse import ArgumentParser
from dataclasses import dataclass, asdict
from pathlib import Path

import yaml
from elftools.elf.elffile import ELFFile


@dataclass
class UnitMetadata:
    progress_categories: list[str]
    source_path: str

@dataclass
class Unit:
    name: str
    base_path: str
    target_path: str
    metadata: UnitMetadata
    symbol_mappings: dict[str, str]

@dataclass
class ProgressCategory:
    id: str
    name: str

@dataclass
class Config:
    build_base: bool
    build_target: bool
    custom_make: str
    custom_args: list[str]
    units: list[Unit]
    progress_categories: list[ProgressCategory]


def _create_config():
    parser = ArgumentParser()
    parser.add_argument("config", type = Path)
    parser.add_argument("version", type = str)
    parser.add_argument("--ninja", action="store_true")
    parser.add_argument("--self-compare", action="store_true",
                        help="Use the matching build's objects as both target and base. Only code/data still "
                             "included from asm (marked with .NON_MATCHING symbols) is counted as unmatched.")
    parser.add_argument("--include-raw-splits", action="store_true",
                        help="Also create units for splat's raw data/rodata/bss splits, so data not yet "
                             "migrated to C is counted as unmatched.")
    parser.add_argument("--output", type = Path, help="Override the config's output path.")
    args = parser.parse_args()

    if not args.config.exists() or args.config.is_dir() or args.config.suffix != ".yaml":
        raise ValueError(f"The given path {args.objects} is not pointing towards a valid config.")

    global is_ninja
    is_ninja = args.ninja or False

    global game_version
    game_version = args.version

    global self_compare
    self_compare = args.self_compare

    global excluded_name
    excluded_name = RAW_SPLIT_EXCLUDED_NAME if args.include_raw_splits else EXCLUDED_NAME

    with open(args.config) as stream:
        try:
            config = yaml.safe_load(stream)
        except yaml.YAMLError as exc:
            raise exc

    if args.output:
        config["output"] = str(args.output)
    return config

def _read_symbols_from_elf(path: Path):
    addr_to_name = {}
    with open(path, 'rb') as f:
        elffile = ELFFile(f)

        for section in elffile.iter_sections():
            if not hasattr(section, 'iter_symbols'):
                continue
            for sym in section.iter_symbols():
                if sym['st_info']['type'] != 'STT_FUNC':
                    continue  # skip non-function symbols
                addr = sym['st_value']
                name = sym.name
                if addr != 0 and name:  # skip undefined/empty
                    addr_to_name[addr] = name
    return addr_to_name

def _normalize_suffixed_symbols(path: Path):
    """Scan elf for symbols with numeric suffixes, return a dictionary of non-suffix -> suffix pairs"""
    symbols = _read_symbols_from_elf(path)
    suffix_pattern = re.compile(r'\.(\d+)$')
    normalized = {}

    for name in symbols.values():
        match = suffix_pattern.search(name)
        if match:
            base_name = name[: match.start()]  # strip the .<digits> suffix
            normalized[base_name] = name

    return normalized

# Skip non-migrated units by suffix
EXCLUDED_NAME = re.compile(r"\.(data|rodata|sdata|bss)\.s\.o$|\.bin\.o$")
RAW_SPLIT_EXCLUDED_NAME = re.compile(r"\.bin\.o$")

RAW_SPLIT_LINE = re.compile(r"-\s*\[\s*(0x[0-9A-Fa-f]+)\s*,\s*(rodata|data|sdata|bss|sbss)\s*(?:,\s*([^\]\s]+)\s*)?\].*#.*garbage", re.IGNORECASE)

def _garbage_split_objects(version: str) -> set[str]:
    """Objects of raw splits commented as garbage in the splat yamls, which hold padding rather than data to migrate."""
    objects = set()
    for yaml_path in Path(f"configs/{version}").rglob("*.yaml"):
        text = yaml_path.read_text()
        asm_path = re.search(r"^\s*asm_path:\s*(\S+)", text, re.MULTILINE)
        if not asm_path:
            continue
        for match in RAW_SPLIT_LINE.finditer(text):
            offset, kind, name = match.groups()
            # Same object naming as tools/get_o_files.py
            stem = name if name else hex(int(offset, 16))[2:].upper()
            objects.add(f"build/{version}/{asm_path.group(1)}/data/{stem}.{kind}.s.o")
    return objects

def _collect_objects(path: Path, config) -> list[Path]:
    ignored = config["ignored_files"]
    garbage = _garbage_split_objects(game_version) if self_compare else set()
    return [
        path for path in path.rglob("*.o")
        if not excluded_name.search(path.name)
        # Raw splits sit in a data/ folder, e.g. bodyprog/data/libkmath/libkmath.rodata.s.o for bodyprog/libkmath
        and not any(file in path.as_posix() or file in path.as_posix().replace("/data/", "/") for file in ignored)
        and path.as_posix() not in garbage
    ]

def _self_compare_roots(version: str) -> list[Path]:
    # Matching build output: C objects under src/, splat asm objects (raw splits, hasm) under asm/<version>/.
    return [Path(f"build/{version}/src"), Path(f"build/{version}/asm/{version}")]

def _determine_categories(path: Path, config) -> tuple[UnitMetadata, str]:
    if self_compare:
        root = next(r for r in _self_compare_roots(game_version) if path.is_relative_to(r))
        modified_path = path.relative_to(root).as_posix()
        # Raw splits live in a data/ folder next to their overlay's asm, e.g. bodyprog/data/694.rodata.s.o
        modified_path = re.sub(r"/data/", "/", modified_path)
    else:
        modified_path = path.relative_to(config["expected_paths"][game_version]).as_posix()
    if game_version == "ALL":
        modified_path = re.sub(r"^(USA|EUR|JAP[0-2])/", "", str(modified_path))

    categories = []
    for category in config["categories"]:
        for prefix in category["paths"]:
            if str(modified_path).startswith(prefix):
                categories.append(category["id"])
    fixed_ext = re.sub( r'\.(s|c).o$', '', modified_path)
    
    if game_version != "ALL":
        return UnitMetadata(categories, f"src/{fixed_ext}.c"), fixed_ext, ""
    else:
        return UnitMetadata(categories, f"src/{fixed_ext}.c"), fixed_ext, re.search(r"/(USA|EUR|JAP[0-2])/", str(path))[0]

def main():
    logging.basicConfig(level = logging.INFO)
    config = _create_config()
    
    if self_compare:
        if game_version == "ALL":
            raise ValueError("--self-compare needs a single game version.")
        expected_objects = [obj for root in _self_compare_roots(game_version) for obj in _collect_objects(root, config)]
    else:
        expected_objects = _collect_objects(Path(config["expected_paths"][game_version]), config)

    ignore_base_paths = ["."]

    logging.info(f"Accounting for {len(expected_objects)} objects.")
    units = []
    build_method = str
    for file in expected_objects:
        processed_path = _determine_categories(file, config)
        input_path = Path(processed_path[1]).as_posix()
        if self_compare:
            # Absolute, since this config is written outside the project root.
            file = file.resolve()
            base_path = file.as_posix()
        elif game_version != "ALL":
            base_path = f"build/{game_version}/src/{input_path}.c.o"
        else:
            base_path = f"build{processed_path[2]}src/{input_path}.c.o"
            input_path = f"{processed_path[2][1:]}{input_path}"
            
        
        # Create mappings for clone/disambiguation symbol suffixes in base object
        # (disabled as objdiff report doesn't appear to support symbol mappings right now)
        #symbol_mappings = _normalize_suffixed_symbols(base_path) if Path(base_path).exists() else {}
        symbol_mappings = {}
        
        if is_ninja:
            unit = Unit(
                input_path,
                base_path,
                str(file.as_posix()),
                processed_path[0],
                symbol_mappings)
            build_method = "ninja"
        else:
            unit = Unit(
                input_path,
                base_path if Path(base_path).exists() else None,
                str(file),
                processed_path[0],
                symbol_mappings)
            build_method = "make"
        units.append(unit)
    
    categories = []
    for category in config["categories"]:
        categories.append(ProgressCategory(category["id"], category["name"]))
    
    with (Path(config["output"])).open("w") as json_file:
        json.dump(asdict(Config(not self_compare, False, build_method, ["build", f"GAME_VERSION={game_version}", "SKIP_ASM=1", "OBJDIFF_FIX=1"] if build_method != "ninja" and game_version != "ALL" else None, units, categories)), json_file, indent=2)

if __name__ == "__main__":
    main()