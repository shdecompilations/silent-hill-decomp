.include "macro.inc"

.set noreorder

.section .text, "ax"

# psyq-obj-parser had issues with odd relocations inside snmain.obj.
# Instead, a disassembled version is used here to help future builds 
# where splitting the original EXE first isn't needed.

# @hack Override `main_BSS_END` with the same BSS end value
# that the original code would have used (snmain.s used `sectend(.bss)`).
# The existing main_BSS_END should be used, but the BSS
# section ends earlier than what the values used here show.
# Likely the main EXE was truncated by devs after building?
.set main_BSS_END, g_Bodyprog
.set main_BSS_SIZE, 0x1EC8

glabel __SN_ENTRY_POINT
    /* 377C 80012F7C 0280023C */  la         $v0, main_SBSS_START
    /* 3784 80012F84 0280033C */  la         $v1, main_BSS_END
clrit:
    .set noat
    /* 378C 80012F8C 000040AC */  sw         $zero, 0x0($v0)
    /* 3790 80012F90 04004224 */  addiu      $v0, $v0, 0x4
    /* 3794 80012F94 2B084300 */  sltu       $at, $v0, $v1
    /* 3798 80012F98 FCFF2014 */  bnez       $at, clrit
    /* 379C 80012F9C 00000000 */   nop
    .set at

    /* 37A0 80012FA0 0280023C */  lw        $v0, _ramsize
    /* 37A8 80012FA8 00000000 */  nop

    /* 37AC 80012FAC F8FF4220 */  sub        $v0, $v0, 0x8
    /* 37B0 80012FB0 0080083C */  lui        $t0, 0x8000   # (mem seg for kernel cached RAM)
    /* 37B4 80012FB4 25E84800 */  or         $sp, $v0, $t0 # set stack in kseg0

    /* 37B8 80012FB8 0280043C */  la         $a0, main_BSS_END # a0 = heap base
    /* 37C0 80012FC0 C0200400 */  sll        $a0, $a0, 3
    /* 37C4 80012FC4 C2200400 */  srl        $a0, $a0, 3 # remove mem seg bits
    /* 37C8 80012FC8 0280033C */  lw         $v1, _stacksize
    /* 37D0 80012FD0 00000000 */  nop
    /* 37D4 80012FD4 23284300 */  subu       $a1, $v0, $v1 # calc a1 = top of heap
    /* 37D8 80012FD8 2328A400 */  subu       $a1, $a1, $a0 # -heap base, => a1 = size of heap
    /* 37DC 80012FDC 0280013C */  sw         $a1, __heapsize
    /* 37E4 80012FE4 25208800 */  or         $a0, $a0, $t0 # heap in kseg0
    /* 37E8 80012FE8 0280013C */  sw         $a0, __heapbase
    
    .set noat
    /* 37F0 80012FF0 0280013C */  lui        $at, %hi(__ra_temp)  # @hack this should be `sw $ra, __ra_temp` which would use $at but doesn't want to with GNU as?
    /* 37F4 80012FF4 942C3FAC */  sw         $ra, %lo(__ra_temp)($at)
    .set at
    /* 37F8 80012FF8 02801C3C */  la         $gp, main_SDATA_START
    /* 3800 80013000 21F0A003 */  addu       $fp, $sp, $zero
    /* 3804 80013004 3F4C000C */  jal        InitHeap
    /* 3808 80013008 04008420 */   addi      $a0, $a0, 4
    
    /* 380C 8001300C 02801F3C */  lui        $ra, %hi(__ra_temp)
    /* 3810 80013010 942CFF8F */  lw         $ra, %lo(__ra_temp)($ra)
    /* 3814 80013014 00000000 */  nop

    /* 3818 80013018 DC41000C */  jal        main
    /* 381C 8001301C 00000000 */   nop

    /* 3820 80013020 4D000000 */  break      0, 1
endlabel __SN_ENTRY_POINT

glabel __main
    /* 3824 80013024 0280083C */  lw         $t0, __initialised

    /* 382C 8001302C F0FFBD27 */  addiu      $sp, $sp, -16

    /* 3830 80013030 0400B0AF */  sw         $s0, 4($sp)
    /* 3834 80013034 0800B1AF */  sw         $s1, 8($sp)
    /* 3838 80013038 0C00BFAF */  sw         $ra, 12($sp)

    /* 383C 8001303C 0F000015 */  bnez       $t0, exit
    /* 3840 80013040 01000834 */   ori       $t0, $zero, 0x1

    /* 3844 80013044 0280013C */  sw         $t0, __initialised

    /* 384C 8001304C 0180103C */  la         $s0, main_RODATA_START
    /* 3854 80013054 0000113C */  lui        $s1, (0x0 >> 16)
    /* 3858 80013058 00003126 */  addiu      $s1, $s1, 0x0
    /* 385C 8001305C 07002012 */  beqz       $s1, exit
    /* 3860 80013060 00000000 */   nop

