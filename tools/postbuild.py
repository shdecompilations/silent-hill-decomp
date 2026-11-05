import sys
import os

# Patch garbage bytes back into built files

USA_STREAM = [
    [0x12ADC, b'\xA0\x05\x60\xAC'],
    [0x12AE4, b'\xE4\x73\x40\xAC']
]

USA_BODYPROG = [
    [0x13A, b'\x08\x8E'],
    [0x9BF, b'\x4c\x00\x95\xab\x90'],
    [0x39E8, b'\x00\x61\x69\x6E'],
    [0x3FDA, b'\x05\x80'],
    [0x8DAB7, b'\x18'],
    [0xA23DC, b'\x0A\x09\x2E\x62'],
    [0xA24CC, b'\x38\x0D\x0A\x09'],
    [0xA2B8C, b'\x0D\x0A\x09\x2E'],
    [0xA3F9C, b'\x09\x30\x78\x61'],
    [0x9827C, b'\x22\x00\xA2\x97'],
    [0x98284, b'\x21\x10\x46'],
    [0x9B08C, b'\x02\x00\x01'],
    [0x9CA4C, b'\x04\xCF\x41\x76'],
    [0x9CA55, b'\x18\xE3\xB8'],
    [0x9F75C, b'\x10\x00\xB0\x8F'],
    [0x9FD8A, b'\x42\x30'],
    [0x9FD8C, b'\x07\x00\x40\x14'],
    [0x9FD94, b'\x0F\x80\x02\x3C'],
    [0x9F774, b'\x21\x80\x80'],
    [0x9F8F6, b'\x02'],
    [0x9F96C, b'\x10\x00\xB0\x8F'],
    [0x9FABC, b'\x30\x00\xA4\x27'],
    [0x9FACC, b'\x61\x26\x01\x0C'],
    [0x910CC, b'\x3C\x38\x92\xD8'],
    [0x981B4, b'\x80\x18\x13'],
    [0x981E1, b'\x70\xE5\x8D'],
    [0x981E4, b'\x05\x00\x02\x24'],
    [0x981EC, b'\x28\x00\x02\x24'],
    [0x9821C, b'\x21\x10\x49'],
]

EUR_BODYPROG = [
    [0x1, b'\x8E\x00'],
    [0xAD, b'\x45'],
    [0x136, b'\x2C'],
    [0x139, b'\xAF\x28'],
    [0x9BF, b'\xB2\x00\x08\xDD\x86'],
    [0x2B82, b'\x05\x80'],
    [0x533A, b'\x08\x80'],
    [0x83F2A, b'\x40\x08'],
    [0x87A0A, b'\x01\x02'], # Padding after `D_800AD098`.
    [0x8850E, b'\x0A\x80'],
    [0x886A6, b'\x0A'],
    [0x8C577, b'\x18'],
    [0x8FB8C, b'\x74\x7A\x74\x20'],
    [0x96CA1, b'\x01\x2C'],
    [0x96CA4, b'\x48\x02'],
    [0x96CAC, b'\x4A\xA4\x01\x02'],
    [0x96CDC, b'\x01\x02\x84'],
    [0x96D3D, b'\x08\x3E\x1D'],
    [0x96D45, b'\x12\x08'],
    [0x99B4F, b'\x50'],
    [0x9B50C, b'\x80'],
    [0x9B50E, b'\xFC'],
    [0x9B516, b'\xF8'],
    [0x9D73E, b'\x5C'],
    [0x9D7C4, b'\x2C\x04\x04'],
    [0x9D884, b'\x04\x2C\x04\x03'],
    [0x9D8C5, b'\x0A\x52\xCC'],
    [0x9D902, b'\x04\xFC'],
    [0x9D906, b'\x0A\x4A'],
    [0x9E21E, b'\x43\x8C'],
    [0x9E235, b'\x25\x09'],
    [0x9E3B6, b'\x02'],
    [0x9E42C, b'\x02\x13\x08'],
    [0x9E57E, b'\x02\x3C'],
    [0x9E58C, b'\x86\xF7\x42\x26'],
    [0x9E84A, b'\x62\x14\x47'],
    [0x9E84E, b'\x02\x24'],
    [0x9E854, b'\x13'],
    [0x9E856, b'\x02\x24'],
    [0xA0E9E, b'\x0C\x0C'],
    [0xA0F8C, b'\x24\x0A'],
    [0xA0F8F, b'\x05'],
    [0xA164E, b'\x02\x0F'],
    [0xA2A5C, b'\x24\x10\x50'],
]

