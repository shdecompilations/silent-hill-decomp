# Configuration

# BUILD_ITEMS only available for the European release.

BUILD_EXE      ?= 1
BUILD_ENGINE   ?= 1
BUILD_SCREENS  ?= 1
BUILD_MAPS     ?= 1
BUILD_ITEMS    ?= 1
CHECKSUM       ?= 1
NON_MATCHING   ?= 0
SKIP_ASM       ?= 0
# Compiler warning flags, default `-w` disables warnings.
WARN_FLAGS     ?= -w

# Enables usage of the `COMMON` segment.
# The compiler can create a segment call `COMMON` intended to handle repetitive
# global variables declared in files which later becomes simple data that then
# get assigned into some data segment. This comes as default and can be disabled,
# however, MASPSX by default do not emit the segment and instead assign data
# directly into the data segments. The importance of having this segment
# supported is specially in the way MASPSX handle data order in a different way
# to how the actual `COMMON` segment handles data order. It is required to have
# the segment supported in the linker script, even though Splat doesn't natively
# support it. Additionally, the linker will reorder variables based on their names.
USE_COMMON     ?= 0

# Fixes Objdiff build feature.
OBJDIFF_FIX    ?= 0

# Names and Paths

# Retail versions supported:
#
# American
# European
# Japanese
# Japanese REV 1
# Japanese REV 2

GAME_VERSION = USA

ifeq ($(GAME_VERSION), USA)

# Version - Retail NTSC (1.1)

GAME_NAME        := SLUS-00707
GAME_VERSION_DIR := USA
GAME_FILE_EXE    := SLUS_007.07
GAME_FILE_SILENT := SILENT.
GAME_FILE_HILL   := HILL.

else ifeq ($(GAME_VERSION), EUR)

# Version - Retail PAL (1.0)

GAME_NAME        := SLES-01514
GAME_VERSION_DIR := EUR
GAME_FILE_EXE    := SLES_015.14
GAME_FILE_SILENT := SILENT.
GAME_FILE_HILL   := HILL.

else ifeq ($(GAME_VERSION), JAP0)

# Version - Retail NTSC-J (1.0)

GAME_NAME        := SLPM-86192
GAME_VERSION_DIR := JAP0
GAME_FILE_EXE    := SLPM_861.92
GAME_FILE_SILENT := SILENT.
GAME_FILE_HILL   := HILL.

else ifeq ($(GAME_VERSION), JAP1)

# Version - Retail NTSC-J (1.1)

GAME_NAME        := SLPM-86498
GAME_VERSION_DIR := JAP1
GAME_FILE_EXE    := SLPM_861.92
GAME_FILE_SILENT := SILENT.
GAME_FILE_HILL   := HILL.

else ifeq ($(GAME_VERSION), JAP2)

# Version - Retail NTSC-J (1.2)

GAME_NAME        := SLPM-87029
GAME_VERSION_DIR := JAP2
GAME_FILE_EXE    := SLPM_861.92
GAME_FILE_SILENT := SILENT.
GAME_FILE_HILL   := HILL.

endif

# Encoding of map text in the game's binaries, C sources are converted to it from UTF-8.
# Everything else is Shift-JIS in every release, including the memory card save titles in PAL.
ifeq ($(GAME_VERSION), EUR)
MAP_TEXT_ENCODING := ISO-8859-1
else
MAP_TEXT_ENCODING := SHIFT-JIS
endif

ROM_DIR         := rom
CONFIG_BASE_DIR := configs
CONFIG_DIR      := $(CONFIG_BASE_DIR)/$(GAME_VERSION_DIR)
LINKER_BASE_DIR := linkers
LINKER_DIR      := $(LINKER_BASE_DIR)/$(GAME_VERSION_DIR)
IMAGE_DIR       := $(ROM_DIR)/image
BUILD_BASE_DIR  := build
BUILD_DIR       := $(BUILD_BASE_DIR)/$(GAME_VERSION_DIR)
OUT_DIR         := $(BUILD_DIR)/out
TOOLS_DIR       := tools
OBJDIFF_DIR     := $(TOOLS_DIR)/objdiff
PERMUTER_DIR    := permuter
ASSETS_DIR      := assets
ASM_BASE_DIR    := asm
ASM_DIR         := $(ASM_BASE_DIR)/$(GAME_VERSION_DIR)
C_DIR           := src
EXPECTED_DIR    := expected

# Tools
# Note: CPP stands for `C Pre-Processor` not `C++`

CROSS   := mips-linux-gnu
AS      := $(CROSS)-as
LD      := $(CROSS)-ld
OBJCOPY := $(CROSS)-objcopy
OBJDUMP := $(CROSS)-objdump
CPP     := $(CROSS)-cpp
CC      := $(TOOLS_DIR)/gcc-2.8.1-psx/cc1
CC272   := $(TOOLS_DIR)/gcc-2.7.2-cdk/cc1
OBJDIFF := $(OBJDIFF_DIR)/objdiff

PYTHON          := python3
SPLAT           := $(PYTHON) -m splat split
MASPSX          := $(PYTHON) $(TOOLS_DIR)/maspsx/maspsx.py
DUMPSXISO       := $(TOOLS_DIR)/psxiso/dumpsxiso
MKPSXISO        := $(TOOLS_DIR)/psxiso/mkpsxiso
SILENT_ASSETS   := $(PYTHON) $(TOOLS_DIR)/silentassets/extract.py
INSERT_OVLS     := $(PYTHON) $(TOOLS_DIR)/silentassets/insertovl.py
GET_YAML_TARGET := $(PYTHON) $(TOOLS_DIR)/get_yaml_target.py
GET_O_FILES     := $(PYTHON) $(TOOLS_DIR)/get_o_files.py
POSTBUILD       := $(PYTHON) $(TOOLS_DIR)/postbuild.py
COMPTEST        := $(TOOLS_DIR)/compilationTest.sh

# Flags
OPT_FLAGS           := -O2
DL_FLAGS            := -G0
ENDIAN              := -EL
INCLUDE_FLAGS       := -Iinclude -I $(BUILD_DIR) -Iinclude/psyq -Iinclude/decomp
DEFINE_FLAGS        := -D_LANGUAGE_C -DUSE_INCLUDE_ASM
CPP_FLAGS           := $(INCLUDE_FLAGS) $(DEFINE_FLAGS) -P -MMD -MP -undef -Wall -lang-c -nostdinc -DVER_${GAME_VERSION}
ifeq ($(USE_COMMON),1)
LD_FLAGS_GCSECTIONS := --no-gc-sections
COMMON_FLAG         := --use-comm-section
else
LD_FLAGS_GCSECTIONS :=
COMMON_FLAG         :=
endif
LD_FLAGS            := $(ENDIAN) $(DL_FLAGS) $(OPT_FLAGS) $(LD_FLAGS_GCSECTIONS) -nostdlib --no-check-sections
OBJCOPY_FLAGS       := -O binary
OBJDUMP_FLAGS       := --disassemble-all --reloc --disassemble-zeroes -Mreg-names=32
ifeq ($(GEN_COMP_TU),1)
SPLAT_FLAGS         := --disassemble-all --make-full-disasm-for-code
else
SPLAT_FLAGS         := --disassemble-all
endif
DUMPSXISO_FLAGS     := -x "$(ROM_DIR)/$(GAME_VERSION)" -s "$(ROM_DIR)/$(GAME_VERSION)/layout.xml" "$(IMAGE_DIR)/$(GAME_NAME).bin"
MKPSXISO_FLAGS      := -y -q "$(ROM_DIR)/$(GAME_VERSION)/rebuild.xml"
SILENT_ASSETS_FLAGS := -exe "$(ROM_DIR)/$(GAME_VERSION)/$(GAME_FILE_EXE)" -fs "$(ROM_DIR)/$(GAME_VERSION)/$(GAME_FILE_SILENT)" -fh "$(ROM_DIR)/$(GAME_VERSION)/$(GAME_FILE_HILL)" "$(ASSETS_DIR)/$(GAME_VERSION)"
INSERT_OVLS_FLAGS   := -exe "$(ROM_DIR)/$(GAME_VERSION)/$(GAME_FILE_EXE)" -fs "$(ROM_DIR)/$(GAME_VERSION)/$(GAME_FILE_SILENT)" -ftb "$(ASSETS_DIR)/$(GAME_VERSION)/filetable.c.inc" -b $(OUT_DIR) -xml "$(ROM_DIR)/$(GAME_VERSION)/layout.xml" -o $(ROM_DIR)

