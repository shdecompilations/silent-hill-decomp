import argparse
import os
import re
import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import List

import splat.scripts.split as split
import splat.util.conf as split_info
from ninja import ninja_syntax

if sys.platform in ["linux", 'linux2']:
    GAMEFILE_EXTENSION = "."
elif sys.platform == "win32":
    GAMEFILE_EXTENSION = ""

# For multi-version support
@dataclass
class GAMEVERSIONSETUPINFO:
    GAME_NAME: str
    GAME_VERSION_DIR: str
    GAME_FILE_EXE: str
    GAME_FILE_SILENT: str
    GAME_FILE_HILL: str

@dataclass
class GAMEVERSIONINFO:
    GAME_IDX: int
    GAME_NAME_VERSION: str
    GAME_SETUP_INFO: GAMEVERSIONSETUPINFO


GAMEVERSIONS = (
    # Retail
    GAMEVERSIONINFO(0, "USA",
        GAMEVERSIONSETUPINFO("SLUS-00707", "USA", "SLUS_007.07", f"SILENT{GAMEFILE_EXTENSION}", f"HILL{GAMEFILE_EXTENSION}")
    ),
    GAMEVERSIONINFO(1, "EUR",
        GAMEVERSIONSETUPINFO("SLES-01514", "EUR", "SLES_015.14", f"SILENT{GAMEFILE_EXTENSION}", f"HILL{GAMEFILE_EXTENSION}")
    ),
    GAMEVERSIONINFO(2, "JAP0",
        GAMEVERSIONSETUPINFO("SLPM-86192", "JAP0", "SLPM_861.92", f"SILENT{GAMEFILE_EXTENSION}", f"HILL{GAMEFILE_EXTENSION}")
    ),
    GAMEVERSIONINFO(3, "JAP1",
        GAMEVERSIONSETUPINFO("SLPM-86498", "JAP1", "SLPM_861.92", f"SILENT{GAMEFILE_EXTENSION}", f"HILL{GAMEFILE_EXTENSION}")
    ),
    GAMEVERSIONINFO(4, "JAP2",
        GAMEVERSIONSETUPINFO("SLPM-87029", "JAP2", "SLPM_861.92", f"SILENT{GAMEFILE_EXTENSION}", f"HILL{GAMEFILE_EXTENSION}")
    )
)

@dataclass
class YAML_INFO:
    SPLIT_ENTRIES: List[str]
    SPLIT_BASENAME: str
    SPLIT_LINKER: str
    SPLIT_UNDEF_FUN: str
    SPLIT_UNDEF_SYM: str

YAML_EXECUTABLE = ["main.yaml"]
YAML_ENGINE = ["bodyprog.yaml"]

YAML_SCREENS = [
    "screens/b_konami.yaml",
    "screens/stream.yaml",
    "screens/options.yaml",
    "screens/stf_roll.yaml",
    "screens/saveload.yaml"
]
YAML_ITEMS = [
    "items/item_eng.yaml",
    "items/item_ger.yaml",
    "items/item_frn.yaml",
    "items/item_itl.yaml",
    "items/item_spn.yaml"
]
YAML_MAPS_0 = [
    "maps/map0_s00.yaml",
    "maps/map0_s01.yaml",
    "maps/map0_s02.yaml"
]
YAML_MAPS_1 = [
    "maps/map1_s00.yaml",
    "maps/map1_s01.yaml",
    "maps/map1_s02.yaml",
    "maps/map1_s03.yaml",
    "maps/map1_s04.yaml",
    "maps/map1_s05.yaml",
    "maps/map1_s06.yaml"
]
YAML_MAPS_2 = [
    "maps/map2_s00.yaml",
    "maps/map2_s01.yaml",
    "maps/map2_s02.yaml",
    "maps/map2_s03.yaml",
    "maps/map2_s04.yaml"
]
YAML_MAPS_3 = [
    "maps/map3_s00.yaml",
    "maps/map3_s01.yaml",
    "maps/map3_s02.yaml",
    "maps/map3_s03.yaml",
    "maps/map3_s04.yaml",
    "maps/map3_s05.yaml",
    "maps/map3_s06.yaml"
]
YAML_MAPS_4 = [
    "maps/map4_s00.yaml",
    "maps/map4_s01.yaml",
    "maps/map4_s02.yaml",
    "maps/map4_s03.yaml",
    "maps/map4_s04.yaml",
    "maps/map4_s05.yaml",
    "maps/map4_s06.yaml"
]
YAML_MAPS_5 = [
    "maps/map5_s00.yaml",
    "maps/map5_s01.yaml",
    "maps/map5_s02.yaml",
    "maps/map5_s03.yaml"
]
YAML_MAPS_6 = [
    "maps/map6_s00.yaml",
    "maps/map6_s01.yaml",
    "maps/map6_s02.yaml",
    "maps/map6_s03.yaml",
    "maps/map6_s04.yaml",
    "maps/map6_s05.yaml"
]
YAML_MAPS_7 = [
    "maps/map7_s00.yaml",
    "maps/map7_s01.yaml",
    "maps/map7_s02.yaml",
    "maps/map7_s03.yaml"
]