EUR_STREAM = [
    [0x12ADC, b'\x06\x00\xF4\x03'],
    [0x12AE4, b'\x06\x00\xF4\x03']
]

JAP0_STREAM = [
    [0x12B2C, b'\x56\x49\x45\x57']
]

JAP0_BODYPROG = [
    [0x12E, b'\xE8\xFF'],
    [0x9AB, b'\x2C'],
    [0x3709, b'\x2C\x28\x20'],
    [0x3CFA, b'\x0D\x0A'],
    [0x871EE, b'\x24\x33'],
    [0x8FFE7, b'\x32'],
    [0x935FC, b'\xAF\x28\xFF\xAD'],
    [0x9A6E6, b'\x60'],
    [0x9A712, b'\x08\x00\x09\x00\x08\x00'],
    [0x9A71E, b'\x58'],
    [0x9A74C, b'\x04\x04\x04\x04'],
    [0x9D5BD, b'\xFE'],
    [0x9EF7C, b'\x10\x00\xB0\xAF'],
    [0x9EF87, b'\x16'],
    [0xA22B9, b'\x4E\x0A\x09\x09\x09\x82\xB7'],
    [0xA22C4, b'\x8C\x8C\x82\xC6'],
    [0xA490C, b'\x3A\x20\x25\x64'],
    [0xA49FC, b'\x54\x81\x09\x80'],
    [0xA50BC, b'\x73\x5F\x6F\x72'],
    [0xA64CC, b'\x6F\x72\x67\x65']
]

JAP1_STREAM = [
    [0x12B2C, b'\x56\x49\x45\x57']
]

JAP1_BODYPROG = [
    [0x12F, b'\xFE'],
    [0x9B7, b'\x20'],
    [0x3715, b'\xBA\x82\xC9'],
    [0x3D06, b'\x05\x80'],
    [0x9010A, b'\x00\x17\xA6\x17\xA9\x17'],
    [0x93724, b'\xFF\x0F\xFF\x03'],
    [0x9A80C, b'\x1D\x00\x04\xA2'],
    [0x9A838, b'\x00\x11\x03\x00\x08\x41\x03\x08'],
    [0x9A844, b'\x68\x00\x42\x24'],
    [0x9A874, b'\x1A\x00\x03\xA6'],
    [0x9A8D4, b'\x00\x00\x03\xAE'],
    [0x9A8DC, b'\x0D\x80\x04\x3C'],
    [0x9D6E4, b'\xB0\xB1\xB8\x3A'],
    [0x9F0A4, b'\x28\x00\x00\xAE'],
    [0x9F0AD, b'\x00\x00\xAE'],
    [0xA145C, b'\x08\x00\x83\xAC'],
    [0xA1498, b'\x00\x00\x02\x24'], # TODO: Check if first 2 00 bytes are used.
    [0xA149C, b'\xE8\xFF\xBD\x27'],
    [0xA1DB4, b'\x65\x7E\x03\x0C'],
    [0xA1DCC, b'\x6C\x00\x42\x8C'],
    [0xA1F4C, b'\x00\x00\x02\x3C'], # TODO: Check if first 2 00 bytes are used.
    [0xA1FC4, b'\x02\x00\x42\xA2'],
    [0xA2114, b'\x2C\x00\x40\xAC'],
    [0xA2124, b'\x0B\x00\x03\x3C'],
    [0xA23E3, b'\xAE'],
    [0xA23E4, b'\x01\x00\x63\x24'],
    [0xA23EC, b'\x0C\x00\x03\xAE'],
    [0xA4A34, b'\x0A\x09\x2E\x62'],
    [0xA4B24, b'\x09\x2E\x62\x79'],
    [0xA51E4, b'\x2E\x62\x79\x74'],
    [0xA65F4, b'\x0A\x09\x2E\x62']
]

JAP2_STREAM = [
    [0x12B2C, b'\x56\x49\x45\x57']
]