# Targets that will run tools/postbuild.py after being linked & extracted from ELF.
TARGET_POSTBUILD := main bodyprog screens/b_konami screens/stf_roll screens/stream screens/option maps/map3_s02 maps/map3_s06 maps/map4_s05 maps/map5_s01 maps/map6_s01 maps/map1_s04 maps/map4_s00 maps/map4_s06 maps/map6_s05 maps/map2_s03

# Adjusts compiler and assembler flags based on source file location.
# - Files under main executable paths use -G8; overlay files use -G0.
# - Enables `--expand-div` for certain `libsd` sources which require it (others can't build with it).
# - Adds overlay-specific compiler flags based on files directory (currently only per-map defines).
# - Switches aspsx-version for libkpad code.
define FlagsSwitch
    $(if $(or $(findstring /main/,$(1)),$(findstring libkpad,$(1))), \
    	$(eval DL_FLAGS = -G8), \
    	$(eval DL_FLAGS = -G0))

	$(eval LD_FLAGS = $(ENDIAN) $(DL_FLAGS) $(OPT_FLAGS) $(LD_FLAGS_GCSECTIONS) -nostdlib --no-check-sections)
	$(eval AS_FLAGS = $(ENDIAN) $(INCLUDE_FLAGS) $(OPT_FLAGS) $(DL_FLAGS) -march=r3000 -mtune=r3000 -no-pad-sections)
	$(eval CC_FLAGS = $(OPT_FLAGS) $(DL_FLAGS) -mips1 -mcpu=3000 $(WARN_FLAGS) -funsigned-char -fpeephole -ffunction-cse -fpcc-struct-return -fcommon -fverbose-asm -msoft-float -mgas -fgnu-linker -quiet)
	
	$(if $(findstring libkpad,$(1)), \
		$(eval ASPSX_VERSION := 2.67), \
		$(eval ASPSX_VERSION := 2.77))

	$(if $(or $(findstring smf_mid,$(1)), $(findstring smf_io,$(1)),), \
		$(eval MASPSX_FLAGS = --gnu-as-path mips-linux-gnu-as --aspsx-version=$(ASPSX_VERSION) $(COMMON_FLAG) --run-assembler --expand-div $(AS_FLAGS)), \
		$(eval MASPSX_FLAGS = --gnu-as-path mips-linux-gnu-as --aspsx-version=$(ASPSX_VERSION) $(COMMON_FLAG) --run-assembler $(AS_FLAGS)))

	$(eval _lang := $(word 1,$(subst /, ,$(patsubst $(BUILD_DIR)/lang/%,%,$(filter $(BUILD_DIR)/lang/%,$(1))))))
	$(eval _path := $(patsubst $(BUILD_DIR)/lang/$(_lang)/%,$(BUILD_DIR)/%,$(strip $(1))))
	$(eval _rel_path := $(patsubst $(BUILD_DIR)/src/maps/%,%,$(patsubst build/${asm}/maps/%,%,$(_path))))
	$(eval _map_name := $(shell echo $(word 1, $(subst /, ,$(_rel_path))) | tr a-z A-Z))
	$(if $(and $(findstring MAP,$(_map_name)),$(findstring _S,$(_map_name))), \
		$(eval OVL_FLAGS := -D$(_map_name)), \
		$(eval OVL_FLAGS :=))
	$(if $(_lang), \
		$(eval OVL_FLAGS += -DLANG_$(shell echo $(_lang) | tr a-z A-Z)))
endef

ifeq ($(NON_MATCHING),1)
	CPP_FLAGS := $(CPP_FLAGS) -DNON_MATCHING
endif

ifeq ($(SKIP_ASM),1)
	CPP_FLAGS := $(CPP_FLAGS) -DSKIP_ASM
endif

# Utils

# Function to find matching .bin files for a target name.
find_bin_files = $(shell find $(ASM_DIR)/$(strip $1) -type f -path "*.bin" 2> /dev/null)

# Function to find matching .s files for a target name.
find_s_files = $(shell find $(ASM_DIR)/$(strip $1) -type f -path "*.s" -not -path "asm/*matchings*" 2> /dev/null)

# Function to find matching .c files for a target name.
find_c_files = $(shell find $(C_DIR)/$(strip $1) -type f -path "*.c" 2> /dev/null)

get_o_files = $(shell $(GET_O_FILES) $1 $(GAME_VERSION_DIR) $2)

# Function to get path to .yaml file for given target.
get_yaml_path = $(addsuffix .yaml,$(addprefix $(CONFIG_DIR)/,$1))

# Function to get target output path for given target.
get_target_out_slow = $(patsubst $(OUT_DIR)/$(GAME_VERSION)%,$(OUT_DIR)%,$(addprefix $(OUT_DIR)/,$(shell $(GET_YAML_TARGET) $(call get_yaml_path,$1))))

# Faster version of the above using a cache lookup.
get_target_out = $(patsubst $(strip $1)|%,%,$(filter $(strip $1)|%,$(TARGET_OUT_CACHE)))

# Template definition for elf target.
# First parameter should be source target with folder (e.g. screens/stf_roll).
# Second parameter should be end target (e.g. build/VIN/STF_ROLL.BIN).
# If we skip the ASM inclusion to determine progress, we will not be able to link. Skip linking, if so.

ifeq ($(SKIP_ASM),1)

define make_elf_target
$2: $2.elf

$2.elf: $(call get_o_files, $1, $(GEN_COMP_TU))
#endef make_elf_target
endef

#else SKIP_ASM
else

define make_elf_target

$2: $2.elf
	$(OBJCOPY) $(OBJCOPY_FLAGS) $$< $$@
ifneq (,$(filter $1,$(TARGET_POSTBUILD)))
	-$(POSTBUILD) $1
endif

$2.elf: $(call get_o_files, $1, $(GEN_COMP_TU))
	@mkdir -p $(dir $2)
	$(LD) $(LD_FLAGS) \
		-Map $2.map \
		-T $(LINKER_DIR)/$1.ld \
		-T $(LINKER_DIR)/$(filter-out ./,$(dir $1))undefined_syms_auto.$(notdir $1).txt \
		-T $(LINKER_DIR)/$(filter-out ./,$(dir $1))undefined_funcs_auto.$(notdir $1).txt \
		-T $(CONFIG_DIR)/lib_externs.ld \
		-o $$@

#endef make_elf_target
endef

#endif SKIP_ASM
endif

# Targets

TARGET_SCREENS_SRC_DIR := screens
TARGET_MAPS_SRC_DIR    := maps
TARGET_ITEMS_SRC_DIR   := items

