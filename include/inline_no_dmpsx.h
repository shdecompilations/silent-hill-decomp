#ifndef _INLINENODMPSX_H_
#define _INLINENODMPSX_H_

#include "inline_c.h"

/* Replace macros originally defined for dmpsx. */

#undef gte_rtps
#define gte_rtps() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4A180001")

#undef gte_rtpt
#define gte_rtpt() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4A280030")

#undef gte_rt
#define gte_rt() __asm__ volatile( \
    "nop;"                         \
    "nop;"                         \
    ".word 0x4A480012")

#undef gte_rtv0
#define gte_rtv0() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4A486012")

#undef gte_rtv1
#define gte_rtv1() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4A48E012")

#undef gte_rtv2
#define gte_rtv2() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4A496012")

#undef gte_rtir
#define gte_rtir() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4A49E012")

#undef gte_rtir_sf0
#define gte_rtir_sf0() __asm__ volatile( \
    "nop;"                               \
    "nop;"                               \
    ".word 0x4A41E012")

#undef gte_rtv0tr
#define gte_rtv0tr() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A480012")

#undef gte_rtv1tr
#define gte_rtv1tr() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A488012")

#undef gte_rtv2tr
#define gte_rtv2tr() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A490012")

#undef gte_rtirtr
#define gte_rtirtr() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A498012")

#undef gte_rtv0bk
#define gte_rtv0bk() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A482012")

#undef gte_rtv1bk
#define gte_rtv1bk() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A48A012")

#undef gte_rtv2bk
#define gte_rtv2bk() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A492012")

#undef gte_rtirbk
#define gte_rtirbk() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A49A012")

#undef gte_ll
#define gte_ll() __asm__ volatile( \
    "nop;"                         \
    "nop;"                         \
    ".word 0x4A4A6412")

#undef gte_llv0
#define gte_llv0() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4A4A6012")

#undef gte_llv1
#define gte_llv1() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4A4AE012")

#undef gte_llv2
#define gte_llv2() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4A4B6012")

#undef gte_llir
#define gte_llir() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4A4BE012")

#undef gte_llv0tr
#define gte_llv0tr() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A4A0012")

#undef gte_llv1tr
#define gte_llv1tr() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A4A8012")

#undef gte_llv2tr
#define gte_llv2tr() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A4B0012")

#undef gte_llirtr
#define gte_llirtr() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A4B8012")

#undef gte_llv0bk
#define gte_llv0bk() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A4A2012")

#undef gte_llv1bk
#define gte_llv1bk() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A4AA012")

#undef gte_llv2bk
#define gte_llv2bk() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A4B2012")

#undef gte_llirbk
#define gte_llirbk() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A4BA012")

#undef gte_lc
#define gte_lc() __asm__ volatile( \
    "nop;"                         \
    "nop;"                         \
    ".word 0x4A4DA412")

#undef gte_lcv0
#define gte_lcv0() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4A4C6012")

#undef gte_lcv1
#define gte_lcv1() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4A4CE012")

#undef gte_lcv2
#define gte_lcv2() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4A4D6012")

#undef gte_lcir
#define gte_lcir() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4A4DE012")

#undef gte_lcv0tr
#define gte_lcv0tr() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A4C0012")

#undef gte_lcv1tr
#define gte_lcv1tr() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A4C8012")

#undef gte_lcv2tr
#define gte_lcv2tr() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A4D0012")

#undef gte_lcirtr
#define gte_lcirtr() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A4D8012")

#undef gte_lcv0bk
#define gte_lcv0bk() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A4C2012")

#undef gte_lcv1bk
#define gte_lcv1bk() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A4CA012")

#undef gte_lcv2bk
#define gte_lcv2bk() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A4D2012")

#undef gte_lcirbk
#define gte_lcirbk() __asm__ volatile( \
    "nop;"                             \
    "nop;"                             \
    ".word 0x4A4DA012")

#undef gte_dpcl
#define gte_dpcl() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4A680029")

#undef gte_dpcs
#define gte_dpcs() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4A780010")

#undef gte_dpct
#define gte_dpct() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4AF8002A")

#undef gte_intpl
#define gte_intpl() __asm__ volatile( \
    "nop;"                            \
    "nop;"                            \
    ".word 0x4A980011")

#undef gte_sqr12
#define gte_sqr12() __asm__ volatile( \
    "nop;"                            \
    "nop;"                            \
    ".word 0x4AA80428")

#undef gte_sqr0
#define gte_sqr0() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4AA00428")

#undef gte_ncs
#define gte_ncs() __asm__ volatile( \
    "nop;"                          \
    "nop;"                          \
    ".word 0x4AC8041E")

#undef gte_nct
#define gte_nct() __asm__ volatile( \
    "nop;"                          \
    "nop;"                          \
    ".word 0x4AD80420")

#undef gte_ncds
#define gte_ncds() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4AE80413")

#undef gte_ncdt
#define gte_ncdt() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4AF80416")

#undef gte_nccs
#define gte_nccs() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4B08041B")

#undef gte_ncct
#define gte_ncct() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4B18043F")

#undef gte_cdp
#define gte_cdp() __asm__ volatile( \
    "nop;"                          \
    "nop;"                          \
    ".word 0x4B280414")

#undef gte_cc
#define gte_cc() __asm__ volatile( \
    "nop;"                         \
    "nop;"                         \
    ".word 0x4B38041C")

#undef gte_nclip
#define gte_nclip() __asm__ volatile( \
    "nop;"                            \
    "nop;"                            \
    ".word 0x4B400006")

#undef gte_avsz3
#define gte_avsz3() __asm__ volatile( \
    "nop;"                            \
    "nop;"                            \
    ".word 0x4B58002D")

#undef gte_avsz4
#define gte_avsz4() __asm__ volatile( \
    "nop;"                            \
    "nop;"                            \
    ".word 0x4B68002E")

#undef gte_op12
#define gte_op12() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4B78000C")

#undef gte_op0
#define gte_op0() __asm__ volatile( \
    "nop;"                          \
    "nop;"                          \
    ".word 0x4B70000C")

#undef gte_gpf12
#define gte_gpf12() __asm__ volatile( \
    "nop;"                            \
    "nop;"                            \
    ".word 0x4B98003D")

#undef gte_gpf0
#define gte_gpf0() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4B90003D")

#undef gte_gpl12
#define gte_gpl12() __asm__ volatile( \
    "nop;"                            \
    "nop;"                            \
    ".word 0x4BA8003E")

#undef gte_gpl0
#define gte_gpl0() __asm__ volatile( \
    "nop;"                           \
    "nop;"                           \
    ".word 0x4BA0003E")

#undef gte_mvmva
#define gte_mvmva(sf, mx, v, cv, lm) \
    gte_mvmva_core(0x4A400012 | ((sf) << 25) | ((mx) << 23) | ((v) << 21) | ((cv) << 19) | ((lm) << 18))

#endif