JAP2_BODYPROG = [
    [0x12F, b'\x0A'],
    [0x3715, b'\x20\x80\x02'],
    [0x3D06, b'\x03\x08'],
    [0x87312, b'\x02'],
    [0x93724, b'\xE4\xD3\x00\x0C'],
    [0x9A80C, b'\x01\x28'],
    [0x9A83A, b'\x01'],
    [0x9A83C, b'\x03'],
    [0x9A844, b'\x03'],
    [0x9A874, b'\x03'],
    [0x9A877, b'\xFE'],
    [0x9A8D4, b'\xFD\x03'],
    [0x9A8DC, b'\xFD\x03\x01\x04'],
    [0x9D6E4, b'\x25\x18\x62'],
    [0x9F0A4, b'\x02\x00\x62\x28'],
    [0xA12D6, b'\x0D\x0D'],
    [0xA135C, b'\x0D\x0D\x0D\x0D'],
    [0xA145C, b'\x03\x05\x1E\x3A'],
    [0xA1499, b'\x07\x29\x3A\x04\x07\x2A\x3A'],
    [0xA1DB5, b'\x08\x01'],
    [0xA1DCC, b'\xA0\x0A\x99\x11'],
    [0xA1F4E, b'\x01'],
    [0xA1FC4, b'\x34\xF3\xFF\xFF'],
    [0xA2114, b'\xBD\x82\xDC\x82'],
    [0xA2124, b'\x09\x8D\xB0\x82'],
    [0xA23E1, b'\x0A\x09\x09\x09\x82\xE0\x82'],
    [0xA23EC, b'\xA0\x82\xCC\x95'],
    [0xA51E4, b'\x30\x0D\x0A\x09'],
    [0xA65F4, b'\x09\x30\x78\x33'],
    [0xA65F4, b'\x09\x30\x78\x33']
]

# Garbage bytes to patch into map overlays, per version: { version: { map: [[offset, bytes], ...] } }
MAP_PATCHES = {
    "JAP2": {
        "map6_s05": [
            [0x1339, b'\x20']
        ],
        "map6_s01": [
            [0x245E, b'\x00\x00']
        ],
        "map5_s01": [
            [0x34D5, b'\xA6\x03\x08']
        ],
        "map4_s06": [
            [0x1325, b'\x10\xC3']
        ],
        "map4_s05": [
            [0x16F9, b'\x20\x89\xBA']
        ],
        "map4_s00": [
            [0x1325, b'\x90\x01']
        ],
        "map1_s04": [
            [0x13AD, b'\x10\x43']
        ],
        "map3_s06": [
            [0x1CA5, b'\x4A\x30\x28']
        ],
        "map2_s03": [
            [0x1356, b'\xB0\xAF']
        ]
    },
    "JAP1": {
        "map6_s05": [
            [0x1339, b'\x20']
        ],
        "map6_s01": [
            [0x245E, b'\x00\x00']
        ],
        "map5_s01": [
            [0x34D5, b'\xA8\x03\x08']
        ],
        "map4_s06": [
            [0x1325, b'\x28\x20\x20']
        ],
        "map4_s05": [
            [0x16F9, b'\x20\x89\xBA']
        ],
        "map4_s00": [
            [0x1325, b'\x90\x01']
        ],
        "map3_s06": [
            [0x1CA5, b'\x4A\x30\x28']
        ],
        "map2_s03": [
            [0x1356, b'\xB0\xAF']
        ],
        "map1_s04": [
            [0x13AD, b'\x10\x43']
        ]
    },
    "JAP0": {
        "map1_s04": [
            [0x13AD, b'\x10\x43\x00']
        ],
        "map2_s03": [
            [0x1356, b'\xB0\xAF']
        ],
        "map3_s06": [
            [0x1CA5, b'\x4A\x30\x28']
        ],
        "map4_s00": [
            [0x1325, b'\x90\x01\x00']
        ],
        "map4_s05": [
            [0x16F9, b'\x20\x89\xBA']
        ],
        "map4_s06": [
            [0x1325, b'\x61\x6D\x65']
        ],
        "map5_s01": [
            [0x34D5, b'\x0F\x63\x24']
        ],
        "map6_s01": [
            [0x245E, b'\x00\x00']
        ],
        "map6_s05": [
            [0x1339, b'\x20']
        ]
    }
}

# Garbage bytes to patch into screen overlays, per version: { version: { screen: [[offset, bytes], ...] } }
SCREEN_PATCHES = {
    "EUR": {
        "b_konami": [
            [0xE55, b'\x5C\x63\x61']
        ],
        "option": [
            [0x81A, b'\x0D\x0A']
        ],
        "stf_roll": [
            [0x822, b'\x31\x37\x00\x30\x28\x24\x00\x29\x0D\x0A'],
            [0x387B, b'\x0A']
        ]
    },
    "JAP0": {
        "option": [
            [0xB62, b'\x68\x98']
        ]
    },
    "JAP1": {
        "option": [
            [0xB62, b'\x68\x98']
        ]
    },
    "JAP2": {
        "option": [
            [0xB62, b'\x68\x98']
        ]
    }
}

