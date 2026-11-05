import os
import sys

# Patches garbage bytes back into built binaries.
#
# When the game was originally built, the `psylink` linker they used seems to have 
# a bug where random memory bytes were used as padding bytes between TU segments.
# This data would also be completely different between each revision of the game, 
# making it a little harder to match each one.
#
# Sometimes it was possible to match it by adding a variable into the TU's C file,
# with the random data as the value, though was kinda ugly as we'd have to add in
# data that was completely unused to each affected C (plus adding in `#if VERSION_IS()`
# wrappers for each revision around it)
#
# Ideally C files shouldn't need any knowledge of these garbage bytes though, 
# should be down to linker or post-build patches instead.
#
# We handle the data in these ways:
# - where the garbage data is 4-byte aligned and not part of .bss, splat's unnamed 
#   `rodata` / `data` segment type can be used to keep data as raw ASM bytes, 
#   which get linked in separate to our C code
#
# - otherwise, if it's unaligned or part of .bss, this script is used to patch 
#   the data back in after building (using splat `pad` segment type if needed and
#   patching on top of it)
#
# PATCHES[version][file] is a list of (offset, bytes) entries, `file` being the built binary's path relative to `build/<version>/out/`.
#
PATCHES = {
    #
    # USA patches
    #
    "USA": {
        "1ST/BODYPROG.BIN": [
            (0x13A, b'\x08\x8E'),             # sys/memcard (.rodata)
            (0x9BF, b'\x4C\x00\x95\xAB\x90'), # events/title (.rodata)
            (0x39E9, b'\x61\x69\x6E'),        # gfx/texture_utils (.rodata)
            (0x3FDA, b'\x05\x80'),            # paper_map/paper_map (.rodata)
            (0x6792, b'\x08\x80'),            # sys/demo (.rodata)
            (0x89BDE, b'\x02\x80'),           # gfx/bodyprog_800652F4 (.data)
            (0x8DAB7, b'\x18'),               # libkpad/libkpad (.sdata)
            (0x910CC, b'\x3C\x38\x92\xD8'),   # screen/fade (.bss)
            (0x981B4, b'\x80\x18\x13'),       # sys/syswork_globals (.bss)
            (0x981E1, b'\x70\xE5\x8D'),       # sys/memcard_2 (.bss)
            (0x981E4, b'\x05\x00\x02\x24'),   # sys/memcard_2 (.bss)
            (0x981EC, b'\x28\x00\x02\x24'),   # game_boot/game_boot (.bss)
            (0x9821C, b'\x21\x10\x49'),       # text/map_msg (.bss)
            (0x9827C, b'\x22\x00\xA2\x97'),   # events/game_sys_states (.bss)
            (0x98284, b'\x21\x10\x46'),       # events/title (.bss)
            (0x9B08C, b'\x02\x00\x01'),       # world/world_draw (.bss)
            (0x9CA4C, b'\x04\xCF\x41\x76'),   # world/world_map (.bss)
            (0x9CA55, b'\x18\xE3\xB8'),       # chara/bone (.bss)
            (0x9F75C, b'\x10\x00\xB0\x8F'),   # gfx/billboard (.bss)
            (0x9F774, b'\x21\x80\x80'),       # sound/sfx (.bss)
            (0x9F8F6, b'\x02'),               # paper_map/paper_map (.bss)
            (0x9F96C, b'\x10\x00\xB0\x8F'),   # collision/collision (.bss)
            (0x9FABC, b'\x30\x00\xA4\x27'),   # player/player_control (.bss)
            (0x9FACC, b'\x61\x26\x01\x0C'),   # player/player_control (.bss)
            (0x9FD8A, b'\x42\x30'),           # ranking (.bss)
            (0x9FD8C, b'\x07\x00\x40\x14'),   # ranking (.bss)
            (0x9FD94, b'\x0F\x80\x02\x3C'),   # credits_init (.bss)
            (0xA23DC, b'\x0A\x09\x2E\x62'),   # credits_init (.bss)
            (0xA24CC, b'\x38\x0D\x0A\x09'),   # credits_init (.bss)
            (0xA2B8C, b'\x0D\x0A\x09\x2E'),   # libsd/smf_snd (.bss)
            (0xA3F9C, b'\x09\x30\x78\x61')    # libsd/smf_io (.bss)
        ],
        "1ST/B_KONAMI.BIN": [
            (0xF79, b'\x1F\xCE\xF3') # b_konami (.data)
        ],
        "VIN/STREAM.BIN": [
            (0x12AE4, b'\xE4\x73\x40\xAC') # stream (.bss)
        ],
        "VIN/STF_ROLL.BIN": [
            (0x3873, b'\x20') # credits (.data)
        ],
        "VIN/OPTION.BIN": [
            (0x742, b'\x40\x10') # options (.rodata)
        ],
        "VIN/MAP3_S06.BIN": [
            (0x1D65, b'\x65\x74\x5F') # map3_s06_2 (.rodata)
        ],
        "VIN/MAP4_S05.BIN": [
            (0x1729, b'\x65\x5F\x77') # map4_s05 (.rodata)
        ],
        "VIN/MAP5_S01.BIN": [
            (0x3515, b'\x9C\x03\x08') # map5_s01 (.rodata)
        ],
        "VIN/MAP6_S01.BIN": [
            (0x25E6, b'\x40\x14') # map6_s01_2 (.rodata)
        ]
    },

    #
    # EUR patches
    #
    "EUR": {
        "1ST/BODYPROG.BIN": [
            (0x136, b'\x2C'),                 # sys/memcard (.rodata)
            (0x139, b'\xAF\x28'),             # sys/memcard (.rodata)
            (0x9BF, b'\xB2\x00\x08\xDD\x86'), # events/title (.rodata)
            (0x2B82, b'\x05\x80'),            # paper_map/paper_map (.rodata)
            (0x533A, b'\x08\x80'),            # sys/demo (.rodata)
            (0x83F2A, b'\x40\x08'),           # text/map_msg (.data)
            (0x87A0A, b'\x01\x02'),           # text/text_draw (.data)
            (0x8850E, b'\x0A\x80'),           # sound/sfx (.data)
            (0x886A6, b'\x0A\x80'),           # gfx/bodyprog_800652F4 (.data)
            (0x8C577, b'\x18'),               # libkpad/libkpad (.sdata)
            (0x8FB8C, b'\x74\x7A\x74\x20'),   # screen/fade (.bss)
            (0x96CA1, b'\x01\x2C'),           # sys/memcard_2 (.bss)
            (0x96CA4, b'\x48\x02'),           # sys/memcard_2 (.bss)
            (0x96CAC, b'\x4A\xA4\x01\x02'),   # game_boot/game_boot (.bss)
            (0x96CDC, b'\x01\x02\x84'),       # text/map_msg (.bss)
            (0x96D3D, b'\x08\x3E\x1D'),       # events/game_sys_states (.bss)
            (0x96D45, b'\x12\x08'),           # events/title (.bss)
            (0x99B4F, b'\x50'),               # world/world_draw (.bss)
            (0x9B50C, b'\x80'),               # world/world_map (.bss)
            (0x9B50E, b'\xFC'),               # world/world_map (.bss)
            (0x9B516, b'\xF8'),               # chara/bone (.bss)
            (0x9D73E, b'\x5C'),               # sound/sd_call (.bss)
            (0x9D7C4, b'\x2C\x04\x04'),       # view/vw_main (.bss)
            (0x9D884, b'\x04\x2C\x04\x03'),   # text/text_draw (.bss)
            (0x9D8C5, b'\x0A\x52\xCC'),       # items/item_utils (.bss)
            (0x9D902, b'\x04\xFC'),           # items/item_screens_2 (.bss)
            (0x9D906, b'\x0A\x4A'),           # items/item_screens_2 (.bss)
            (0x9E21E, b'\x43\x8C'),           # gfx/billboard (.bss)
            (0x9E235, b'\x25\x09'),           # sound/sfx (.bss)
            (0x9E3B6, b'\x02'),               # paper_map/paper_map (.bss)
            (0x9E42C, b'\x02\x13\x08'),       # collision/collision (.bss)
            (0x9E57E, b'\x02\x3C'),           # player/player_control (.bss)
            (0x9E58C, b'\x86\xF7\x42\x26'),   # player/player_control (.bss)
            (0x9E84A, b'\x62\x14\x47'),       # ranking (.bss)
            (0x9E84E, b'\x02\x24'),           # ranking (.bss)
            (0x9E854, b'\x13'),               # credits_init (.bss)
            (0x9E856, b'\x02\x24'),           # credits_init (.bss)
            (0xA0E9E, b'\x0C\x0C'),           # credits_init (.bss)
            (0xA0F8C, b'\x24\x0A'),           # credits_init (.bss)
            (0xA0F8F, b'\x05'),               # credits_init (.bss)
            (0xA164E, b'\x02\x0F'),           # libsd/smf_snd (.bss)
            (0xA2A5C, b'\x24\x10\x50')        # libsd/smf_io (.bss)
        ],
        "1ST/B_KONAMI.BIN": [
            (0xE55, b'\x5C\x63\x61') # b_konami (.data)
        ],
        "VIN/STREAM.BIN": [
            (0x12AE4, b'\x06\x00\xF4\x03') # stream (.bss)
        ],
        "VIN/STF_ROLL.BIN": [
            (0x387B, b'\x0A') # credits (.data)
        ],
        "VIN/OPTION.BIN": [
            (0x81A, b'\x0D\x0A') # options (.rodata)
        ]
    },

    #
    # JAP0 patches
    #
    "JAP0": {
        "1ST/BODYPROG.BIN": [
            (0x12E, b'\xE8\xFF'),                       # sys/memcard (.rodata)
            (0x9AB, b'\x2C'),                           # events/title (.rodata)
            (0x3709, b'\x2C\x28\x20'),                  # gfx/texture_utils (.rodata)
            (0x3CFA, b'\x0D\x0A'),                      # paper_map/paper_map (.rodata)
            (0x6452, b'\x09\x20'),                      # sys/demo (.rodata)
            (0x871EE, b'\x24\x33'),                     # text/map_msg (.data)
            (0x8BE06, b'\x55\x54'),                     # sound/sfx (.data)
            (0x8BF9E, b'\x2C\x34'),                     # gfx/bodyprog_800652F4 (.data)
            (0x8FFE7, b'\x32'),                         # libkpad/libkpad (.sdata)
            (0x935FC, b'\xAF\x28\xFF\xAD'),             # screen/fade (.bss)
            (0x9A6E6, b'\x60'),                         # sys/syswork_globals (.bss)
            (0x9A712, b'\x08\x00\x09\x00\x08\x00'),     # sys/memcard_2 (.bss)
            (0x9A71E, b'\x58'),                         # game_boot/game_boot (.bss)
            (0x9A74C, b'\x04\x04\x04\x04'),             # text/map_msg (.bss)
            (0x9D5BD, b'\xFE'),                         # world/world_draw (.bss)
            (0x9EF7C, b'\x10\x00\xB0\xAF'),             # world/world_map (.bss)
            (0x9EF87, b'\x16'),                         # chara/bone (.bss)
            (0xA22B9, b'\x4E\x0A\x09\x09\x09\x82\xB7'), # ranking (.bss)
            (0xA22C4, b'\x8C\x8C\x82\xC6'),             # credits_init (.bss)
            (0xA490C, b'\x3A\x20\x25\x64'),             # credits_init (.bss)
            (0xA49FC, b'\x54\x81\x09\x80'),             # credits_init (.bss)
            (0xA50BC, b'\x73\x5F\x6F\x72'),             # libsd/smf_snd (.bss)
            (0xA64CC, b'\x6F\x72\x67\x65')              # libsd/smf_io (.bss)
        ],
        "1ST/B_KONAMI.BIN": [
            (0x10A5, b'\x20\x20\x39') # b_konami (.data)
        ],
        "VIN/STREAM.BIN": [
            (0x12B2C, b'\x56\x49\x45\x57') # stream (.bss)
        ],
        "VIN/STF_ROLL.BIN": [
            (0x386B, b'\x8D') # credits (.data)
        ],
        "VIN/OPTION.BIN": [
            (0xB62, b'\x68\x98') # options (.rodata)
        ],
        "VIN/MAP1_S04.BIN": [
            (0x13AD, b'\x10\x43\x00') # map1_s04 (.rodata)
        ],
        "VIN/MAP2_S03.BIN": [
            (0x1356, b'\xB0\xAF') # map2_s03 (.rodata)
        ],
        "VIN/MAP3_S06.BIN": [
            (0x1CA5, b'\x4A\x30\x28') # map3_s06_2 (.rodata)
        ],
        "VIN/MAP4_S00.BIN": [
            (0x1325, b'\x90\x01\x00') # map4_s00 (.rodata)
        ],
        "VIN/MAP4_S05.BIN": [
            (0x16F9, b'\x20\x89\xBA') # map4_s05 (.rodata)
        ],
        "VIN/MAP4_S06.BIN": [
            (0x1325, b'\x61\x6D\x65') # map4_s06 (.rodata)
        ],
        "VIN/MAP5_S01.BIN": [
            (0x34D5, b'\x0F\x63\x24') # map5_s01 (.rodata)
        ],
        "VIN/MAP6_S01.BIN": [
            (0x245E, b'\x00\x00') # map6_s01_2 (.rodata)
        ],
        "VIN/MAP6_S05.BIN": [
            (0x1339, b'\x20') # map6_s05 (.rodata)
        ]
    },
    
    #
    # JAP1 patches
    #
    "JAP1": {
        "1ST/BODYPROG.BIN": [
            (0x12F, b'\xFE'),                           # sys/memcard (.rodata)
            (0x9B7, b'\x20'),                           # events/title (.rodata)
            (0x3715, b'\xBA\x82\xC9'),                  # gfx/texture_utils (.rodata)
            (0x3D06, b'\x05\x80'),                      # paper_map/paper_map (.rodata)
            (0x64BE, b'\x08\x80'),                      # sys/demo (.rodata)
            (0x8C0C2, b'\x02\x80'),                     # gfx/bodyprog_800652F4 (.data)
            (0x9010A, b'\x00\x17\xA6\x17\xA9\x17'),     # libkpad/libkpad (.sdata)
            (0x93724, b'\xFF\x0F\xFF\x03'),             # screen/fade (.bss)
            (0x9A80C, b'\x1D\x00\x04\xA2'),             # sys/syswork_globals (.bss)
            (0x9A839, b'\x11\x03\x00\x08\x41\x03\x08'), # sys/memcard_2 (.bss)
            (0x9A844, b'\x68\x00\x42\x24'),             # game_boot/game_boot (.bss)
            (0x9A874, b'\x1A\x00\x03\xA6'),             # text/map_msg (.bss)
            (0x9A8D4, b'\x00\x00\x03\xAE'),             # events/game_sys_states (.bss)
            (0x9A8DC, b'\x0D\x80\x04\x3C'),             # events/title (.bss)
            (0x9D6E4, b'\xB0\xB1\xB8\x3A'),             # world/world_draw (.bss)
            (0x9F0A4, b'\x28\x00\x00\xAE'),             # world/world_map (.bss)
            (0x9F0AD, b'\x00\x00\xAE'),                 # chara/bone (.bss)
            (0xA145C, b'\x08\x00\x83\xAC'),             # items/item_utils (.bss)
            (0xA1498, b'\x00\x00\x02\x24'),             # items/item_screens_2 (.bss)
            (0xA149C, b'\xE8\xFF\xBD\x27'),             # items/item_screens_2 (.bss)
            (0xA1DB4, b'\x65\x7E\x03\x0C'),             # gfx/billboard (.bss)
            (0xA1DCC, b'\x6C\x00\x42\x8C'),             # sound/sfx (.bss)
            (0xA1F4C, b'\x00\x00\x02\x3C'),             # paper_map/paper_map (.bss)
            (0xA1FC4, b'\x02\x00\x42\xA2'),             # collision/collision (.bss)
            (0xA2114, b'\x2C\x00\x40\xAC'),             # player/player_control (.bss)
            (0xA2124, b'\x0B\x00\x03\x3C'),             # player/player_control (.bss)
            (0xA23E3, b'\xAE'),                         # ranking (.bss)
            (0xA23E4, b'\x01\x00\x63\x24'),             # ranking (.bss)
            (0xA23EC, b'\x0C\x00\x03\xAE'),             # credits_init (.bss)
            (0xA4A34, b'\x0A\x09\x2E\x62'),             # credits_init (.bss)
            (0xA4B24, b'\x09\x2E\x62\x79'),             # credits_init (.bss)
            (0xA51E4, b'\x2E\x62\x79\x74'),             # libsd/smf_snd (.bss)
            (0xA65F4, b'\x0A\x09\x2E\x62')              # libsd/smf_io (.bss)
        ],
        "1ST/B_KONAMI.BIN": [
            (0x10A5, b'\x0D\x0A\x09') # b_konami (.data)
        ],
        "VIN/STREAM.BIN": [
            (0x12B2C, b'\x56\x49\x45\x57') # stream (.bss)
        ],
        "VIN/STF_ROLL.BIN": [
            (0x3873, b'\x04') # credits (.data)
        ],
        "VIN/OPTION.BIN": [
            (0xB62, b'\x68\x98') # options (.rodata)
        ],
        "VIN/MAP1_S04.BIN": [
            (0x13AD, b'\x10\x43') # map1_s04 (.rodata)
        ],
        "VIN/MAP2_S03.BIN": [
            (0x1356, b'\xB0\xAF') # map2_s03 (.rodata)
        ],
        "VIN/MAP3_S06.BIN": [
            (0x1CA5, b'\x4A\x30\x28') # map3_s06_2 (.rodata)
        ],
        "VIN/MAP4_S00.BIN": [
            (0x1325, b'\x90\x01') # map4_s00 (.rodata)
        ],
        "VIN/MAP4_S05.BIN": [
            (0x16F9, b'\x20\x89\xBA') # map4_s05 (.rodata)
        ],
        "VIN/MAP4_S06.BIN": [
            (0x1325, b'\x28\x20\x20') # map4_s06 (.rodata)
        ],
        "VIN/MAP5_S01.BIN": [
            (0x34D5, b'\xA8\x03\x08') # map5_s01 (.rodata)
        ],
        "VIN/MAP6_S01.BIN": [
            (0x245E, b'\x00\x00') # map6_s01_2 (.rodata)
        ],
        "VIN/MAP6_S05.BIN": [
            (0x1339, b'\x20') # map6_s05 (.rodata)
        ]
    },
    
    #
    # JAP2 patches
    #
    "JAP2": {
        "1ST/BODYPROG.BIN": [
            (0x12F, b'\x0A'),                           # sys/memcard (.rodata)
            (0x3715, b'\x20\x80\x02'),                  # gfx/texture_utils (.rodata)
            (0x3D06, b'\x03\x08'),                      # paper_map/paper_map (.rodata)
            (0x64BE, b'\xA4\x8E'),                      # sys/demo (.rodata)
            (0x87312, b'\x02'),                         # text/map_msg (.data)
            (0x8BF2A, b'\x04\x80'),                     # sound/sfx (.data)
            (0x93724, b'\xE4\xD3\x00\x0C'),             # screen/fade (.bss)
            (0x9A80C, b'\x01\x28'),                     # sys/syswork_globals (.bss)
            (0x9A83A, b'\x01'),                         # sys/memcard_2 (.bss)
            (0x9A83C, b'\x03'),                         # sys/memcard_2 (.bss)
            (0x9A844, b'\x03'),                         # game_boot/game_boot (.bss)
            (0x9A874, b'\x03'),                         # text/map_msg (.bss)
            (0x9A877, b'\xFE'),                         # text/map_msg (.bss)
            (0x9A8D4, b'\xFD\x03'),                     # events/game_sys_states (.bss)
            (0x9A8DC, b'\xFD\x03\x01\x04'),             # events/title (.bss)
            (0x9D6E4, b'\x25\x18\x62'),                 # world/world_draw (.bss)
            (0x9F0A4, b'\x02\x00\x62\x28'),             # world/world_map (.bss)
            (0xA12D6, b'\x0D\x0D'),                     # sound/sd_call (.bss)
            (0xA135C, b'\x0D\x0D\x0D\x0D'),             # view/vw_main (.bss)
            (0xA145C, b'\x03\x05\x1E\x3A'),             # items/item_utils (.bss)
            (0xA1499, b'\x07\x29\x3A\x04\x07\x2A\x3A'), # items/item_screens_2 (.bss)
            (0xA1DB5, b'\x08\x01'),                     # gfx/billboard (.bss)
            (0xA1DCC, b'\xA0\x0A\x99\x11'),             # sound/sfx (.bss)
            (0xA1F4E, b'\x01'),                         # paper_map/paper_map (.bss)
            (0xA1FC4, b'\x34\xF3\xFF\xFF'),             # collision/collision (.bss)
            (0xA2114, b'\xBD\x82\xDC\x82'),             # player/player_control (.bss)
            (0xA2124, b'\x09\x8D\xB0\x82'),             # player/player_control (.bss)
            (0xA23E1, b'\x0A\x09\x09\x09\x82\xE0\x82'), # ranking (.bss)
            (0xA23EC, b'\xA0\x82\xCC\x95'),             # credits_init (.bss)
            (0xA51E4, b'\x30\x0D\x0A\x09'),             # libsd/smf_snd (.bss)
            (0xA65F4, b'\x09\x30\x78\x33')              # libsd/smf_io (.bss)
        ],
        "1ST/B_KONAMI.BIN": [
            (0x10A5, b'\x00\xAC\x8F') # b_konami (.data)
        ],
        "VIN/STREAM.BIN": [
            (0x12B2C, b'\x56\x49\x45\x57') # stream (.bss)
        ],
        "VIN/STF_ROLL.BIN": [
            (0x3873, b'\x10') # credits (.data)
        ],
        "VIN/OPTION.BIN": [
            (0xB62, b'\x68\x98') # options (.rodata)
        ],
        "VIN/MAP1_S04.BIN": [
            (0x13AD, b'\x10\x43') # map1_s04 (.rodata)
        ],
        "VIN/MAP2_S03.BIN": [
            (0x1356, b'\xB0\xAF') # map2_s03 (.rodata)
        ],
        "VIN/MAP3_S06.BIN": [
            (0x1CA5, b'\x4A\x30\x28') # map3_s06_2 (.rodata)
        ],
        "VIN/MAP4_S00.BIN": [
            (0x1325, b'\x90\x01') # map4_s00 (.rodata)
        ],
        "VIN/MAP4_S05.BIN": [
            (0x16F9, b'\x20\x89\xBA') # map4_s05 (.rodata)
        ],
        "VIN/MAP4_S06.BIN": [
            (0x1325, b'\x10\xC3') # map4_s06 (.rodata)
        ],
        "VIN/MAP5_S01.BIN": [
            (0x34D5, b'\xA6\x03\x08') # map5_s01 (.rodata)
        ],
        "VIN/MAP6_S01.BIN": [
            (0x245E, b'\x00\x00') # map6_s01_2 (.rodata)
        ],
        "VIN/MAP6_S05.BIN": [
            (0x1339, b'\x20') # map6_s05 (.rodata)
        ]
    }
}