# Directories
ASSETS_DIR   = "assets"
ASM_DIR      = "asm"
LINKER_DIR   = "linkers"
EXPECTED_DIR = "expected"
CONFIG_DIR   = "configs"
TOOLS_DIR    = "tools"
ROM_DIR      = "rom"
BUILD_DIR    = "build"
PERMUTER_DIR = "permuter"

# Sub-directories
if sys.platform == "linux" or sys.platform == "linux2":
    IMAGE_DIR   = f"{ROM_DIR}/image"
    OBJDIFF_DIR = f"{TOOLS_DIR}/objdiff"
    PSXISO_DIR  = f"{TOOLS_DIR}/psxiso"
elif sys.platform == "win32":
    IMAGE_DIR   = f"{ROM_DIR}\\image"
    OBJDIFF_DIR = f"{TOOLS_DIR}\\objdiff"
    PSXISO_DIR  = f"{TOOLS_DIR}\\psxiso"

# Tooling Paths
if sys.platform == "win32":
    PYTHON = "python"
else:
    PYTHON = "python3"
MASPSX = f"{PYTHON} tools/maspsx/maspsx.py"
if sys.platform == "linux" or sys.platform == "linux2":
    CROSS             = "mips-linux-gnu"
    AS                = f"{CROSS}-as"
    LD                = f"{CROSS}-ld"
    OBJCOPY           = f"{CROSS}-objcopy"
    OBJDUMP           = f"{CROSS}-objdump"
    CPP               = f"{CROSS}-cpp"
    CC                = f"{TOOLS_DIR}/gcc-2.8.1-psx/cc1"
    CC272             = f"{TOOLS_DIR}/gcc-2.7.2-cdk/cc1"
    OBJDIFF           = f"{OBJDIFF_DIR}/objdiff"
    OBJDIFF_GENSCRIPT = f"{OBJDIFF_DIR}/objdiff_generate.py"
    POSTBUILD         = f"{PYTHON} {TOOLS_DIR}/postbuild.py"
    DUMPSXISO         = f"{PSXISO_DIR}/dumpsxiso"
    ICONV             = f"{PYTHON} {TOOLS_DIR}/iconv_sjis_wrapper.py"
elif sys.platform == "win32":
    CROSS             = f"{TOOLS_DIR}/win-build/binutils/mips-linux-gnu"
    AS                = f"{CROSS}-as.exe"
    LD                = f"{CROSS}-ld.exe"
    OBJCOPY           = f"{CROSS}-objcopy.exe"
    OBJDUMP           = f"{CROSS}-objdump.exe"
    CPP               = f"{TOOLS_DIR}/win-build/gcc-win/bin/gcc.exe"
    CC                = f"{TOOLS_DIR}/win-build/gcc-psx/cc1psx.exe"
    CC272             = f"{TOOLS_DIR}/win-build/gcc-2.7.2-win/cc1psx.exe"
    OBJDIFF           = f"{OBJDIFF_DIR}\\objdiff.exe"
    OBJDIFF_GENSCRIPT = f"{OBJDIFF_DIR}\\objdiff_generate.py"
    POSTBUILD         = f"{PYTHON} {TOOLS_DIR}\\postbuild.py"
    DUMPSXISO         = f"{PSXISO_DIR}\\dumpsxiso.exe"
    ICONV             = f"{TOOLS_DIR}\\win-build\\iconv\\iconv.bat"

# Compilation flags (General)
INCLUDE_FLAGS   = f"-Iinclude -I {BUILD_DIR} -Iinclude/psyq -Iinclude/decomp"
OPT_FLAGS       = "-O2"
ENDIAN          = "-EL"
DL_EXE_FLAGS    = "-G8"
DL_OVL_FLAGS    = "-G0"