ifeq ($(EXE)$(ENG)$(BKO)$(STF)$(OPT)$(SAV)$(FMV)$(M00)$(M01)$(M02)$(M10)$(M11)$(M12)$(M13)$(M14)$(M15)$(M16)$(M20)$(M21)$(M22)$(M23)$(M24)$(M30)$(M31)$(M32)$(M33)$(M34)$(M35)$(M36)$(M40)$(M41)$(M42)$(M43)$(M44)$(M45)$(M46)$(M50)$(M51)$(M52)$(M53)$(M60)$(M61)$(M62)$(M63)$(M64)$(M65)$(M70)$(M71)$(M72)$(M73)$(M0X)$(M1X)$(M2X)$(M3X)$(M4X)$(M5X)$(M6X)$(M7X)$(SCR)$(STR)$(MAP)$(ITE)$(ITG)$(ITF)$(ITI)$(ITS)$(ITM),)

ifeq ($(BUILD_EXE), 1)

TARGET_MAIN := main

#endif BUILD_EXE
endif

ifeq ($(BUILD_ENGINE), 1)

TARGET_BODYPROG := bodyprog

#endif BUILD_ENGINE
endif

ifeq ($(BUILD_SCREENS), 1)

TARGET_SCREENS := b_konami option saveload stf_roll stream

ifeq ($(GAME_VERSION), JAP0)
TARGET_SCREENS := $(TARGET_SCREENS) hp_safe1 s__safe2
endif

TARGET_SCREENS := $(addprefix $(TARGET_SCREENS_SRC_DIR)/,$(TARGET_SCREENS))

#endif BUILD_SCREENS
endif

ifeq ($(BUILD_MAPS), 1)


ifeq ($(GAME_VERSION), EUR)

TARGET_MAPS := map0_s00 map0_s01 map0_s02 \
                map1_s00 map1_s01 map1_s02 map1_s03 map1_s04 map1_s05 map1_s06 \
                map2_s00 map2_s01 map2_s02 map2_s03 map2_s04 \
                map3_s00 map3_s01 map3_s02 map3_s03 map3_s04 map3_s05 map3_s06 \
                map4_s00 map4_s01 map4_s02 map4_s03 map4_s04 map4_s05 map4_s06 \
                map5_s00 map5_s01 map5_s02 map5_s03 \
                map6_s00 map6_s01 map6_s02 map6_s03 map6_s04 map6_s05 \
                map7_s00 map7_s01 map7_s02 map7_s03 \
                map0_s10 map0_s11 map0_s12 \
                map1_s10 map1_s11 map1_s12 map1_s13 map1_s14 map1_s15 map1_s16 \
                map2_s10 map2_s11 map2_s12 map2_s13 map2_s14 \
                map3_s10 map3_s11 map3_s12 map3_s13 map3_s14 map3_s15 map3_s16 \
                map4_s10 map4_s11 map4_s12 map4_s13 map4_s14 map4_s15 map4_s16 \
                map5_s10 map5_s11 map5_s12 map5_s13 \
                map6_s10 map6_s11 map6_s12 map6_s13 map6_s14 map6_s15 \
                map7_s10 map7_s11 map7_s12 map7_s13 \
                map0_s20 map0_s21 map0_s22 \
                map1_s20 map1_s21 map1_s22 map1_s23 map1_s24 map1_s25 map1_s26 \
                map2_s20 map2_s21 map2_s22 map2_s23 map2_s24 \
                map3_s20 map3_s21 map3_s22 map3_s23 map3_s24 map3_s25 map3_s26 \
                map4_s20 map4_s21 map4_s22 map4_s23 map4_s24 map4_s25 map4_s26 \
                map5_s20 map5_s21 map5_s22 map5_s23 \
                map6_s20 map6_s21 map6_s22 map6_s23 map6_s24 map6_s25 \
                map7_s20 map7_s21 map7_s22 map7_s23 \
                map0_s30 map0_s31 map0_s32 \
                map1_s30 map1_s31 map1_s32 map1_s33 map1_s34 map1_s35 map1_s36 \
                map2_s30 map2_s31 map2_s32 map2_s33 map2_s34 \
                map3_s30 map3_s31 map3_s32 map3_s33 map3_s34 map3_s35 map3_s36 \
                map4_s30 map4_s31 map4_s32 map4_s33 map4_s34 map4_s35 map4_s36 \
                map5_s30 map5_s31 map5_s32 map5_s33 \
                map6_s30 map6_s31 map6_s32 map6_s33 map6_s34 map6_s35 \
                map7_s30 map7_s31 map7_s32 map7_s33 \
                map0_s40 map0_s41 map0_s42 \
                map1_s40 map1_s41 map1_s42 map1_s43 map1_s44 map1_s45 map1_s46 \
                map2_s40 map2_s41 map2_s42 map2_s43 map2_s44 \
                map3_s40 map3_s41 map3_s42 map3_s43 map3_s44 map3_s45 map3_s46 \
                map4_s40 map4_s41 map4_s42 map4_s43 map4_s44 map4_s45 map4_s46 \
                map5_s40 map5_s41 map5_s42 map5_s43 \
                map6_s40 map6_s41 map6_s42 map6_s43 map6_s44 map6_s45 \
                map7_s40 map7_s41 map7_s42 map7_s43

else

TARGET_MAPS := map0_s00 map0_s01 map0_s02 \
                map1_s00 map1_s01 map1_s02 map1_s03 map1_s04 map1_s05 map1_s06 \
                map2_s00 map2_s01 map2_s02 map2_s03 map2_s04 \
                map3_s00 map3_s01 map3_s02 map3_s03 map3_s04 map3_s05 map3_s06 \
                map4_s00 map4_s01 map4_s02 map4_s03 map4_s04 map4_s05 map4_s06 \
                map5_s00 map5_s01 map5_s02 map5_s03 \
                map6_s00 map6_s01 map6_s02 map6_s03 map6_s04 map6_s05 \
                map7_s00 map7_s01 map7_s02 map7_s03

#endif GAME_VERSION
endif

# If BUILD_MAP is set, only use that; otherwise use the default huge list
# (allows large speedup by skipping config parse of all maps)
TARGET_MAPS				:= $(if $(BUILD_MAP),$(BUILD_MAP),$(TARGET_MAPS))
TARGET_MAPS				:= $(addprefix $(TARGET_MAPS_SRC_DIR)/,$(TARGET_MAPS))

#endif BUILD_MAPS
endif


ifeq ($(GAME_VERSION), EUR)
ifeq ($(BUILD_ITEMS), 1)

TARGET_ITEMS := item_eng item_ger item_frn item_itl item_spn
TARGET_ITEMS := $(addprefix $(TARGET_ITEMS_SRC_DIR)/,$(TARGET_ITEMS))

#endif BUILD_ITEMS
endif
#endif GAME_VERSION
endif

TARGET_IN := $(TARGET_MAIN) $(TARGET_BODYPROG) $(TARGET_SCREENS) $(TARGET_MAPS) $(TARGET_ITEMS)

else
# Specific overlay compilation/generation

ifeq ($(GAME_VERSION), EUR)