MAIN_EXE_PATHS = [
    "build/USA/out/SLUS_007.07",
    "build/EUR/out/SLES_015.14",
    "build/JAP0/out/SLPM_861.92",
    "build/JAP1/out/SLPM_861.92",
    "build/JAP2/out/SLPM_861.92",
]

# Targets given by the Makefile -> built file, relative to `build/<version>/out/`.
def target_file(target):
    if target == "bodyprog":
        return "1ST/BODYPROG.BIN"
    if target.startswith("screens/"):
        name = target.split("/")[-1]
        return ("1ST/" if name == "b_konami" else "VIN/") + name.upper() + ".BIN"
    if target.startswith("maps/"):
        return "VIN/" + target.split("/")[-1].upper() + ".BIN"
    return None

def apply_patches(target):
    # Either a Makefile target name or a full `build/<version>/out/<file>` path.
    versions = list(PATCHES)
    if target.startswith("build/"):
        parts = target.split("/")
        versions = [parts[1]]
        file = "/".join(parts[3:])
    else:
        file = target_file(target)
    if file is None:
        return

    for version in versions:
        path = f"build/{version}/out/{file}"
        patches = PATCHES.get(version, {}).get(file)
        if not patches or not os.path.isfile(path):
            continue
        with open(path, "r+b") as f:
            for offset, data in patches:
                f.seek(offset)
                f.write(data)

# TS seems to have trimmed the main EXE down by truncating some bytes from .bss.
# Since those are zero-filled it wouldn't cause an issue for the game, but means we have to trim it too.
# We can use the size from original EXE header (+ 0x800 header size) to tell us what the expected size should be.
# TODO: Our `libgpu/sys.o` lib object was edited to trim the .bss for USA, that can be undone to closer match original lib.
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
    else:
        apply_patches(sys.argv[1])