loop: #loop for all C++ global constructors
    /* 3864 80013064 0000088E */  lw         $t0, 0x0($s0)
    /* 3868 80013068 04001026 */  addiu      $s0, $s0, 0x4

    /* 386C 8001306C 09F80001 */  jalr       $t0 # call C++ constructor
    /* 3870 80013070 FFFF3126 */   addiu     $s1, $s1, -0x1

    /* 3874 80013074 FBFF2016 */  bnez       $s1, loop
    /* 3878 80013078 00000000 */   nop

exit:
    /* 387C 8001307C 0C00BF8F */  lw         $ra, 12($sp)
    /* 3880 80013080 0800B18F */  lw         $s1, 8($sp)
    /* 3884 80013084 0400B08F */  lw         $s0, 4($sp)

    /* 3888 80013088 1000BD27 */  addiu      $sp, $sp, 16

    /* 388C 8001308C 0800E003 */  jr         $ra
    /* 3890 80013090 00000000 */   nop
endlabel __main

glabel __do_global_dtors

    /* 3894 80013094 0280083C */  lw         $t0, __initialised

    /* 389C 8001309C F0FFBD27 */  addiu      $sp, $sp, -16

    /* 38A0 800130A0 0400B0AF */  sw         $s0, 4($sp)
    /* 38A4 800130A4 0800B1AF */  sw         $s1, 8($sp)
    /* 38A8 800130A8 0C00BFAF */  sw         $ra, 12($sp)

    /* 38AC 800130AC 0D000011 */  beqz       $t0, exit2
    /* 38B0 800130B0 00000000 */   nop

    /* 38B4 800130B4 0180103C */  la         $s0, main_RODATA_START
    /* 38BC 800130BC 0000113C */  lui        $s1, (0x0 >> 16)
    /* 38C0 800130C0 00003126 */  addiu      $s1, $s1, 0x0
    /* 38C4 800130C4 07002012 */  beqz       $s1, exit2
    /* 38C8 800130C8 00000000 */   nop
    
loop2:
    /* 38CC 800130CC 0000088E */  lw         $t0, 0x0($s0)
    /* 38D0 800130D0 04001026 */  addiu      $s0, $s0, 0x4

    /* 38D4 800130D4 09F80001 */  jalr       $t0
    /* 38D8 800130D8 FFFF3126 */   addiu     $s1, $s1, -1

    /* 38DC 800130DC FBFF2016 */  bnez       $s1, loop2
    /* 38E0 800130E0 00000000 */   nop

exit2:
    /* 38E4 800130E4 0C00BF8F */  lw         $ra, 12($sp)
    /* 38E8 800130E8 0800B18F */  lw         $s1, 8($sp)
    /* 38EC 800130EC 0400B08F */  lw         $s0, 4($sp)

    /* 38F0 800130F0 1000BD27 */  addiu      $sp, $sp, 16

    /* 38F4 800130F4 0800E003 */  jr         $ra
    /* 38F8 800130F8 00000000 */   nop
endlabel __do_global_dtors

.section .data, "wa"

dlabel __initialised
    /* 11C6C 8002146C 00000000 */ .word 0x00000000
enddlabel __initialised

dlabel __heapbase
    /* 11C70 80021470 00000000 */ .word 0x00000000
enddlabel __heapbase

dlabel __heapsize
    /* 11C74 80021474 00000000 */ .word 0x00000000
enddlabel __heapsize

dlabel __text
    /* 11C78 80021478 70070180 */ .word main_TEXT_START
enddlabel __text

dlabel __textlen
    /* 11C7C 8002147C ACA90000 */ .word main_TEXT_SIZE
enddlabel __textlen

dlabel __data
    /* 11C80 80021480 1CB10180 */ .word main_DATA_START
enddlabel __data

dlabel __datalen
    /* 11C84 80021484 947A0000 */ .word main_DATA_SIZE
enddlabel __datalen

dlabel __bss
    /* 11C88 80021488 982C0280 */ .word main_BSS_START
enddlabel __bss

dlabel __bsslen
    /* 11C8C 8002148C C81E0000 */ .word main_BSS_SIZE
enddlabel __bsslen

.section .sbss, "wa"

dlabel __ra_temp
    /* 13494 80022C94 00000000 */ .word 0x00000000
enddlabel __ra_temp