TARGET_MAPS := map0_s00 map0_s01 map0_s02 \
                map1_s00 map1_s01 map1_s02 map1_s03 map1_s04 map1_s05 map1_s06 \
                map2_s00 map2_s01 map2_s02 map2_s03 map2_s04 \
                map3_s00 map3_s01 map3_s02 map3_s03 map3_s04 map3_s05 map3_s06 \
                map4_s00 map4_s01 map4_s02 map4_s03 map4_s04 map4_s05 map4_s06 \
                map5_s00 map5_s01 map5_s02 map5_s03 \
                map6_s00 map6_s01 map6_s02 map6_s03 map6_s04 map6_s05 \
                map7_s00 map7_s01 map7_s02 map7_s03 \
                map0_s10 map0_s11 map0_s12 \
                map1_s10 map1_s11 map1_s12 map1_s13 map1_s14 map1_s15 map1_s16 \
                map2_s10 map2_s11 map2_s12 map2_s13 map2_s14 \
                map3_s10 map3_s11 map3_s12 map3_s13 map3_s14 map3_s15 map3_s16 \
                map4_s10 map4_s11 map4_s12 map4_s13 map4_s14 map4_s15 map4_s16 \
                map5_s10 map5_s11 map5_s12 map5_s13 \
                map6_s10 map6_s11 map6_s12 map6_s13 map6_s14 map6_s15 \
                map7_s10 map7_s11 map7_s12 map7_s13 \
                map0_s20 map0_s21 map0_s22 \
                map1_s20 map1_s21 map1_s22 map1_s23 map1_s24 map1_s25 map1_s26 \
                map2_s20 map2_s21 map2_s22 map2_s23 map2_s24 \
                map3_s20 map3_s21 map3_s22 map3_s23 map3_s24 map3_s25 map3_s26 \
                map4_s20 map4_s21 map4_s22 map4_s23 map4_s24 map4_s25 map4_s26 \
                map5_s20 map5_s21 map5_s22 map5_s23 \
                map6_s20 map6_s21 map6_s22 map6_s23 map6_s24 map6_s25 \
                map7_s20 map7_s21 map7_s22 map7_s23 \
                map0_s30 map0_s31 map0_s32 \
                map1_s30 map1_s31 map1_s32 map1_s33 map1_s34 map1_s35 map1_s36 \
                map2_s30 map2_s31 map2_s32 map2_s33 map2_s34 \
                map3_s30 map3_s31 map3_s32 map3_s33 map3_s34 map3_s35 map3_s36 \
                map4_s30 map4_s31 map4_s32 map4_s33 map4_s34 map4_s35 map4_s36 \
                map5_s30 map5_s31 map5_s32 map5_s33 \
                map6_s30 map6_s31 map6_s32 map6_s33 map6_s34 map6_s35 \
                map7_s30 map7_s31 map7_s32 map7_s33 \
                map0_s40 map0_s41 map0_s42 \
                map1_s40 map1_s41 map1_s42 map1_s43 map1_s44 map1_s45 map1_s46 \
                map2_s40 map2_s41 map2_s42 map2_s43 map2_s44 \
                map3_s40 map3_s41 map3_s42 map3_s43 map3_s44 map3_s45 map3_s46 \
                map4_s40 map4_s41 map4_s42 map4_s43 map4_s44 map4_s45 map4_s46 \
                map5_s40 map5_s41 map5_s42 map5_s43 \
                map6_s40 map6_s41 map6_s42 map6_s43 map6_s44 map6_s45 \
                map7_s40 map7_s41 map7_s42 map7_s43

else

TARGET_MAPS := map0_s00 map0_s01 map0_s02 \
                map1_s00 map1_s01 map1_s02 map1_s03 map1_s04 map1_s05 map1_s06 \
                map2_s00 map2_s01 map2_s02 map2_s03 map2_s04 \
                map3_s00 map3_s01 map3_s02 map3_s03 map3_s04 map3_s05 map3_s06 \
                map4_s00 map4_s01 map4_s02 map4_s03 map4_s04 map4_s05 map4_s06 \
                map5_s00 map5_s01 map5_s02 map5_s03 \
                map6_s00 map6_s01 map6_s02 map6_s03 map6_s04 map6_s05 \
                map7_s00 map7_s01 map7_s02 map7_s03

#endif GAME_VERSION
endif

TARGET_MAPS := $(addprefix $(TARGET_MAPS_SRC_DIR)/,$(TARGET_MAPS))

TARGET_EXE_$(EXE)             := main
TARGET_ENG_$(ENG)             := bodyprog
TARGET_BKO_$(BKO)$(SCR)       := $(TARGET_SCREENS_SRC_DIR)/b_konami
TARGET_OPT_$(OPT)$(SCR)       := $(TARGET_SCREENS_SRC_DIR)/option
TARGET_SAV_$(SAV)$(SCR)       := $(TARGET_SCREENS_SRC_DIR)/saveload
TARGET_STF_$(STF)$(SCR)       := $(TARGET_SCREENS_SRC_DIR)/stf_roll
TARGET_FMV_$(FMV)$(STR)$(SCR) := $(TARGET_SCREENS_SRC_DIR)/stream
TARGET_MAP_$(MAP)             := $(TARGET_MAPS)

ifeq ($(GAME_VERSION), EUR)

TARGET_ITE_$(ITE)$(ITM) := $(TARGET_ITEMS_SRC_DIR)/item_eng
TARGET_ITG_$(ITG)$(ITM) := $(TARGET_ITEMS_SRC_DIR)/item_ger
TARGET_ITF_$(ITF)$(ITM) := $(TARGET_ITEMS_SRC_DIR)/item_frn
TARGET_ITL_$(ITI)$(ITM) := $(TARGET_ITEMS_SRC_DIR)/item_itl
TARGET_ITS_$(ITS)$(ITM) := $(TARGET_ITEMS_SRC_DIR)/item_spn