SCREEN_BIN_NAMES = {
    "option": "OPTION",
    "stf_roll": "STF_ROLL",
    "b_konami": "B_KONAMI"
}

def apply_screen_patches(target):
    name = target.split("/")[-1]
    for version, screens in SCREEN_PATCHES.items():
        if name not in screens:
            continue
        folder = "1ST" if name == "b_konami" else "VIN"
        path = f"build/{version}/out/{folder}/{SCREEN_BIN_NAMES[name]}.BIN"
        if not os.path.isfile(path):
            continue
        with open(path, "r+b") as file:
            for offset, data in screens[name]:
                file.seek(offset)
                file.write(data)

def apply_map_patches(target):
    name = target.split("/")[-1]
    for version, maps in MAP_PATCHES.items():
        if name not in maps:
            continue
        path = f"build/{version}/out/VIN/{name.upper()}.BIN"
        if not os.path.isfile(path):
            continue
        with open(path, "r+b") as file:
            for offset, data in maps[name]:
                file.seek(offset)
                file.write(data)

# TS seems to have trimmed the main EXE down by truncating some bytes from .bss.
# Since those are zero-filled it wouldn't cause an issue for the game, but means we have to trim it too.
# We can use the size from original EXE header (+ 0x800 header size) to tell us what the expected size should be.
# TODO: Our `libgpu/sys.o` lib object was edited to trim the .bss for USA, that can be undone to closer match original lib.
MAIN_EXE_PATHS = [
    "build/USA/out/SLUS_007.07",
    "build/EUR/out/SLES_015.14",
    "build/JAP0/out/SLPM_861.92",
    "build/JAP1/out/SLPM_861.92",
    "build/JAP2/out/SLPM_861.92",
]

def trim_main_exes():
    for path in MAIN_EXE_PATHS:
        if not os.path.isfile(path):
            continue
        with open(path, "r+b") as file:
            file.seek(0x1C)
            size = 0x800 + int.from_bytes(file.read(4), "little")
            file.seek(0, os.SEEK_END)
            if file.tell() > size:
                file.truncate(size)