# Compilation flags (Tool specific)
# Note - MASPSX: Some of the files from SD library modifies the parameters, see `ninja_setup_list_add_source`
CPP_FLAGS = f"{INCLUDE_FLAGS} -D_LANGUAGE_C -DUSE_INCLUDE_ASM -P -MMD -MP -undef -Wall -lang-c -nostdinc"
if sys.platform == "linux" or sys.platform == "linux2":
    ICONV_FLAGS = f"-f UTF-8 -t SHIFT-JIS $in -o $out"
elif sys.platform == "win32":
    CPP_FLAGS = f"{CPP_FLAGS} -E"
    ICONV_FLAGS = f"$in $out"
MASPSX_FLAGS  = f"--gnu-as-path {AS} --run-assembler"
CC_FLAGS      = f"{OPT_FLAGS} -mips1 -mcpu=3000 -w -funsigned-char -fpeephole -ffunction-cse -fpcc-struct-return -fcommon -fverbose-asm -msoft-float -mgas -fgnu-linker -quiet"
AS_FLAGS      = f"{ENDIAN} {INCLUDE_FLAGS} {OPT_FLAGS} -march=r3000 -mtune=r3000 -no-pad-sections"
OBJDUMP_FLAGS = f"--disassemble-all --reloc --disassemble-zeroes -Mreg-names=32"

TARGETS_POSTBUILD = ["1ST/BODYPROG.BIN", "1ST/B_KONAMI.BIN", "VIN/STREAM.BIN", "VIN/MAP3_S06.BIN", "VIN/MAP4_S05.BIN", "VIN/MAP5_S01.BIN"]