TARGET_M00_$(M00)$(M0X) := $(TARGET_MAPS_SRC_DIR)/map0_s00 $(TARGET_MAPS_SRC_DIR)/map0_s10 $(TARGET_MAPS_SRC_DIR)/map0_s20 $(TARGET_MAPS_SRC_DIR)/map0_s30 $(TARGET_MAPS_SRC_DIR)/map0_s40
TARGET_M01_$(M01)$(M0X) := $(TARGET_MAPS_SRC_DIR)/map0_s01 $(TARGET_MAPS_SRC_DIR)/map0_s11 $(TARGET_MAPS_SRC_DIR)/map0_s21 $(TARGET_MAPS_SRC_DIR)/map0_s31 $(TARGET_MAPS_SRC_DIR)/map0_s41
TARGET_M02_$(M02)$(M0X) := $(TARGET_MAPS_SRC_DIR)/map0_s02 $(TARGET_MAPS_SRC_DIR)/map0_s12 $(TARGET_MAPS_SRC_DIR)/map0_s22 $(TARGET_MAPS_SRC_DIR)/map0_s32 $(TARGET_MAPS_SRC_DIR)/map0_s42
TARGET_M10_$(M10)$(M1X) := $(TARGET_MAPS_SRC_DIR)/map1_s00 $(TARGET_MAPS_SRC_DIR)/map1_s10 $(TARGET_MAPS_SRC_DIR)/map1_s20 $(TARGET_MAPS_SRC_DIR)/map1_s30 $(TARGET_MAPS_SRC_DIR)/map1_s40
TARGET_M11_$(M11)$(M1X) := $(TARGET_MAPS_SRC_DIR)/map1_s01 $(TARGET_MAPS_SRC_DIR)/map1_s11 $(TARGET_MAPS_SRC_DIR)/map1_s21 $(TARGET_MAPS_SRC_DIR)/map1_s31 $(TARGET_MAPS_SRC_DIR)/map1_s41
TARGET_M12_$(M12)$(M1X) := $(TARGET_MAPS_SRC_DIR)/map1_s02 $(TARGET_MAPS_SRC_DIR)/map1_s12 $(TARGET_MAPS_SRC_DIR)/map1_s22 $(TARGET_MAPS_SRC_DIR)/map1_s32 $(TARGET_MAPS_SRC_DIR)/map1_s42
TARGET_M13_$(M13)$(M1X) := $(TARGET_MAPS_SRC_DIR)/map1_s03 $(TARGET_MAPS_SRC_DIR)/map1_s13 $(TARGET_MAPS_SRC_DIR)/map1_s23 $(TARGET_MAPS_SRC_DIR)/map1_s33 $(TARGET_MAPS_SRC_DIR)/map1_s43
TARGET_M14_$(M14)$(M1X) := $(TARGET_MAPS_SRC_DIR)/map1_s04 $(TARGET_MAPS_SRC_DIR)/map1_s14 $(TARGET_MAPS_SRC_DIR)/map1_s24 $(TARGET_MAPS_SRC_DIR)/map1_s34 $(TARGET_MAPS_SRC_DIR)/map1_s44
TARGET_M15_$(M15)$(M1X) := $(TARGET_MAPS_SRC_DIR)/map1_s05 $(TARGET_MAPS_SRC_DIR)/map1_s15 $(TARGET_MAPS_SRC_DIR)/map1_s25 $(TARGET_MAPS_SRC_DIR)/map1_s35 $(TARGET_MAPS_SRC_DIR)/map1_s45
TARGET_M16_$(M16)$(M1X) := $(TARGET_MAPS_SRC_DIR)/map1_s06 $(TARGET_MAPS_SRC_DIR)/map1_s16 $(TARGET_MAPS_SRC_DIR)/map1_s26 $(TARGET_MAPS_SRC_DIR)/map1_s36 $(TARGET_MAPS_SRC_DIR)/map1_s46
TARGET_M20_$(M20)$(M2X) := $(TARGET_MAPS_SRC_DIR)/map2_s00 $(TARGET_MAPS_SRC_DIR)/map2_s10 $(TARGET_MAPS_SRC_DIR)/map2_s20 $(TARGET_MAPS_SRC_DIR)/map2_s30 $(TARGET_MAPS_SRC_DIR)/map2_s40
TARGET_M21_$(M21)$(M2X) := $(TARGET_MAPS_SRC_DIR)/map2_s01 $(TARGET_MAPS_SRC_DIR)/map2_s11 $(TARGET_MAPS_SRC_DIR)/map2_s21 $(TARGET_MAPS_SRC_DIR)/map2_s31 $(TARGET_MAPS_SRC_DIR)/map2_s41
TARGET_M22_$(M22)$(M2X) := $(TARGET_MAPS_SRC_DIR)/map2_s02 $(TARGET_MAPS_SRC_DIR)/map2_s12 $(TARGET_MAPS_SRC_DIR)/map2_s22 $(TARGET_MAPS_SRC_DIR)/map2_s32 $(TARGET_MAPS_SRC_DIR)/map2_s42
TARGET_M23_$(M23)$(M2X) := $(TARGET_MAPS_SRC_DIR)/map2_s03 $(TARGET_MAPS_SRC_DIR)/map2_s13 $(TARGET_MAPS_SRC_DIR)/map2_s23 $(TARGET_MAPS_SRC_DIR)/map2_s33 $(TARGET_MAPS_SRC_DIR)/map2_s43
TARGET_M24_$(M24)$(M2X) := $(TARGET_MAPS_SRC_DIR)/map2_s04 $(TARGET_MAPS_SRC_DIR)/map2_s14 $(TARGET_MAPS_SRC_DIR)/map2_s24 $(TARGET_MAPS_SRC_DIR)/map2_s34 $(TARGET_MAPS_SRC_DIR)/map2_s44
TARGET_M30_$(M30)$(M3X) := $(TARGET_MAPS_SRC_DIR)/map3_s00 $(TARGET_MAPS_SRC_DIR)/map3_s10 $(TARGET_MAPS_SRC_DIR)/map3_s20 $(TARGET_MAPS_SRC_DIR)/map3_s30 $(TARGET_MAPS_SRC_DIR)/map3_s40
TARGET_M31_$(M31)$(M3X) := $(TARGET_MAPS_SRC_DIR)/map3_s01 $(TARGET_MAPS_SRC_DIR)/map3_s11 $(TARGET_MAPS_SRC_DIR)/map3_s21 $(TARGET_MAPS_SRC_DIR)/map3_s31 $(TARGET_MAPS_SRC_DIR)/map3_s41
TARGET_M32_$(M32)$(M3X) := $(TARGET_MAPS_SRC_DIR)/map3_s02 $(TARGET_MAPS_SRC_DIR)/map3_s12 $(TARGET_MAPS_SRC_DIR)/map3_s22 $(TARGET_MAPS_SRC_DIR)/map3_s32 $(TARGET_MAPS_SRC_DIR)/map3_s42
TARGET_M33_$(M33)$(M3X) := $(TARGET_MAPS_SRC_DIR)/map3_s03 $(TARGET_MAPS_SRC_DIR)/map3_s13 $(TARGET_MAPS_SRC_DIR)/map3_s23 $(TARGET_MAPS_SRC_DIR)/map3_s33 $(TARGET_MAPS_SRC_DIR)/map3_s43
TARGET_M34_$(M34)$(M3X) := $(TARGET_MAPS_SRC_DIR)/map3_s04 $(TARGET_MAPS_SRC_DIR)/map3_s14 $(TARGET_MAPS_SRC_DIR)/map3_s24 $(TARGET_MAPS_SRC_DIR)/map3_s34 $(TARGET_MAPS_SRC_DIR)/map3_s44
TARGET_M35_$(M35)$(M3X) := $(TARGET_MAPS_SRC_DIR)/map3_s05 $(TARGET_MAPS_SRC_DIR)/map3_s15 $(TARGET_MAPS_SRC_DIR)/map3_s25 $(TARGET_MAPS_SRC_DIR)/map3_s35 $(TARGET_MAPS_SRC_DIR)/map3_s45
TARGET_M36_$(M36)$(M3X) := $(TARGET_MAPS_SRC_DIR)/map3_s06 $(TARGET_MAPS_SRC_DIR)/map3_s16 $(TARGET_MAPS_SRC_DIR)/map3_s26 $(TARGET_MAPS_SRC_DIR)/map3_s36 $(TARGET_MAPS_SRC_DIR)/map3_s46
TARGET_M40_$(M40)$(M4X) := $(TARGET_MAPS_SRC_DIR)/map4_s00 $(TARGET_MAPS_SRC_DIR)/map4_s10 $(TARGET_MAPS_SRC_DIR)/map4_s20 $(TARGET_MAPS_SRC_DIR)/map4_s30 $(TARGET_MAPS_SRC_DIR)/map4_s40
TARGET_M41_$(M41)$(M4X) := $(TARGET_MAPS_SRC_DIR)/map4_s01 $(TARGET_MAPS_SRC_DIR)/map4_s11 $(TARGET_MAPS_SRC_DIR)/map4_s21 $(TARGET_MAPS_SRC_DIR)/map4_s31 $(TARGET_MAPS_SRC_DIR)/map4_s41
TARGET_M42_$(M42)$(M4X) := $(TARGET_MAPS_SRC_DIR)/map4_s02 $(TARGET_MAPS_SRC_DIR)/map4_s12 $(TARGET_MAPS_SRC_DIR)/map4_s22 $(TARGET_MAPS_SRC_DIR)/map4_s32 $(TARGET_MAPS_SRC_DIR)/map4_s42
TARGET_M43_$(M43)$(M4X) := $(TARGET_MAPS_SRC_DIR)/map4_s03 $(TARGET_MAPS_SRC_DIR)/map4_s13 $(TARGET_MAPS_SRC_DIR)/map4_s23 $(TARGET_MAPS_SRC_DIR)/map4_s33 $(TARGET_MAPS_SRC_DIR)/map4_s43
TARGET_M44_$(M44)$(M4X) := $(TARGET_MAPS_SRC_DIR)/map4_s04 $(TARGET_MAPS_SRC_DIR)/map4_s14 $(TARGET_MAPS_SRC_DIR)/map4_s24 $(TARGET_MAPS_SRC_DIR)/map4_s34 $(TARGET_MAPS_SRC_DIR)/map4_s44
TARGET_M45_$(M45)$(M4X) := $(TARGET_MAPS_SRC_DIR)/map4_s05 $(TARGET_MAPS_SRC_DIR)/map4_s15 $(TARGET_MAPS_SRC_DIR)/map4_s25 $(TARGET_MAPS_SRC_DIR)/map4_s35 $(TARGET_MAPS_SRC_DIR)/map4_s45
TARGET_M46_$(M46)$(M4X) := $(TARGET_MAPS_SRC_DIR)/map4_s06 $(TARGET_MAPS_SRC_DIR)/map4_s16 $(TARGET_MAPS_SRC_DIR)/map4_s26 $(TARGET_MAPS_SRC_DIR)/map4_s36 $(TARGET_MAPS_SRC_DIR)/map4_s46
TARGET_M50_$(M50)$(M5X) := $(TARGET_MAPS_SRC_DIR)/map5_s00 $(TARGET_MAPS_SRC_DIR)/map5_s10 $(TARGET_MAPS_SRC_DIR)/map5_s20 $(TARGET_MAPS_SRC_DIR)/map5_s30 $(TARGET_MAPS_SRC_DIR)/map5_s40
TARGET_M51_$(M51)$(M5X) := $(TARGET_MAPS_SRC_DIR)/map5_s01 $(TARGET_MAPS_SRC_DIR)/map5_s11 $(TARGET_MAPS_SRC_DIR)/map5_s21 $(TARGET_MAPS_SRC_DIR)/map5_s31 $(TARGET_MAPS_SRC_DIR)/map5_s41
TARGET_M52_$(M52)$(M5X) := $(TARGET_MAPS_SRC_DIR)/map5_s02 $(TARGET_MAPS_SRC_DIR)/map5_s12 $(TARGET_MAPS_SRC_DIR)/map5_s22 $(TARGET_MAPS_SRC_DIR)/map5_s32 $(TARGET_MAPS_SRC_DIR)/map5_s42
TARGET_M53_$(M53)$(M5X) := $(TARGET_MAPS_SRC_DIR)/map5_s03 $(TARGET_MAPS_SRC_DIR)/map5_s13 $(TARGET_MAPS_SRC_DIR)/map5_s23 $(TARGET_MAPS_SRC_DIR)/map5_s33 $(TARGET_MAPS_SRC_DIR)/map5_s43
TARGET_M60_$(M60)$(M6X) := $(TARGET_MAPS_SRC_DIR)/map6_s00 $(TARGET_MAPS_SRC_DIR)/map6_s10 $(TARGET_MAPS_SRC_DIR)/map6_s20 $(TARGET_MAPS_SRC_DIR)/map6_s30 $(TARGET_MAPS_SRC_DIR)/map6_s40
TARGET_M61_$(M61)$(M6X) := $(TARGET_MAPS_SRC_DIR)/map6_s01 $(TARGET_MAPS_SRC_DIR)/map6_s11 $(TARGET_MAPS_SRC_DIR)/map6_s21 $(TARGET_MAPS_SRC_DIR)/map6_s31 $(TARGET_MAPS_SRC_DIR)/map6_s41
TARGET_M62_$(M62)$(M6X) := $(TARGET_MAPS_SRC_DIR)/map6_s02 $(TARGET_MAPS_SRC_DIR)/map6_s12 $(TARGET_MAPS_SRC_DIR)/map6_s22 $(TARGET_MAPS_SRC_DIR)/map6_s32 $(TARGET_MAPS_SRC_DIR)/map6_s42
TARGET_M63_$(M63)$(M6X) := $(TARGET_MAPS_SRC_DIR)/map6_s03 $(TARGET_MAPS_SRC_DIR)/map6_s13 $(TARGET_MAPS_SRC_DIR)/map6_s23 $(TARGET_MAPS_SRC_DIR)/map6_s33 $(TARGET_MAPS_SRC_DIR)/map6_s43
TARGET_M64_$(M64)$(M6X) := $(TARGET_MAPS_SRC_DIR)/map6_s04 $(TARGET_MAPS_SRC_DIR)/map6_s14 $(TARGET_MAPS_SRC_DIR)/map6_s24 $(TARGET_MAPS_SRC_DIR)/map6_s34 $(TARGET_MAPS_SRC_DIR)/map6_s44
TARGET_M65_$(M65)$(M6X) := $(TARGET_MAPS_SRC_DIR)/map6_s05 $(TARGET_MAPS_SRC_DIR)/map6_s15 $(TARGET_MAPS_SRC_DIR)/map6_s25 $(TARGET_MAPS_SRC_DIR)/map6_s35 $(TARGET_MAPS_SRC_DIR)/map6_s45
TARGET_M70_$(M70)$(M7X) := $(TARGET_MAPS_SRC_DIR)/map7_s00 $(TARGET_MAPS_SRC_DIR)/map7_s10 $(TARGET_MAPS_SRC_DIR)/map7_s20 $(TARGET_MAPS_SRC_DIR)/map7_s30 $(TARGET_MAPS_SRC_DIR)/map7_s40
TARGET_M71_$(M71)$(M7X) := $(TARGET_MAPS_SRC_DIR)/map7_s01 $(TARGET_MAPS_SRC_DIR)/map7_s11 $(TARGET_MAPS_SRC_DIR)/map7_s21 $(TARGET_MAPS_SRC_DIR)/map7_s31 $(TARGET_MAPS_SRC_DIR)/map7_s41
TARGET_M72_$(M72)$(M7X) := $(TARGET_MAPS_SRC_DIR)/map7_s02 $(TARGET_MAPS_SRC_DIR)/map7_s12 $(TARGET_MAPS_SRC_DIR)/map7_s22 $(TARGET_MAPS_SRC_DIR)/map7_s32 $(TARGET_MAPS_SRC_DIR)/map7_s42
TARGET_M73_$(M73)$(M7X) := $(TARGET_MAPS_SRC_DIR)/map7_s03 $(TARGET_MAPS_SRC_DIR)/map7_s13 $(TARGET_MAPS_SRC_DIR)/map7_s23 $(TARGET_MAPS_SRC_DIR)/map7_s33 $(TARGET_MAPS_SRC_DIR)/map7_s43