if __name__ == "__main__":
    if sys.argv[1] == "main":
        trim_main_exes()
    if sys.argv[1].startswith("maps/"):
        apply_map_patches(sys.argv[1])
    if sys.argv[1].startswith("screens/"):
        apply_screen_patches(sys.argv[1])
    match sys.argv[1]:
        case "screens/stream" | "build/USA/out/VIN/STREAM.BIN" | "build/EUR/out/VIN/STREAM.BIN" | "build/JAP0/out/VIN/STREAM.BIN" | "build/JAP1/out/VIN/STREAM.BIN" | "build/JAP2/out/VIN/STREAM.BIN":
            usaPath = "build/USA/out/VIN/STREAM.BIN"
            eurPath = "build/EUR/out/VIN/STREAM.BIN"
            jap0Path = "build/JAP0/out/VIN/STREAM.BIN"
            jap1Path = "build/JAP1/out/VIN/STREAM.BIN"
            jap2Path = "build/JAP2/out/VIN/STREAM.BIN"
            if os.path.isfile(usaPath):
                file = open(usaPath, "r+b")
                for data in USA_STREAM:
                    file.seek(data[0])
                    file.write(data[1])
                file.close()
            if os.path.isfile(eurPath):
                file = open(eurPath, "r+b")
                for data in EUR_STREAM:
                    file.seek(data[0])
                    file.write(data[1])
                file.close()
            if os.path.isfile(jap0Path):
                file = open(jap0Path, "r+b")
                for data in JAP0_STREAM:
                    file.seek(data[0])
                    file.write(data[1])
                file.close()
            if os.path.isfile(jap1Path):
                file = open(jap1Path, "r+b")
                for data in JAP1_STREAM:
                    file.seek(data[0])
                    file.write(data[1])
                file.close()
            if os.path.isfile(jap2Path):
                file = open(jap2Path, "r+b")
                for data in JAP2_STREAM:
                    file.seek(data[0])
                    file.write(data[1])
                file.close()

        case "bodyprog" | "build/USA/out/1ST/BODYPROG.BIN" | "build/EUR/out/1ST/BODYPROG.BIN" | "build/JAP0/out/1ST/BODYPROG.BIN" | "build/JAP1/out/1ST/BODYPROG.BIN" | "build/JAP2/out/1ST/BODYPROG.BIN":
            usaPath = "build/USA/out/1ST/BODYPROG.BIN"
            eurPath = "build/EUR/out/1ST/BODYPROG.BIN"
            jap0Path = "build/JAP0/out/1ST/BODYPROG.BIN"
            jap1Path = "build/JAP1/out/1ST/BODYPROG.BIN"
            jap2Path = "build/JAP2/out/1ST/BODYPROG.BIN"
            if os.path.isfile(usaPath):
                file = open(usaPath, "r+b")
                for data in USA_BODYPROG:
                    file.seek(data[0])
                    file.write(data[1])
                file.close()
            if os.path.isfile(eurPath):
                file = open(eurPath, "r+b")
                for data in EUR_BODYPROG:
                    file.seek(data[0])
                    file.write(data[1])
                file.close()
            if os.path.isfile(jap0Path):
                file = open(jap0Path, "r+b")
                for data in JAP0_BODYPROG:
                    file.seek(data[0])
                    file.write(data[1])
                file.close()
            if os.path.isfile(jap1Path):
                file = open(jap1Path, "r+b")
                for data in JAP1_BODYPROG:
                    file.seek(data[0])
                    file.write(data[1])
                file.close()
            if os.path.isfile(jap2Path):
                file = open(jap2Path, "r+b")
                for data in JAP2_BODYPROG:
                    file.seek(data[0])
                    file.write(data[1])
                file.close()
        case "screens/b_konami" | "build/USA/out/1ST/B_KONAMI.BIN" | "build/JAP0/out/1ST/B_KONAMI.BIN" | "build/JAP1/out/1ST/B_KONAMI.BIN" | "build/JAP2/out/1ST/B_KONAMI.BIN":
            # Random pad bytes between b_konami.c .data and lzss.c .data.
            usaPath = "build/USA/out/1ST/B_KONAMI.BIN"
            jap0Path = "build/JAP0/out/1ST/B_KONAMI.BIN"
            jap1Path = "build/JAP1/out/1ST/B_KONAMI.BIN"
            jap2Path = "build/JAP2/out/1ST/B_KONAMI.BIN"
            if os.path.isfile(usaPath):
                file = open(usaPath, "r+b")
                file.seek(0xF79)
                file.write(b'\x1F\xCE\xF3')
            if os.path.isfile(jap0Path):
                file = open(jap0Path, "r+b")
                file.seek(0x10A5)
                file.write(b'\x20\x20\x39')
            if os.path.isfile(jap1Path):
                file = open(jap1Path, "r+b")
                file.seek(0x10A5)
                file.write(b'\x0D\x0A\x09')
            if os.path.isfile(jap2Path):
                file = open(jap2Path, "r+b")
                file.seek(0x10A5)
                file.write(b'\x00\xAC\x8F')
        case "maps/map3_s06" | "build/USA/out/VIN/MAP3_S06.BIN":
            # Random bytes at end of rodata section, linker artifact?
            file = open("build/USA/out/VIN/MAP3_S06.BIN", "r+b")
            file.seek(0x1D65)
            file.write(b'\x65\x74\x5F')
            file.close()
        case "maps/map4_s05" | "build/USA/out/VIN/MAP4_S05.BIN":
            # Random bytes at end of rodata section, linker artifact?
            file = open("build/USA/out/VIN/MAP4_S05.BIN", "r+b")
            file.seek(0x1729)
            file.write(b'\x65\x5F\x77')
            file.close()
        case "maps/map5_s01" | "build/USA/out/VIN/MAP5_S01.BIN":
            # Random bytes at end of rodata section, linker artifact?
            file = open("build/USA/out/VIN/MAP5_S01.BIN", "r+b")
            file.seek(0x3515)
            file.write(b'\x9C\x03\x08')
            file.close()
        case "maps/map6_s01" | "build/USA/out/VIN/MAP6_S01.BIN":
            # Random bytes at end of rodata section, linker artifact?
            file = open("build/USA/out/VIN/MAP6_S01.BIN", "r+b")
            file.seek(0x25e6)
            file.write(b'\x40\x14')
            file.close()