def ninja_setup_list_add_source(target_path: str, source_path: str, ninjaFile, objdiff_file, non_matching_enabled, game_version_idx: int):
    
    skipAsm = ""
    nonMatching = ""
    
    if objdiff_file != None:
        skipAsm = "-DSKIP_ASM"
        nonMatching = "-DNON_MATCHING"
        if sys.platform == "linux" or sys.platform == "linux2":
            source_target_path = re.sub(r"^src", fr"asm/{GAMEVERSIONS[game_version_idx].GAME_NAME_VERSION}", source_path)
        elif sys.platform == "win32":
            source_target_path = re.sub(r"^src", fr"asm\\{GAMEVERSIONS[game_version_idx].GAME_NAME_VERSION}", source_path)
        source_target_path = re.sub(r".c$", r".s", source_target_path)
        if sys.platform == "linux" or sys.platform == "linux2":
            expected_path = re.sub(fr"^build/{GAMEVERSIONS[game_version_idx].GAME_NAME_VERSION}/src", fr"expected/{GAMEVERSIONS[game_version_idx].GAME_NAME_VERSION}", target_path)
        elif sys.platform == "win32":
            expected_path = re.sub(fr"^build\\{GAMEVERSIONS[game_version_idx].GAME_NAME_VERSION}\\src", fr"expected\\{GAMEVERSIONS[game_version_idx].GAME_NAME_VERSION}", target_path)
        
        if os.path.exists(source_target_path):
            if re.search("^asm.(USA|EUR|JAP\d).main.*", source_path):
                objdiff_file.build(outputs=f"{expected_path}.o", rule="as", inputs=source_target_path,
                    variables={
                        "DLFLAG": DL_EXE_FLAGS
                    }
                )
            else:
                objdiff_file.build(outputs=f"{expected_path}.s.o", rule="as", inputs=source_target_path,
                    variables={
                        "DLFLAG": DL_OVL_FLAGS
                    }
                )
        else:
            return
    
    if non_matching_enabled:
        nonMatching = "-DNON_MATCHING"
    
    
    if sys.platform == "win32":
        source_path = re.sub(r"\\", r"/", source_path)
        target_path = re.sub(r"\\", r"/", target_path)
    
    # Checks if the path indicates that the source file comes from a map file
    # in order to asign map flag
    if re.search("^src.maps.*", source_path):
        mapId = (re.search("map\d_s\d\d", source_path)[0]).upper()
        ninjaFile.build(
            outputs=f"{target_path}.i", rule="cpp", inputs=source_path,
            variables={
                "VERSION": f"-DVER_{GAMEVERSIONS[game_version_idx].GAME_NAME_VERSION}",
                "MAPIDFLAG": f"-D{mapId}",
                "SKIPASMFLAG": skipAsm,
                "NONMATCHINGFLAG": nonMatching
            }
        )
    else:
        mapId = ""
        ninjaFile.build(
            outputs=f"{target_path}.i", rule="cpp", inputs=source_path,
            variables={
                "VERSION": f"-DVER_{GAMEVERSIONS[game_version_idx].GAME_NAME_VERSION}",
                "MAPIDFLAG": "",
                "SKIPASMFLAG": skipAsm,
                "NONMATCHINGFLAG": nonMatching
            }
        )
    
    # Checks:
    # Enables SHIFT-JIS conversion if the file is part from a memcard file (bodyprog)
    # Assign proper DL flag for the executable or the overlays
    # Assign proper compiler version for the `libkpad` compilation
    if re.search("^src.bodyprog.memcard*", source_path):
        ninjaFile.build(
            outputs=f"{target_path}.sjis.i", rule="iconv", inputs=f"{target_path}.i", implicit=f"{target_path}.i"
        )
        ninjaFile.build(
            outputs=f"{target_path}.c.s", rule="cc", inputs=f"{target_path}.sjis.i", implicit=f"{target_path}.sjis.i",
            variables={
                "DLFLAG": DL_OVL_FLAGS
            }
        )
    elif re.search("^src.main.*", source_path):
        ninjaFile.build(
            outputs=f"{target_path}.c.s", rule="cc", inputs=f"{target_path}.i",
            variables={
                "DLFLAG": DL_EXE_FLAGS
            }
        )
    elif re.search("^src.bodyprog.libkpad.*", source_path):
        ninjaFile.build(
            outputs=f"{target_path}.c.s", rule="cc272", inputs=f"{target_path}.i",
            variables={
                "DLFLAG": DL_EXE_FLAGS
            }
        )
    else:
        ninjaFile.build(
            outputs=f"{target_path}.c.s", rule="cc", inputs=f"{target_path}.i",
            variables={
                "DLFLAG": DL_OVL_FLAGS
            }
        )
    
    # Checks:
    # Enables `--expand-div` for `smf_io` or `smf_mid`
    # Assign proper DL flag for the executable or the overlays
    # Assign proper assembler version for the `libkpad` compilation
    maspxVersion = "2.77"
    if re.search("(smf_io|smf_mid)", source_path):
        ninjaFile.build(
            outputs=f"{target_path}.c.o", rule="maspsx", inputs=f"{target_path}.c.s",
            variables={
                "EXPANDIVFLAG": "--expand-div",
                "DLFLAG": DL_OVL_FLAGS,
                "MASPSXVER": maspxVersion
            }
        )
    elif re.search("^src.main.*", source_path):
        ninjaFile.build(
            outputs=f"{target_path}.c.o", rule="maspsx", inputs=f"{target_path}.c.s",
            variables={
                "EXPANDIVFLAG": "",
                "DLFLAG": DL_EXE_FLAGS,
                "MASPSXVER": maspxVersion
            }
        )
    elif re.search("^src.bodyprog.libkpad.*", source_path):
        maspxVersion = "2.67"
        ninjaFile.build(
            outputs=f"{target_path}.c.o", rule="maspsx", inputs=f"{target_path}.c.s",
            variables={
                "EXPANDIVFLAG": "",
                "DLFLAG": DL_OVL_FLAGS,
                "MASPSXVER": maspxVersion
            }
        )
    else:
        ninjaFile.build(
            outputs=f"{target_path}.c.o", rule="maspsx", inputs=f"{target_path}.c.s",
            variables={
                "EXPANDIVFLAG": "",
                "DLFLAG": DL_OVL_FLAGS,
                "MASPSXVER": maspxVersion
            }
        )
    
    
    if objdiff_file != None:
        return f"{expected_path}.s.o"