else

TARGET_M00_$(M00)$(M0X) := $(TARGET_MAPS_SRC_DIR)/map0_s00
TARGET_M01_$(M01)$(M0X) := $(TARGET_MAPS_SRC_DIR)/map0_s01
TARGET_M02_$(M02)$(M0X) := $(TARGET_MAPS_SRC_DIR)/map0_s02
TARGET_M10_$(M10)$(M1X) := $(TARGET_MAPS_SRC_DIR)/map1_s00
TARGET_M11_$(M11)$(M1X) := $(TARGET_MAPS_SRC_DIR)/map1_s01
TARGET_M12_$(M12)$(M1X) := $(TARGET_MAPS_SRC_DIR)/map1_s02
TARGET_M13_$(M13)$(M1X) := $(TARGET_MAPS_SRC_DIR)/map1_s03
TARGET_M14_$(M14)$(M1X) := $(TARGET_MAPS_SRC_DIR)/map1_s04
TARGET_M15_$(M15)$(M1X) := $(TARGET_MAPS_SRC_DIR)/map1_s05
TARGET_M16_$(M16)$(M1X) := $(TARGET_MAPS_SRC_DIR)/map1_s06
TARGET_M20_$(M20)$(M2X) := $(TARGET_MAPS_SRC_DIR)/map2_s00
TARGET_M21_$(M21)$(M2X) := $(TARGET_MAPS_SRC_DIR)/map2_s01
TARGET_M22_$(M22)$(M2X) := $(TARGET_MAPS_SRC_DIR)/map2_s02
TARGET_M23_$(M23)$(M2X) := $(TARGET_MAPS_SRC_DIR)/map2_s03
TARGET_M24_$(M24)$(M2X) := $(TARGET_MAPS_SRC_DIR)/map2_s04
TARGET_M30_$(M30)$(M3X) := $(TARGET_MAPS_SRC_DIR)/map3_s00
TARGET_M31_$(M31)$(M3X) := $(TARGET_MAPS_SRC_DIR)/map3_s01
TARGET_M32_$(M32)$(M3X) := $(TARGET_MAPS_SRC_DIR)/map3_s02
TARGET_M33_$(M33)$(M3X) := $(TARGET_MAPS_SRC_DIR)/map3_s03
TARGET_M34_$(M34)$(M3X) := $(TARGET_MAPS_SRC_DIR)/map3_s04
TARGET_M35_$(M35)$(M3X) := $(TARGET_MAPS_SRC_DIR)/map3_s05
TARGET_M36_$(M36)$(M3X) := $(TARGET_MAPS_SRC_DIR)/map3_s06
TARGET_M40_$(M40)$(M4X) := $(TARGET_MAPS_SRC_DIR)/map4_s00
TARGET_M41_$(M41)$(M4X) := $(TARGET_MAPS_SRC_DIR)/map4_s01
TARGET_M42_$(M42)$(M4X) := $(TARGET_MAPS_SRC_DIR)/map4_s02
TARGET_M43_$(M43)$(M4X) := $(TARGET_MAPS_SRC_DIR)/map4_s03
TARGET_M44_$(M44)$(M4X) := $(TARGET_MAPS_SRC_DIR)/map4_s04
TARGET_M45_$(M45)$(M4X) := $(TARGET_MAPS_SRC_DIR)/map4_s05
TARGET_M46_$(M46)$(M4X) := $(TARGET_MAPS_SRC_DIR)/map4_s06
TARGET_M50_$(M50)$(M5X) := $(TARGET_MAPS_SRC_DIR)/map5_s00
TARGET_M51_$(M51)$(M5X) := $(TARGET_MAPS_SRC_DIR)/map5_s01
TARGET_M52_$(M52)$(M5X) := $(TARGET_MAPS_SRC_DIR)/map5_s02
TARGET_M53_$(M53)$(M5X) := $(TARGET_MAPS_SRC_DIR)/map5_s03
TARGET_M60_$(M60)$(M6X) := $(TARGET_MAPS_SRC_DIR)/map6_s00
TARGET_M61_$(M61)$(M6X) := $(TARGET_MAPS_SRC_DIR)/map6_s01
TARGET_M62_$(M62)$(M6X) := $(TARGET_MAPS_SRC_DIR)/map6_s02
TARGET_M63_$(M63)$(M6X) := $(TARGET_MAPS_SRC_DIR)/map6_s03
TARGET_M64_$(M64)$(M6X) := $(TARGET_MAPS_SRC_DIR)/map6_s04
TARGET_M65_$(M65)$(M6X) := $(TARGET_MAPS_SRC_DIR)/map6_s05
TARGET_M70_$(M70)$(M7X) := $(TARGET_MAPS_SRC_DIR)/map7_s00
TARGET_M71_$(M71)$(M7X) := $(TARGET_MAPS_SRC_DIR)/map7_s01
TARGET_M72_$(M72)$(M7X) := $(TARGET_MAPS_SRC_DIR)/map7_s02
TARGET_M73_$(M73)$(M7X) := $(TARGET_MAPS_SRC_DIR)/map7_s03