def ninja_write_rules(rulesNinjaName: str, game_version_idx: int):    
    
    ninjaRulesFile = ninja_syntax.Writer(open(rulesNinjaName, "w", encoding="utf-8"), width=9999)
    
    ninjaRulesFile.rule(
        "as", description="as $in",
        command=f"{AS} {AS_FLAGS} $DLFLAG -o $out $in"
    )

    ninjaRulesFile.rule(
        "cc", description="cc $in",
        command=f"{CC} {CC_FLAGS} $DLFLAG -o $out $in",
    )
    
    ninjaRulesFile.rule(
        "cc272", description="cc272 $in",
        command=f"{CC272} {CC_FLAGS} $DLFLAG -o $out $in",
    )
    
    ninjaRulesFile.rule(
        "cpp", description="cpp $in",
        command=f"{CPP} -P -MMD -MP -MT $out -MF $out.d {CPP_FLAGS} $VERSION $MAPIDFLAG $SKIPASMFLAG $NONMATCHINGFLAG -o $out $in",
        depfile="$out.d"
    )

    ninjaRulesFile.rule(
        "iconv", description="iconv $in",
        command=f"{ICONV} {ICONV_FLAGS}",
    )

    ninjaRulesFile.rule(
        "objdump", description="objdump $in",
        command=f"{OBJDUMP} {OBJDUMP_FLAGS} $in > $out",
    )

    ninjaRulesFile.rule(
        "maspsx", description="maspsx $in",
        command=f"{MASPSX} {MASPSX_FLAGS} --aspsx-version=$MASPSXVER $EXPANDIVFLAG {AS_FLAGS} $DLFLAG -o $out $in",
    )
    
    ninjaRulesFile.rule(
        "ld", description="link $out",
        command=f"{LD} {ENDIAN} {OPT_FLAGS} -nostdlib --no-check-sections -r -b binary -o $out $in",
    )
    
    ninjaRulesFile.rule(
        "elf", description="elf $out",
        command=f"{LD} {ENDIAN} {OPT_FLAGS} -nostdlib --no-check-sections -Map $out.map -T $in -T $undef_sym_path -T $undef_fun_path -T {CONFIG_DIR}/{GAMEVERSIONS[game_version_idx].GAME_SETUP_INFO.GAME_VERSION_DIR}/lib_externs.ld -o $out",
    )
    
    ninjaRulesFile.rule(
        "objcopy", description="objcopy $out",
        command=f"{OBJCOPY} -O binary $in $out",
    )
    
    if sys.platform == "linux" or sys.platform == "linux2":
        ninjaRulesFile.rule(
            "sha256sum", description="checksum",
            command=f"sha256sum --ignore-missing --check $in",
        )
    elif sys.platform == "win32":
        ninjaRulesFile.rule(
            "sha256sum", description="checksum",
            command=f"cmd.exe /c {TOOLS_DIR}\\sha256sum.bat $in",
        )
    
    ninjaRulesFile.rule(
        "postbuild", description="postbuild script $in",
        command=f"{POSTBUILD} $in",
    )
    
    ninjaRulesFile.rule(
        "objdiff-config", description="objdiff-config",
        command=f"{PYTHON} {OBJDIFF_GENSCRIPT} --ninja $in $version",
    )

def ninja_build(split_entries, game_version_idx: int, objdiff_mode: bool, skip_checksum: bool, non_matching: bool):
    
    elfBuildRequirements      = []
    entriesPaths              = []
    
    checksumBuildRequirements = []
    objdiffConfigRequirements = []
    
    rulesNinja = "rules.ninja"
    
    ninja_write_rules(rulesNinja, game_version_idx)
    
    if objdiff_mode:
        ninjaFile            = ninja_syntax.Writer(open("matching.ninja", "w", encoding="utf-8"), width=9999)
        
        ninjaDiffFile        = ninja_syntax.Writer(open("objdiff.ninja", "w", encoding="utf-8"), width=9999)
        ninjaDiffFile.include(rulesNinja)
        
        ninjaNonmatchingFile = ninja_syntax.Writer(open("build.ninja", "w", encoding="utf-8"), width=9999)
        ninjaNonmatchingFile.include(rulesNinja)
    else:
        ninjaFile = ninja_syntax.Writer(open("build.ninja", "w", encoding="utf-8"), width=9999)
    
    ninjaFile.include(rulesNinja)
    
    for split_config in split_entries:        
        for split_entry in split_config.SPLIT_ENTRIES:
            for entry in split_entry:
                seg = entry.segment
                
                if entry.object_path is None:
                    continue
                
                if seg.type[0] == ".":
                    continue
                
                source_path = str(entry.src_paths[0])
                target_path = str(entry.object_path)
                
                match seg.type:
                    case "asm" | "data" | "sdata" | "bss" | "sbss" | "rodata" | "header" | "hasm":
                        if re.search("^asm.(USA|EUR|JAP\d).main.*", source_path):
                            ninjaFile.build(outputs=target_path, rule="as", inputs=source_path, variables={ "DLFLAG": DL_EXE_FLAGS } )
                        else:
                            ninjaFile.build(outputs=target_path, rule="as", inputs=source_path, variables={ "DLFLAG": DL_OVL_FLAGS } )
                        
                    case "c":
                        target_path_no_ext = target_path.removesuffix(".c.o")
                        ninja_setup_list_add_source(target_path_no_ext, source_path, ninjaFile, None, non_matching, game_version_idx)
                        
                        if objdiff_mode:
                            temp = ninja_setup_list_add_source(target_path_no_ext, source_path, ninjaNonmatchingFile, ninjaDiffFile, True, game_version_idx)
                            if temp != None: objdiffConfigRequirements.append(temp)
                    case "bin":
                        ninjaFile.build(outputs=target_path, rule="ld", inputs=source_path)
                    case "lib" | "o":
                        """Did you know that Silent Hill: Homecoming was meant to
                        have a seccion where Alex would have to go through a forest
                        and there he would have founded a hunter that would have
                        tried to kill him, but due lack of direction the whole
                        section was scrapped and the hunter model was repurpose for
                        Travis Grady"""
                        continue
                    case _:
                        print(f"ERROR: Unsupported build segment type {seg.type}")
                        sys.exit(1)
                
                
                elfBuildRequirements.append(target_path)
        
        output = f"{BUILD_DIR}/{GAMEVERSIONS[game_version_idx].GAME_SETUP_INFO.GAME_VERSION_DIR}/out"
        match split_config.SPLIT_BASENAME:
            case "BODYPROG.BIN" | "B_KONAMI.BIN":
                split_config.SPLIT_BASENAME = f"1ST/{split_config.SPLIT_BASENAME}"
                output = f"{output}/{split_config.SPLIT_BASENAME}"
            case "main":
                output = f"{output}/SLUS_007.07"
            case _:
                split_config.SPLIT_BASENAME = f"VIN/{split_config.SPLIT_BASENAME}"
                output = f"{output}/{split_config.SPLIT_BASENAME}"
        
        ninjaFile.build(
            outputs=f"{output}.elf",
            rule="elf",
            inputs=split_config.SPLIT_LINKER,
            variables={
                "undef_sym_path": split_config.SPLIT_UNDEF_SYM,
                "undef_fun_path": split_config.SPLIT_UNDEF_FUN,
            },
            implicit=elfBuildRequirements
        )
        ninjaFile.build(
            outputs=output,
            rule="objcopy",
            inputs=f"{output}.elf",
            implicit=f"{output}.elf"
        )
        
        if split_config.SPLIT_BASENAME in TARGETS_POSTBUILD:
            ninjaFile.build(
                outputs=f"{output}.fix",
                rule="postbuild",
                inputs=output,
                implicit=output
            )
            checksumBuildRequirements.append(f"{output}.fix")
        else:
            checksumBuildRequirements.append(output)
    
    if objdiff_mode:
        ninjaDiffFile.build(outputs=f"{EXPECTED_DIR}/{GAMEVERSIONS[game_version_idx].GAME_NAME_VERSION}/objdiff.ok", rule="objdiff-config", inputs=f"{OBJDIFF_DIR}/config-retail.yaml",
        variables={ "version": GAMEVERSIONS[game_version_idx].GAME_SETUP_INFO.GAME_VERSION_DIR }, implicit=objdiffConfigRequirements)
    
    if skip_checksum != True:
        if sys.platform == "linux" or sys.platform == "linux2":
            checksumTarget = f"{CONFIG_DIR}/{GAMEVERSIONS[game_version_idx].GAME_SETUP_INFO.GAME_VERSION_DIR}/checksum.sha"
        elif sys.platform == "win32":
            checksumTarget = f"{CONFIG_DIR}/{GAMEVERSIONS[game_version_idx].GAME_SETUP_INFO.GAME_VERSION_DIR}"
        
        ninjaFile.build(
            outputs=f"{BUILD_DIR}/{GAMEVERSIONS[game_version_idx].GAME_SETUP_INFO.GAME_VERSION_DIR}/out/checksum.ok",
            rule="sha256sum",
            inputs=checksumTarget,
            implicit=checksumBuildRequirements
        )

def clean_working_files(clean_build_files: bool, clean_target_files: bool):
    shutil.rmtree(BUILD_DIR, ignore_errors=True)
    shutil.rmtree(PERMUTER_DIR, ignore_errors=True)
    if os.path.exists(".splache"):
        os.remove(".splache")

    if clean_build_files:
        if os.path.exists("build.ninja"):
            os.remove("build.ninja")
        if os.path.exists("matching.ninja"):
            os.remove("matching.ninja")
        if os.path.exists("objdiff.ninja"):
            os.remove("objdiff.ninja")
        if os.path.exists("rules.ninja"):
            os.remove("rules.ninja")
        if os.path.exists(".ninja_log"):
            os.remove(".ninja_log")
        shutil.rmtree(ASM_DIR, ignore_errors=True)
        shutil.rmtree(LINKER_DIR, ignore_errors=True)
        
    if clean_target_files:
        shutil.rmtree(EXPECTED_DIR, ignore_errors=True)