#endif GAME_VERSION
endif

TARGET_IN := $(TARGET_EXE_1) $(TARGET_ENG_1) $(TARGET_BKO_1) $(TARGET_STF_1) $(TARGET_OPT_1) $(TARGET_SAV_1) $(TARGET_FMV_1) $(TARGET_M00_1) $(TARGET_M01_1) $(TARGET_M02_1) $(TARGET_M10_1) $(TARGET_M11_1) $(TARGET_M12_1) $(TARGET_M13_1) $(TARGET_M14_1) $(TARGET_M15_1) $(TARGET_M16_1) $(TARGET_M20_1) $(TARGET_M21_1) $(TARGET_M22_1) $(TARGET_M23_1) $(TARGET_M24_1) $(TARGET_M30_1) $(TARGET_M31_1) $(TARGET_M32_1) $(TARGET_M33_1) $(TARGET_M34_1) $(TARGET_M35_1) $(TARGET_M36_1) $(TARGET_M40_1) $(TARGET_M41_1) $(TARGET_M42_1) $(TARGET_M43_1) $(TARGET_M44_1) $(TARGET_M45_1) $(TARGET_M46_1) $(TARGET_M50_1) $(TARGET_M51_1) $(TARGET_M52_1) $(TARGET_M53_1) $(TARGET_M60_1) $(TARGET_M61_1) $(TARGET_M62_1) $(TARGET_M63_1) $(TARGET_M64_1) $(TARGET_M65_1) $(TARGET_M70_1) $(TARGET_M71_1) $(TARGET_M72_1) $(TARGET_M73_1) $(TARGET_MAP_1) $(TARGET_ITS_1) $(TARGET_ITL_1) $(TARGET_ITF_1) $(TARGET_ITG_1) $(TARGET_ITE_1)

endif

# Source Definitions

ifneq ($(OBJDIFF_FIX),1)
TARGET_OUT_CACHE := $(foreach target,$(TARGET_IN),$(target)|$(call get_target_out_slow,$(target)))
endif

TARGET_OUT := $(foreach target,$(TARGET_IN),$(call get_target_out,$(target)))

CONFIG_FILES := $(foreach target,$(TARGET_IN),$(call get_yaml_path,$(target)))
LD_FILES     := $(addsuffix .ld,$(addprefix $(LINKER_DIR)/,$(TARGET_IN)))

# Recursively include any .d dependency files from previous builds.
# Allowing Make to rebuild targets when any included headers/sources change.
-include $(shell [ -d $(BUILD_DIR) ] && find $(BUILD_DIR) -name '*.d' || true)

# Rules

# Rules - Build (Normal/Matching)

default: build

build: $(TARGET_OUT) checksum

reset-build:
	rm -rf $(BUILD_DIR)/src
	rm -rf $(OUT_DIR)
	$(MAKE) build $(TARGET_OUT) checksum

clean-build: reset
	$(MAKE) generate
	$(MAKE) build

generate: $(LD_FILES)

# Rules - Build (Objdiff/Progress)

objdiff-config:
	rm -rf $(EXPECTED_DIR)
	mkdir -p $(EXPECTED_DIR)
	$(MAKE) GAME_VERSION=$(GAME_VERSION) objdiff-generate
	$(PYTHON) $(OBJDIFF_DIR)/objdiff_generate.py $(OBJDIFF_DIR)/config-retail.yaml $(GAME_VERSION_DIR)

objdiff-config-all:
	rm -rf $(EXPECTED_DIR)
	mkdir -p $(EXPECTED_DIR)
	$(MAKE) GAME_VERSION=USA objdiff-generate
	$(MAKE) GAME_VERSION=EUR objdiff-generate
	$(MAKE) GAME_VERSION=JAP0 objdiff-generate
	$(MAKE) GAME_VERSION=JAP1 objdiff-generate
	$(MAKE) GAME_VERSION=JAP2 objdiff-generate
	$(PYTHON) $(OBJDIFF_DIR)/objdiff_generate.py $(OBJDIFF_DIR)/config-retail.yaml ALL

objdiff-generate:
	$(MAKE) GAME_VERSION=$(GAME_VERSION) GEN_COMP_TU=1 regenerate
# Hide jtbl symbols from the splat asm files, to improve data match % (see py script header)
	$(PYTHON) $(TOOLS_DIR)/hide_jtbl_syms.py $(ASM_DIR)
	$(MAKE) GAME_VERSION=$(GAME_VERSION) NON_MATCHING=1 SKIP_ASM=1 GEN_COMP_TU=1 build
	mv $(BUILD_DIR)/$(ASM_DIR) $(EXPECTED_DIR)