def extract_files(version: int):
    print(f"Extracting files for version {GAMEVERSIONS[version].GAME_NAME_VERSION}")
    
    targetAssets = f"{ASSETS_DIR}/{GAMEVERSIONS[version].GAME_SETUP_INFO.GAME_VERSION_DIR}"
    targetRom    = f"{ROM_DIR}/{GAMEVERSIONS[version].GAME_SETUP_INFO.GAME_VERSION_DIR}"
    
    shutil.rmtree(targetAssets, ignore_errors=True)
    shutil.rmtree(targetRom, ignore_errors=True)
    
    dumpsxisoFlags = f"-x {ROM_DIR}/{GAMEVERSIONS[version].GAME_SETUP_INFO.GAME_VERSION_DIR} -s {ROM_DIR}/{GAMEVERSIONS[version].GAME_SETUP_INFO.GAME_VERSION_DIR}/layout.xml {IMAGE_DIR}/{GAMEVERSIONS[version].GAME_SETUP_INFO.GAME_NAME}.bin"

    os.system(f"{DUMPSXISO} {dumpsxisoFlags}")
    subprocess.call([PYTHON, f"{TOOLS_DIR}/silentassets/extract.py",
    "-exe", f"{targetRom}/{GAMEVERSIONS[version].GAME_SETUP_INFO.GAME_FILE_EXE}",
    "-fs", f"{targetRom}/{GAMEVERSIONS[version].GAME_SETUP_INFO.GAME_FILE_SILENT}",
    "-fh", f"{targetRom}/{GAMEVERSIONS[version].GAME_SETUP_INFO.GAME_FILE_HILL}",
    targetAssets])

def splat_generate(yaml_path: str, generate_everything: bool):
    if generate_everything:
        subprocess.call(["splat", "split", "--disassemble-all", "--make-full-disasm-for-code", yaml_path])
    else:
        subprocess.call(["splat", "split", "--disassemble-all", yaml_path])