report:
	$(MAKE) BUILD_EXE=1 BUILD_ENGINE=1 BUILD_SCREENS=1 BUILD_MAPS=1 objdiff-config
	$(OBJDIFF) report generate > $(BUILD_DIR)/progress.json

# Progress report comparing the matching build against itself: any code/data still
# included as ASM carries a .NON_MATCHING marker which objdiff counts as unmatched.
# `build` fails on a checksum mismatch, which keeps unmatched C data from counting.
SELF_REPORT_DIR := $(BUILD_DIR)/objdiff_self

report-self:
	$(MAKE) GAME_VERSION=$(GAME_VERSION) regenerate
	$(MAKE) GAME_VERSION=$(GAME_VERSION) build
	mkdir -p $(SELF_REPORT_DIR)
	$(PYTHON) $(OBJDIFF_DIR)/objdiff_generate.py $(OBJDIFF_DIR)/config-retail.yaml $(GAME_VERSION_DIR) \
		--self-compare --include-raw-splits --output $(SELF_REPORT_DIR)/objdiff.json
	$(OBJDIFF) report generate -p $(SELF_REPORT_DIR) > $(BUILD_DIR)/progress.json

# Falls back to the target-vs-base report when the build does not match, so a
# commit that breaks matching still produces a report.
report-ci:
	$(MAKE) GAME_VERSION=$(GAME_VERSION) report-self || $(MAKE) GAME_VERSION=$(GAME_VERSION) report

progress:
	$(MAKE) NON_MATCHING=1 SKIP_ASM=1 build

reset-progress:
	rm -rf $(BUILD_DIR)/src
	rm -rf $(OUT_DIR)
	$(MAKE) NON_MATCHING=1 SKIP_ASM=1 build $(TARGET_OUT)

# Rules - Rom handling (Extraction/Insertion)
# TODO: Allow to insert any files to the game files
# At the moment only overlays have been handled

setup: reset-root
	rm -rf $(EXPECTED_DIR)
	$(MAKE) extract
	$(MAKE) generate

insert-ovl:
	$(INSERT_OVLS) $(INSERT_OVLS_FLAGS)
	$(MKPSXISO) $(MKPSXISO_FLAGS)

extract:
	rm -rf "$(ASSETS_DIR)/$(GAME_VERSION)"
	$(DUMPSXISO) $(DUMPSXISO_FLAGS)
	$(SILENT_ASSETS) $(SILENT_ASSETS_FLAGS)

# Rules - Cleaning

clean-rom:
	find $(ROM_DIR) -maxdepth 1 -type f -delete
	find $(ROM_DIR)/$(GAME_VERSION_DIR) -maxdepth 1 -type f -delete

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(PERMUTER_DIR)

reset: clean
	rm -rf $(ASM_DIR)
	rm -rf $(LINKER_DIR)

clean-root:
	rm -rf $(BUILD_DIR)
	rm -rf $(PERMUTER_DIR)

reset-root: clean-root
	rm -rf $(ASM_DIR)
	rm -rf $(LINKER_DIR)

# Rules - Misc.

regenerate: reset
	$(MAKE) generate

compilation-test:
	$(COMPTEST)

checksum: $(TARGET_OUT)
ifeq ($(CHECKSUM),1)
ifeq ($(SKIP_ASM),0)
	@sha256sum --ignore-missing --check "$(CONFIG_DIR)/checksum.sha"
endif
endif

config-formatter:
	$(PYTHON) $(TOOLS_DIR)/configs_formatter.py

# Recipes

ifeq ($(OBJDIFF_FIX),1)
$2: $(info $(patsubst $(BUILD_DIR)/%.c.o,%.c,$2))
else
# .elf targets
# Generate .elf target for each target from TARGET_IN.
$(foreach target,$(TARGET_IN),$(eval $(call make_elf_target,$(target),$(call get_target_out,$(target)))))
endif

# Generate objects.
# (Running make with MAKE_COMPILE_LOG=1 will create a compile.log that can be passed to tools/create_compile_commands.py)

$(BUILD_DIR)/%.i: %.c
	@mkdir -p $(dir $@)
	$(call FlagsSwitch, $@)
ifeq ($(MAKE_COMPILE_LOG),1)
	@echo "$(CPP) -P -MMD -MP -MT $@ -MF $@.d $(CPP_FLAGS) $(OVL_FLAGS) -o $@ $<" >> compile.log
endif
	$(CPP) -P -MMD -MP -MT $@ -MF $@.d $(CPP_FLAGS) $(OVL_FLAGS) -o $@ $<

# PAL map overlays in languages other than English are built from the English map's source,
# into build/<version>/lang/<language>/ (the `build_path` of their yaml).
PAL_LANGUAGES := ger frn spn itl

define make_lang_rules
$(BUILD_DIR)/lang/$(1)/%.i: %.c
	@mkdir -p $$(dir $$@)
	$$(call FlagsSwitch, $$@)
	$$(CPP) -P -MMD -MP -MT $$@ -MF $$@.d $$(CPP_FLAGS) $$(OVL_FLAGS) -o $$@ $$<

$(BUILD_DIR)/lang/$(1)/%.s.o: %.s
	@mkdir -p $$(dir $$@)
	$$(call FlagsSwitch, $$@)
	$$(AS) $$(AS_FLAGS) $$(if $$(filter 1,$$(GEN_COMP_TU)),--defsym JLABEL_LOCAL=1) -o $$@ $$<
endef

$(foreach lang,$(PAL_LANGUAGES),$(eval $(call make_lang_rules,$(lang))))

$(BUILD_DIR)/%.sjis.i: $(BUILD_DIR)/%.i
	$(PYTHON) $(TOOLS_DIR)/iconv_sjis_wrapper.py -f UTF-8 -t $(if $(findstring /src/maps/,$@),$(MAP_TEXT_ENCODING),$(if $(findstring item_screens_2,$@),$(MAP_TEXT_ENCODING),SHIFT-JIS)) $< -o $@

# Switch compiler to 2.7.2-cdk / 2.7.2-970404 for `libkpad` code.
$(BUILD_DIR)/src/bodyprog/libkpad/%.c.s: CC := $(CC272)

$(BUILD_DIR)/%.c.s: $(BUILD_DIR)/%.sjis.i
	@mkdir -p $(dir $@)
	$(call FlagsSwitch, $@)
	$(CC) $(CC_FLAGS) -o $@ $<

$(BUILD_DIR)/%.c.o: $(BUILD_DIR)/%.c.s
	@mkdir -p $(dir $@)
	$(call FlagsSwitch, $@)
	$(MASPSX) $(MASPSX_FLAGS) -o $@ $<

ifneq ($(OBJDIFF_FIX),1)

# jlabel is global unless building objdiff expected objects (GEN_COMP_TU), which keep jumptable labels local.
$(BUILD_DIR)/%.s.o: %.s
	@mkdir -p $(dir $@)
	$(call FlagsSwitch, $@)
	$(AS) $(AS_FLAGS) $(if $(filter 1,$(GEN_COMP_TU)),--defsym JLABEL_LOCAL=1) -o $@ $<

$(BUILD_DIR)/%.bin.o: %.bin
	@mkdir -p $(dir $@)
	$(LD) $(LD_FLAGS) -r -b binary -o $@ $<

# Split .yaml.
$(LINKER_DIR)/%.ld: $(CONFIG_DIR)/%.yaml
	@mkdir -p $(dir $@)
	$(SPLAT) $(SPLAT_FLAGS) $<

endif

### Settings
.SECONDARY:
.PHONY: clean default build
SHELL = /bin/bash -e -o pipefail