def main():
    parser = argparse.ArgumentParser(description="Configure the project")
    parser.add_argument(
        "-c", "--clean",
        help="Clean build and permuter files",
        action="store_true",
    )
    parser.add_argument(
        "-iso_e", "--iso_extract",
        help="Extract game files",
        action="store_true"
    )
    parser.add_argument(
        "-sc", "--skip_checksum",
        help="Skip checksum",
        action="store_true",
    )
    parser.add_argument(
        "-nm", "--non_matching",
        help="Compile functions labeled as Non Matching",
        action="store_true",
    )
    parser.add_argument(
        "-obj", "--objdiff_config",
        help="Builds setup for Objdiff",
        action="store_true",
    )
    parser.add_argument(
        "-ver", "--game_version",
        help="Extract and work under a specific version of the game (NOT IMPLEMENTED)",
        type=str,
    )
    parser.add_argument(
        "-set", "--setup",
        help="Setup individual overlays and/or the executable only",
        action="extend",
        nargs="+",
        type=str
    )
    args = parser.parse_args()

    cleanCompilationFiles = args.clean or False
    skipChecksumOption    = args.skip_checksum or False
    objdiffConfigOption   = args.objdiff_config or False
    nonMatchingOption     = args.non_matching or False
    gameVersionOption     = 0 # USA by default
    yamlsPaths            = []
    yamlMaps              = []
    splitsYamlInfo        = []
    
    if args.game_version != None:
        for gameVersionInfo in GAMEVERSIONS:
            if args.game_version.upper() == gameVersionInfo.GAME_NAME_VERSION:
                print(f"Version selected: {gameVersionInfo.GAME_NAME_VERSION}")
                gameVersionOption = gameVersionInfo.GAME_IDX
                break
        else:
            print("Version not supported.")
            sys.exit(1)
    
    if cleanCompilationFiles:
        print("Cleaning compilation files")
        clean_working_files(False, False)
        return
    
    if args.iso_extract:
        extract_files(gameVersionOption)
        return
    
    if args.setup == None:
        yamlsPaths.extend(YAML_EXECUTABLE)
        yamlsPaths.extend(YAML_ENGINE)
        yamlsPaths.extend(YAML_SCREENS)
        yamlMaps.extend(YAML_MAPS_0)
        yamlMaps.extend(YAML_MAPS_1)
        yamlMaps.extend(YAML_MAPS_2)
        yamlMaps.extend(YAML_MAPS_3)
        yamlMaps.extend(YAML_MAPS_4)
        yamlMaps.extend(YAML_MAPS_5)
        yamlMaps.extend(YAML_MAPS_6)
        yamlMaps.extend(YAML_MAPS_7)
        if gameVersionOption == 1:
            yamlsPaths.extend(YAML_ITEMS)
            for yamlMap in yamlMaps:
                for x in range(1,4):
                    yamlsPaths.extend([f"{yamlMap[:-7]}{x}{yamlMap[-6:]}"])
        else:
            yamlsPaths.extend(yamlMaps)
    else:
        for bins in args.setup:
            lowerBinName = bins.lower()
            match lowerBinName:
                case "exe":
                    yamlsPaths.extend(YAML_EXECUTABLE)
                case "eng":
                    yamlsPaths.extend(YAML_ENGINE)
                case "bko":
                    yamlsPaths.extend([YAML_SCREENS[0]])
                case "fmv" | "str":
                    yamlsPaths.extend([YAML_SCREENS[1]])
                case "opt":
                    yamlsPaths.extend([YAML_SCREENS[2]])
                case "cre":
                    yamlsPaths.extend([YAML_SCREENS[3]])
                case "sav":
                    yamlsPaths.extend([YAML_SCREENS[4]])
                case "scr":
                    yamlsPaths.extend(YAML_SCREENS)
                case "itm":
                    if gameVersionOption == 1:
                        yamlsPaths.extend(YAML_ITEMS)
                    else:
                        print("ITEM overlays are only available for the European release.")
                case "map":
                    yamlMaps.extend(YAML_MAPS_0)
                    yamlMaps.extend(YAML_MAPS_1)
                    yamlMaps.extend(YAML_MAPS_2)
                    yamlMaps.extend(YAML_MAPS_3)
                    yamlMaps.extend(YAML_MAPS_4)
                    yamlMaps.extend(YAML_MAPS_5)
                    yamlMaps.extend(YAML_MAPS_6)
                    yamlMaps.extend(YAML_MAPS_7)
                    if gameVersionOption == 1:
                        for yamlMap in yamlMaps:
                            for x in range(1,4):
                                yamlsPaths.extend([f"{yamlMap[:-7]}{x}{yamlMap[-6:]}"])
                    else:
                        yamlsPaths.extend(yamlMaps)
                case _:
                    try:
                        if re.match(r"m\d\d", lowerBinName):
                            yamlsPaths.extend([globals()[f"YAML_MAPS_{int(lowerBinName[1])}"][int(lowerBinName[2])]])
                        elif re.match(r"m\dx", lowerBinName):
                            yamlsPaths.extend(globals()[f"YAML_MAPS_{int(lowerBinName[1])}"])
                        else:
                            print("No recognizable overlay has been assigned.")
                            sys.exit(1)
                    except:
                        print("Overlay(s) not found.")
                        sys.exit(1)
    
    ##threads = []
    ##for i in range(len(yamlsPaths)):
    ##    path = f"{CONFIG_DIR}/{GAMEVERSIONS[gameVersionOption].GAME_SETUP_INFO.GAME_VERSION_DIR}/{yamlsPaths[i]}"
    ##    t = threading.Thread(target=splat_generate, args=(path, objdiffConfigOption,))
    ##    threads.append(t)
    ##    t.start()
    ##
    ##for t in threads:
    ##    t.join()
    
    clean_working_files(True, objdiffConfigOption)
    
    for yamlInfo in yamlsPaths:
        path = f"{CONFIG_DIR}/{GAMEVERSIONS[gameVersionOption].GAME_SETUP_INFO.GAME_VERSION_DIR}/{yamlInfo}"
        splitInfo = split_info.load([Path(path)])
        splat_generate(path, objdiffConfigOption)
        
        splitsEntries = []
        for segments in split.initialize_segments(splitInfo["segments"]):
            splitsEntries.extend(segments.get_linker_entries())
        
        splitsYamlInfo.append(YAML_INFO([splitsEntries],
        splitInfo["options"]["basename"],
        splitInfo["options"]["ld_script_path"],
        splitInfo["options"]["undefined_funcs_auto_path"],
        splitInfo["options"]["undefined_syms_auto_path"]))
    
    ninja_build(splitsYamlInfo, gameVersionOption, objdiffConfigOption, skipChecksumOption, nonMatchingOption)
    
    if objdiffConfigOption:
        subprocess.call([PYTHON, "-m", "ninja", "-f", "objdiff.ninja"])
        

if __name__ == "__main__":
    main()
