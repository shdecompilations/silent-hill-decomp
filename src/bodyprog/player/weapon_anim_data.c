#include "game.h"
#include "inline_no_dmpsx.h"

#include <psyq/gtemac.h>
#include <psyq/libapi.h>
#include <psyq/strings.h>

#include "bodyprog/bodyprog.h"
#include "bodyprog/collision/collision.h"
#include "bodyprog/math/math.h"
#include "bodyprog/item_screens.h"
#include "bodyprog/player/player.h"
#include "bodyprog/screen/data.h"
#include "bodyprog/screen/draw.h"
#include "bodyprog/sound/sound_system.h"
#include "main/rng.h"

/** @brief Used to overwrite `HARRY_BASE_ANIM_INFOS[56:76]` with weapon-specific animations.
 * Always copies 20 `s_AnimInfo`s, but most weapons use fewwer than that.
 *
 * @bug `EquippedWeaponId_HyperBlaster` will copy past the end of this array?
 */
const s_AnimInfo HARRY_WEAPON_ANIM_INFOS[] = {
    /* `EquippedWeaponId_Axe` */
    /* 0   */ { Anim_BlendLinear, ANIM_STATUS(28, false), false, ANIM_STATUS(28, true), { Q12(100.0f) }, NO_VALUE, 568 },
    /* 1   */ { Anim_PlayOnce,    ANIM_STATUS(28, true),  false, ANIM_STATUS(28, true), { Q12(25.0f)  }, 568, 577 },
    /* 2   */ { Anim_BlendLinear, ANIM_STATUS(29, false), false, ANIM_STATUS(29, true), { Q12(100.0f) }, NO_VALUE, 579 },
    /* 3   */ { Anim_PlayOnce,    ANIM_STATUS(29, true),  false, ANIM_STATUS(29, true), { Q12(20.0f)  }, 579, 598 },
    /* 4   */ { Anim_BlendLinear, ANIM_STATUS(30, false), false, ANIM_STATUS(30, true), { Q12(100.0f) }, NO_VALUE, 599 },
    /* 5   */ { Anim_PlayOnce,    ANIM_STATUS(30, true),  false, ANIM_STATUS(30, true), { Q12(18.0f)  }, 599, 615 },
    /* 6   */ { Anim_BlendLinear, ANIM_STATUS(31, false), false, ANIM_STATUS(31, true), { Q12(100.0f) }, NO_VALUE, 616 },
    /* 7   */ { Anim_PlayOnce,    ANIM_STATUS(31, true),  false, ANIM_STATUS(31, true), { Q12(20.0f)  }, 616, 635 },
    /* 8   */ { Anim_BlendLinear, ANIM_STATUS(32, false), false, ANIM_STATUS(32, true), { Q12(100.0f) }, NO_VALUE, 577 },
    /* 9   */ { Anim_PlayOnce,    ANIM_STATUS(32, true),  false, ANIM_STATUS(32, true), { Q12(-35.0f) }, 568, 577 },

    /* `EquippedWeaponId_Hammer` */
    /* 10  */ { Anim_BlendLinear, ANIM_STATUS(28, false), false, ANIM_STATUS(28, true), { Q12(100.0f) }, NO_VALUE, 568 },
    /* 11  */ { Anim_PlayOnce,    ANIM_STATUS(28, true),  false, ANIM_STATUS(28, true), { Q12(40.0f)  }, 568, 579 },
    /* 12  */ { Anim_BlendLinear, ANIM_STATUS(29, false), false, ANIM_STATUS(29, true), { Q12(100.0f) }, NO_VALUE, 584 },
    /* 13  */ { Anim_PlayOnce,    ANIM_STATUS(29, true),  false, ANIM_STATUS(29, true), { Q12(22.0f)  }, 584, 613 },
    /* 14  */ { Anim_BlendLinear, ANIM_STATUS(30, false), false, ANIM_STATUS(30, true), { Q12(100.0f) }, NO_VALUE, 614 },
    /* 15  */ { Anim_PlayOnce,    ANIM_STATUS(30, true),  false, ANIM_STATUS(30, true), { Q12(16.0f)  }, 614, 634 },
    /* 16  */ { Anim_BlendLinear, ANIM_STATUS(31, false), false, ANIM_STATUS(31, true), { Q12(100.0f) }, NO_VALUE, 637 },
    /* 17  */ { Anim_PlayOnce,    ANIM_STATUS(31, true),  false, ANIM_STATUS(31, true), { Q12(18.0f)  }, 637, 659 },
    /* 18  */ { Anim_BlendLinear, ANIM_STATUS(32, false), false, ANIM_STATUS(32, true), { Q12(100.0f) }, NO_VALUE, 579 },
    /* 19  */ { Anim_PlayOnce,    ANIM_STATUS(32, true),  false, ANIM_STATUS(32, true), { Q12(-35.0f) }, 568, 579 },

    /* `EquippedWeaponId_SteelPipe` */
    /* 20  */ { Anim_BlendLinear, ANIM_STATUS(28, false), false, ANIM_STATUS(28, true), { Q12(100.0f) }, NO_VALUE, 568 },
    /* 21  */ { Anim_PlayOnce,    ANIM_STATUS(28, true),  false, ANIM_STATUS(28, true), { Q12(45.0f)  }, 568, 579 },
    /* 22  */ { Anim_BlendLinear, ANIM_STATUS(29, false), false, ANIM_STATUS(29, true), { Q12(100.0f) }, NO_VALUE, 584 },
    /* 23  */ { Anim_PlayOnce,    ANIM_STATUS(29, true),  false, ANIM_STATUS(29, true), { Q12(25.0f)  }, 584, 613 },
    /* 24  */ { Anim_BlendLinear, ANIM_STATUS(30, false), false, ANIM_STATUS(30, true), { Q12(100.0f) }, NO_VALUE, 614 },
    /* 25  */ { Anim_PlayOnce,    ANIM_STATUS(30, true),  false, ANIM_STATUS(30, true), { Q12(16.0f)  }, 614, 634 },
    /* 26  */ { Anim_BlendLinear, ANIM_STATUS(31, false), false, ANIM_STATUS(31, true), { Q12(100.0f) }, NO_VALUE, 637 },
    /* 27  */ { Anim_PlayOnce,    ANIM_STATUS(31, true),  false, ANIM_STATUS(31, true), { Q12(20.0f)  }, 637, 659 },
    /* 28  */ { Anim_BlendLinear, ANIM_STATUS(32, false), false, ANIM_STATUS(32, true), { Q12(100.0f) }, NO_VALUE, 579 },
    /* 29  */ { Anim_PlayOnce,    ANIM_STATUS(32, true),  false, ANIM_STATUS(32, true), { Q12(-40.0f) }, 568, 579 },

    /* `EquippedWeaponId_KitchenKnife` */
    /* 30  */ { Anim_BlendLinear, ANIM_STATUS(28, false), false, ANIM_STATUS(28, true), { Q12(100.0f) }, NO_VALUE, 568 },
    /* 31  */ { Anim_PlayOnce,    ANIM_STATUS(28, true),  false, ANIM_STATUS(28, true), { Q12(30.0f)  }, 568, 575 },
    /* 32  */ { Anim_BlendLinear, ANIM_STATUS(29, false), false, ANIM_STATUS(29, true), { Q12(100.0f) }, NO_VALUE, 581 },
    /* 33  */ { Anim_PlayOnce,    ANIM_STATUS(29, true),  false, ANIM_STATUS(29, true), { Q12(20.0f)  }, 581, 595 },
    /* 34  */ { Anim_BlendLinear, ANIM_STATUS(30, false), false, ANIM_STATUS(30, true), { Q12(100.0f) }, NO_VALUE, 596 },
    /* 35  */ { Anim_PlayOnce,    ANIM_STATUS(30, true),  false, ANIM_STATUS(30, true), { Q12(16.0f)  }, 596, 611 },
    /* 36  */ { Anim_BlendLinear, ANIM_STATUS(31, false), false, ANIM_STATUS(31, true), { Q12(100.0f) }, NO_VALUE, 613 },
    /* 37  */ { Anim_PlayOnce,    ANIM_STATUS(31, true),  false, ANIM_STATUS(31, true), { Q12(16.0f)  }, 613, 629 },
    /* 38  */ { Anim_BlendLinear, ANIM_STATUS(32, false), false, ANIM_STATUS(32, true), { Q12(100.0f) }, NO_VALUE, 577 },
    /* 39  */ { Anim_PlayOnce,    ANIM_STATUS(32, true),  false, ANIM_STATUS(32, true), { Q12(-40.0f) }, 568, 577 },

    /* `EquippedWeaponId_Katana` */
    /* 40  */ { Anim_BlendLinear, ANIM_STATUS(28, false), false, ANIM_STATUS(28, true), { Q12(100.0f) }, NO_VALUE, 568 },
    /* 41  */ { Anim_PlayOnce,    ANIM_STATUS(28, true),  false, ANIM_STATUS(28, true), { Q12(30.0f) }, 568, 579 },
    /* 42  */ { Anim_BlendLinear, ANIM_STATUS(29, false), false, ANIM_STATUS(29, true), { Q12(100.0f) }, NO_VALUE, 580 },
    /* 43  */ { Anim_PlayOnce,    ANIM_STATUS(29, true),  false, ANIM_STATUS(29, true), { Q12(25.0f) }, 580, 597 },
    /* 44  */ { Anim_BlendLinear, ANIM_STATUS(30, false), false, ANIM_STATUS(30, true), { Q12(100.0f) }, NO_VALUE, 598 },
    /* 45  */ { Anim_PlayOnce,    ANIM_STATUS(30, true),  false, ANIM_STATUS(30, true), { Q12(16.0f) }, 598, 611 },
    /* 46  */ { Anim_BlendLinear, ANIM_STATUS(31, false), false, ANIM_STATUS(31, true), { Q12(100.0f) }, NO_VALUE, 615 },
    /* 47  */ { Anim_PlayOnce,    ANIM_STATUS(31, true),  false, ANIM_STATUS(31, true), { Q12(16.0f) }, 615, 625 },
    /* 48  */ { Anim_BlendLinear, ANIM_STATUS(32, false), false, ANIM_STATUS(32, true), { Q12(100.0f) }, NO_VALUE, 579 },
    /* 49  */ { Anim_PlayOnce,    ANIM_STATUS(32, true),  false, ANIM_STATUS(32, true), { Q12(-35.0f) }, 568, 579 },

    /* `EquippedWeaponId_Chainsaw` */
    /* 50  */ { Anim_BlendLinear, ANIM_STATUS(28, false), false, ANIM_STATUS(28, true), { Q12(100.0f) }, NO_VALUE, 568 },
    /* 51  */ { Anim_PlayOnce,    ANIM_STATUS(28, true),  false, ANIM_STATUS(28, true), { Q12(15.0f)  }, 568, 583 },
    /* 52  */ { Anim_BlendLinear, ANIM_STATUS(29, false), false, ANIM_STATUS(29, true), { Q12(100.0f) }, NO_VALUE, 584 },
    /* 53  */ { Anim_PlayOnce,    ANIM_STATUS(29, true),  false, ANIM_STATUS(29, true), { Q12(16.0f)  }, 584, 602 },
    /* 54  */ { Anim_BlendLinear, ANIM_STATUS(30, false), false, ANIM_STATUS(30, true), { Q12(100.0f) }, NO_VALUE, 603 },
    /* 55  */ { Anim_PlayOnce,    ANIM_STATUS(30, true),  false, ANIM_STATUS(30, true), { Q12(14.0f)  }, 603, 618 },
    /* 56  */ { Anim_BlendLinear, ANIM_STATUS(31, false), false, ANIM_STATUS(31, true), { Q12(100.0f) }, NO_VALUE, 619 },
    /* 57  */ { Anim_PlayOnce,    ANIM_STATUS(31, true),  false, ANIM_STATUS(31, true), { Q12(14.0f)  }, 619, 637 },
    /* 58  */ { Anim_BlendLinear, ANIM_STATUS(32, false), false, ANIM_STATUS(32, true), { Q12(100.0f) }, NO_VALUE, 649 },
    /* 59  */ { Anim_PlayOnce,    ANIM_STATUS(32, true),  false, ANIM_STATUS(32, true), { Q12(-35.0f) }, 638, 649 },
    /* 60  */ { Anim_BlendLinear, ANIM_STATUS(33, false), false, ANIM_STATUS(33, true), { Q12(100.0f) }, NO_VALUE, 638 },
    /* 61  */ { Anim_PlayOnce,    ANIM_STATUS(33, true),  false, ANIM_STATUS(33, true), { Q12(24.0f)  }, 638, 649 },
    /* 62  */ { Anim_BlendLinear, ANIM_STATUS(34, false), false, ANIM_STATUS(34, true), { Q12(100.0f) }, NO_VALUE, 650 },
    /* 63  */ { Anim_PlayLoop,    ANIM_STATUS(34, true),  false, NO_VALUE,              { Q12(20.0f)  }, 650, 655 },

    /* `EquippedWeaponId_RockDrill` */
    /* 64  */ { Anim_BlendLinear, ANIM_STATUS(28, false), false, ANIM_STATUS(28, true), { Q12(100.0f) }, NO_VALUE, 568 },
    /* 65  */ { Anim_PlayOnce,    ANIM_STATUS(28, true),  false, ANIM_STATUS(28, true), { Q12(18.0f)  }, 568, 583 },
    /* 66  */ { Anim_BlendLinear, ANIM_STATUS(29, false), false, ANIM_STATUS(29, true), { Q12(100.0f) }, NO_VALUE, 584 },
    /* 67  */ { Anim_PlayOnce,    ANIM_STATUS(29, true),  false, ANIM_STATUS(29, true), { Q12(24.0f)  }, 584, 597 },
    /* 68  */ { Anim_BlendLinear, ANIM_STATUS(30, false), false, ANIM_STATUS(30, true), { Q12(100.0f) }, NO_VALUE, 598 },
    /* 69  */ { Anim_PlayOnce,    ANIM_STATUS(30, true),  false, ANIM_STATUS(30, true), { Q12(20.0f)  }, 598, 611 },
    /* 70  */ { Anim_BlendLinear, ANIM_STATUS(31, false), false, ANIM_STATUS(31, true), { Q12(100.0f) }, NO_VALUE, 612 },
    /* 71  */ { Anim_PlayOnce,    ANIM_STATUS(31, true),  false, ANIM_STATUS(31, true), { Q12(22.0f)  }, 612, 625 },
    /* 72  */ { Anim_BlendLinear, ANIM_STATUS(32, false), false, ANIM_STATUS(32, true), { Q12(100.0f) }, NO_VALUE, 636 },
    /* 73  */ { Anim_PlayOnce,    ANIM_STATUS(32, true),  false, ANIM_STATUS(32, true), { Q12(-25.0f) }, 626, 636 },
    /* 74  */ { Anim_BlendLinear, ANIM_STATUS(33, false), false, ANIM_STATUS(33, true), { Q12(100.0f) }, NO_VALUE, 626 },
    /* 75  */ { Anim_PlayOnce,    ANIM_STATUS(33, true),  false, ANIM_STATUS(33, true), { Q12(23.0f)  }, 626, 636 },
    /* 76  */ { Anim_BlendLinear, ANIM_STATUS(34, false), false, ANIM_STATUS(34, true), { Q12(100.0f) }, NO_VALUE, 637 },
    /* 77  */ { Anim_PlayLoop,    ANIM_STATUS(34, true),  false, NO_VALUE,              { Q12(24.0f)  }, 637, 640 },

    /* `EquippedWeaponId_Handgun` */
    /* 78  */ { Anim_BlendLinear, ANIM_STATUS(28, false), false, ANIM_STATUS(28, true), { Q12(100.0f) }, NO_VALUE, 570 },
    /* 79  */ { Anim_PlayOnce,    ANIM_STATUS(28, true),  false, ANIM_STATUS(28, true), { Q12(35.0f)  }, 570, 579 },
    /* 80  */ { Anim_BlendLinear, ANIM_STATUS(29, false), false, ANIM_STATUS(29, true), { Q12(100.0f) }, NO_VALUE, 582 },
    /* 81  */ { Anim_PlayOnce,    ANIM_STATUS(29, true),  false, ANIM_STATUS(29, true), { Q12(40.0f)  }, 582, 592 },
    /* 82  */ { Anim_BlendLinear, ANIM_STATUS(30, false), false, ANIM_STATUS(30, true), { Q12(100.0f) }, NO_VALUE, 594 },
    /* 83  */ { Anim_PlayOnce,    ANIM_STATUS(30, true),  false, ANIM_STATUS(30, true), { Q12(25.0f)  }, 594, 604 },
    /* 84  */ { Anim_BlendLinear, ANIM_STATUS(31, false), false, ANIM_STATUS(31, true), { Q12(100.0f) }, NO_VALUE, 605 },
    /* 85  */ { Anim_PlayOnce,    ANIM_STATUS(31, true),  false, ANIM_STATUS(31, true), { Q12(30.0f)  }, 605, 658 },
    /* 86  */ { Anim_BlendLinear, ANIM_STATUS(32, false), false, ANIM_STATUS(32, true), { Q12(100.0f) }, NO_VALUE, 570 },
    /* 87  */ { Anim_PlayOnce,    ANIM_STATUS(32, true),  false, ANIM_STATUS(32, true), { Q12(60.0f)  }, 570, 592 },
    /* 88  */ { Anim_BlendLinear, ANIM_STATUS(33, false), false, ANIM_STATUS(33, true), { Q12(100.0f) }, NO_VALUE, 579 },
    /* 89  */ { Anim_PlayOnce,    ANIM_STATUS(33, true),  false, ANIM_STATUS(33, true), { Q12(-35.0f) }, 570, 579 },
    /* 90  */ { Anim_BlendLinear, ANIM_STATUS(34, false), false, ANIM_STATUS(34, true), { Q12(100.0f) }, NO_VALUE, 592 },
    /* 91  */ { Anim_PlayOnce,    ANIM_STATUS(34, true),  false, ANIM_STATUS(34, true), { Q12(-35.0f) }, 580, 592 },
    /* 92  */ { Anim_BlendLinear, ANIM_STATUS(35, false), false, ANIM_STATUS(35, true), { Q12(100.0f) }, NO_VALUE, 592 },
    /* 93  */ { Anim_PlayOnce,    ANIM_STATUS(35, true),  false, ANIM_STATUS(35, true), { Q12(-30.0f) }, 570, 592 },
    /* 94  */ { Anim_BlendLinear, ANIM_STATUS(36, false), false, ANIM_STATUS(36, true), { Q12(100.0f) }, NO_VALUE, 582 },
    /* 95  */ { Anim_PlayOnce,    ANIM_STATUS(36, true),  false, ANIM_STATUS(36, true), { Q12(25.0f)  }, 582, 604 },

    /* `EquippedWeaponId_HuntingRifle` */
    /* 96  */ { Anim_BlendLinear, ANIM_STATUS(28, false), false, ANIM_STATUS(28, true), { Q12(100.0f) }, NO_VALUE, 568 },
    /* 97  */ { Anim_PlayOnce,    ANIM_STATUS(28, true),  false, ANIM_STATUS(28, true), { Q12(35.0f)  }, 568, 587 },
    /* 98  */ { Anim_BlendLinear, ANIM_STATUS(29, false), false, ANIM_STATUS(29, true), { Q12(100.0f) }, NO_VALUE, 588 },
    /* 99  */ { Anim_PlayOnce,    ANIM_STATUS(29, true),  false, ANIM_STATUS(29, true), { Q12(25.0f)  }, 588, 597 },
    /* 100 */ { Anim_BlendLinear, ANIM_STATUS(30, false), false, ANIM_STATUS(30, true), { Q12(100.0f) }, NO_VALUE, 598 },
    /* 101 */ { Anim_PlayOnce,    ANIM_STATUS(30, true),  false, ANIM_STATUS(30, true), { Q12(16.0f)  }, 598, 607 },
    /* 102 */ { Anim_BlendLinear, ANIM_STATUS(31, false), false, ANIM_STATUS(31, true), { Q12(100.0f) }, NO_VALUE, 608 },
    /* 103 */ { Anim_PlayOnce,    ANIM_STATUS(31, true),  false, ANIM_STATUS(31, true), { Q12(20.0f)  }, 608, 642 },
    /* 104 */ { Anim_BlendLinear, ANIM_STATUS(32, false), false, ANIM_STATUS(32, true), { Q12(100.0f) }, NO_VALUE, 588 },
    /* 105 */ { Anim_PlayOnce,    ANIM_STATUS(32, true),  false, ANIM_STATUS(32, true), { Q12(25.0f)  }, 588, 597 },
    /* 106 */ { Anim_BlendLinear, ANIM_STATUS(33, false), false, ANIM_STATUS(33, true), { Q12(100.0f) }, NO_VALUE, 597 },
    /* 107 */ { Anim_PlayOnce,    ANIM_STATUS(33, true),  false, ANIM_STATUS(33, true), { Q12(-30.0f) }, 588, 597 },
    /* 108 */ { Anim_BlendLinear, ANIM_STATUS(34, false), false, ANIM_STATUS(34, true), { Q12(100.0f) }, NO_VALUE, 597 },
    /* 109 */ { Anim_PlayOnce,    ANIM_STATUS(34, true),  false, ANIM_STATUS(34, true), { Q12(-30.0f) }, 597, 597 },
    /* 110 */ { Anim_BlendLinear, ANIM_STATUS(35, false), false, ANIM_STATUS(35, true), { Q12(100.0f) }, NO_VALUE, 587 },
    /* 111 */ { Anim_PlayOnce,    ANIM_STATUS(35, true),  false, ANIM_STATUS(35, true), { Q12(-22.0f) }, 568, 587 },
    /* 112 */ { Anim_BlendLinear, ANIM_STATUS(36, false), false, ANIM_STATUS(36, true), { Q12(100.0f) }, NO_VALUE, 598 },
    /* 113 */ { Anim_PlayOnce,    ANIM_STATUS(36, true),  false, ANIM_STATUS(36, true), { Q12(16.0f)  }, 598, 607 },

    /* `EquippedWeaponId_Shotgun` */
    /* 114 */ { Anim_BlendLinear, ANIM_STATUS(28, false), false, ANIM_STATUS(28, true), { Q12(100.0f) }, NO_VALUE, 570 },
    /* 115 */ { Anim_PlayOnce,    ANIM_STATUS(28, true),  false, ANIM_STATUS(28, true), { Q12(40.0f)  }, 570, 579 },
    /* 116 */ { Anim_BlendLinear, ANIM_STATUS(29, false), false, ANIM_STATUS(29, true), { Q12(100.0f) }, NO_VALUE, 582 },
    /* 117 */ { Anim_PlayOnce,    ANIM_STATUS(29, true),  false, ANIM_STATUS(29, true), { Q12(30.0f)  }, 582, 592 },
    /* 118 */ { Anim_BlendLinear, ANIM_STATUS(30, false), false, ANIM_STATUS(30, true), { Q12(100.0f) }, NO_VALUE, 594 },
    /* 119 */ { Anim_PlayOnce,    ANIM_STATUS(30, true),  false, ANIM_STATUS(30, true), { Q12(22.0f)  }, 594, 604 },
    /* 120 */ { Anim_BlendLinear, ANIM_STATUS(31, false), false, ANIM_STATUS(31, true), { Q12(100.0f) }, NO_VALUE, 605 },
    /* 121 */ { Anim_PlayOnce,    ANIM_STATUS(31, true),  false, ANIM_STATUS(31, true), { Q12(17.0f)  }, 605, 641 },
    /* 122 */ { Anim_BlendLinear, ANIM_STATUS(32, false), false, ANIM_STATUS(32, true), { Q12(100.0f) }, NO_VALUE, 570 },
    /* 123 */ { Anim_PlayOnce,    ANIM_STATUS(32, true),  false, ANIM_STATUS(32, true), { Q12(60.0f)  }, 570, 592 },
    /* 124 */ { Anim_BlendLinear, ANIM_STATUS(33, false), false, ANIM_STATUS(33, true), { Q12(100.0f) }, NO_VALUE, 579 },
    /* 125 */ { Anim_PlayOnce,    ANIM_STATUS(33, true),  false, ANIM_STATUS(33, true), { Q12(-40.0f) }, 570, 579 },
    /* 126 */ { Anim_BlendLinear, ANIM_STATUS(34, false), false, ANIM_STATUS(34, true), { Q12(100.0f) }, NO_VALUE, 592 },
    /* 127 */ { Anim_PlayOnce,    ANIM_STATUS(34, true),  false, ANIM_STATUS(34, true), { Q12(-40.0f) }, 580, 592 },
    /* 128 */ { Anim_BlendLinear, ANIM_STATUS(35, false), false, ANIM_STATUS(35, true), { Q12(100.0f) }, NO_VALUE, 592 },
    /* 129 */ { Anim_PlayOnce,    ANIM_STATUS(35, true),  false, ANIM_STATUS(35, true), { Q12(-20.0f) }, 570, 592 },
    /* 130 */ { Anim_BlendLinear, ANIM_STATUS(36, false), false, ANIM_STATUS(36, true), { Q12(100.0f) }, NO_VALUE, 582 },
    /* 131 */ { Anim_PlayOnce,    ANIM_STATUS(36, true),  false, ANIM_STATUS(36, true), { Q12(22.0f)  }, 582, 604 },

    /* `EquippedWeaponId_HyperBlaster` */
    /* 132 */ { Anim_BlendLinear, ANIM_STATUS(28, false), false, ANIM_STATUS(28, true), { Q12(100.0f) }, NO_VALUE, 568 },
    /* 133 */ { Anim_PlayOnce,    ANIM_STATUS(28, true),  false, ANIM_STATUS(28, true), { Q12(24.0f)  }, 568, 574 },
    /* 134 */ { Anim_BlendLinear, ANIM_STATUS(29, false), false, ANIM_STATUS(29, true), { Q12(100.0f) }, NO_VALUE, 574 },
    /* 135 */ { Anim_PlayOnce,    ANIM_STATUS(29, true),  false, ANIM_STATUS(29, true), { Q12(20.0f)  }, 574, 574 },
    /* 136 */ { Anim_BlendLinear, ANIM_STATUS(30, false), false, ANIM_STATUS(30, true), { Q12(100.0f) }, NO_VALUE, 575 },
    /* 137 */ { Anim_PlayOnce,    ANIM_STATUS(30, true),  false, ANIM_STATUS(30, true), { Q12(20.0f)  }, 575, 579 },
    /* 138 */ { Anim_BlendLinear, ANIM_STATUS(31, false), false, ANIM_STATUS(31, true), { Q12(100.0f) }, NO_VALUE, 568 },
    /* 139 */ { Anim_PlayOnce,    ANIM_STATUS(31, true),  false, ANIM_STATUS(31, true), { Q12(17.0f)  }, 568, 568 },
    /* 140 */ { Anim_BlendLinear, ANIM_STATUS(32, false), false, ANIM_STATUS(32, true), { Q12(100.0f) }, NO_VALUE, 568 },
    /* 141 */ { Anim_PlayOnce,    ANIM_STATUS(32, true),  false, ANIM_STATUS(32, true), { Q12(24.0f)  }, 568, 574 },
    /* 142 */ { Anim_BlendLinear, ANIM_STATUS(33, false), false, ANIM_STATUS(33, true), { Q12(100.0f) }, NO_VALUE, 574 },
    /* 143 */ { Anim_PlayOnce,    ANIM_STATUS(33, true),  false, ANIM_STATUS(33, true), { Q12(-20.0f) }, 568, 574 },
    /* 144 */ { Anim_BlendLinear, ANIM_STATUS(34, false), false, ANIM_STATUS(34, true), { Q12(100.0f) }, NO_VALUE, 574 },
    /* 145 */ { Anim_PlayOnce,    ANIM_STATUS(34, true),  false, ANIM_STATUS(34, true), { Q12(-20.0f) }, 574, 574 },
    /* 146 */ { Anim_BlendLinear, ANIM_STATUS(35, false), false, ANIM_STATUS(35, true), { Q12(100.0f) }, NO_VALUE, 574 },
    /* 147 */ { Anim_PlayOnce,    ANIM_STATUS(35, true),  false, ANIM_STATUS(35, true), { Q12(-20.0f) }, 568, 574 },
    /* 148 */ { Anim_BlendLinear, ANIM_STATUS(36, false), false, ANIM_STATUS(36, true), { Q12(100.0f) }, NO_VALUE, 574 },
    /* 149 */ { Anim_PlayOnce,    ANIM_STATUS(36, true),  false, ANIM_STATUS(36, true), { Q12(20.0f)  }, 574, 579 }
};

const s_800C44F0 D_800294F4[] = {
    {
        .unused_0         = { 56, 57, 100, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 577
    },
    {
        .unused_0         = { 58, 59, 101, 0 },
        .startKeyframeIdx = 579,
        .endKeyframeIdx   = 598
    },
    {
        .unused_0         = { 60, 61, 102, 0 },
        .startKeyframeIdx = 599,
        .endKeyframeIdx   = 615
    },
    {
        .unused_0         = { 62, 63, 103, 0 },
        .startKeyframeIdx = 616,
        .endKeyframeIdx   = 635
    },
    {
        .unused_0         = { 64, 65, 100, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 577
    },
    {
        .unused_0         = { 56, 57, 110, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 579
    },
    {
        .unused_0         = { 58, 59, 111, 0 },
        .startKeyframeIdx = 584,
        .endKeyframeIdx   = 613
    },
    {
        .unused_0         = { 60, 61, 112, 0 },
        .startKeyframeIdx = 614,
        .endKeyframeIdx   = 634
    },
    {
        .unused_0         = { 62, 63, 113, 0 },
        .startKeyframeIdx = 637,
        .endKeyframeIdx   = 659
    },
    {
        .unused_0         = { 64, 65, 110, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 579
    },
    {
        .unused_0         = { 56, 57, 110, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 579
    },
    {
        .unused_0         = { 58, 59, 111, 0 },
        .startKeyframeIdx = 584,
        .endKeyframeIdx   = 613
    },
    {
        .unused_0         = { 60, 61, 112, 0 },
        .startKeyframeIdx = 614,
        .endKeyframeIdx   = 634
    },
    {
        .unused_0         = { 62, 63, 113, 0 },
        .startKeyframeIdx = 637,
        .endKeyframeIdx   = 659
    },
    {
        .unused_0         = { 64, 65, 110, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 579
    },
    {
        .unused_0         = { 56, 57, 120, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 575
    },
    {
        .unused_0         = { 58, 59, 121, 0 },
        .startKeyframeIdx = 583,
        .endKeyframeIdx   = 595
    },
    {
        .unused_0         = { 60, 61, 122, 0 },
        .startKeyframeIdx = 596,
        .endKeyframeIdx   = 611
    },
    {
        .unused_0         = { 62, 63, 123, 0 },
        .startKeyframeIdx = 613,
        .endKeyframeIdx   = 629
    },
    {
        .unused_0         = { 64, 65, 120, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 577
    },
    {
        .unused_0         = { 56, 57, 190, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 579
    },
    {
        .unused_0         = { 58, 59, 191, 0 },
        .startKeyframeIdx = 580,
        .endKeyframeIdx   = 597
    },
    {
        .unused_0         = { 60, 61, 192, 0 },
        .startKeyframeIdx = 598,
        .endKeyframeIdx   = 611
    },
    {
        .unused_0         = { 62, 63, 193, 0 },
        .startKeyframeIdx = 612,
        .endKeyframeIdx   = 625
    },
    {
        .unused_0         = { 64, 65, 190, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 579
    },
    {
        .unused_0         = { 56, 57, 160, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 583
    },
    {
        .unused_0         = { 58, 59, 161, 0 },
        .startKeyframeIdx = 584,
        .endKeyframeIdx   = 602
    },
    {
        .unused_0         = { 60, 61, 162, 0 },
        .startKeyframeIdx = 603,
        .endKeyframeIdx   = 618
    },
    {
        .unused_0         = { 62, 63, 163, 0 },
        .startKeyframeIdx = 619,
        .endKeyframeIdx   = 637
    },
    {
        .unused_0         = { 64, 65, 164, 0 },
        .startKeyframeIdx = 638,
        .endKeyframeIdx   = 649
    },
    {
        .unused_0         = { 66, 67, 164, 0 },
        .startKeyframeIdx = 638,
        .endKeyframeIdx   = 649
    },
    {
        .unused_0         = { 68, 69, 165, 0 },
        .startKeyframeIdx = 650,
        .endKeyframeIdx   = 655
    },
    {
        .unused_0         = { 56, 57, 180, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 583
    },
    {
        .unused_0         = { 58, 59, 181, 0 },
        .startKeyframeIdx = 584,
        .endKeyframeIdx   = 597
    },
    {
        .unused_0         = { 60, 61, 182, 0 },
        .startKeyframeIdx = 598,
        .endKeyframeIdx   = 611
    },
    {
        .unused_0         = { 62, 63, 183, 0 },
        .startKeyframeIdx = 612,
        .endKeyframeIdx   = 625
    },
    {
        .unused_0         = { 64, 65, 184, 0 },
        .startKeyframeIdx = 626,
        .endKeyframeIdx   = 636
    },
    {
        .unused_0         = { 66, 67, 184, 0 },
        .startKeyframeIdx = 626,
        .endKeyframeIdx   = 636
    },
    {
        .unused_0         = { 68, 69, 185, 0 },
        .startKeyframeIdx = 637,
        .endKeyframeIdx   = 640
    },
    {
        .unused_0         = { 56, 57, 130, 0 },
        .startKeyframeIdx = 570,
        .endKeyframeIdx   = 579
    },
    {
        .unused_0         = { 58, 59, 131, 0 },
        .startKeyframeIdx = 582,
        .endKeyframeIdx   = 592
    },
    {
        .unused_0         = { 60, 61, 132, 0 },
        .startKeyframeIdx = 594,
        .endKeyframeIdx   = 604
    },
    {
        .unused_0         = { 62, 63, 133, 0 },
        .startKeyframeIdx = 605,
        .endKeyframeIdx   = 658
    },
    {
        .unused_0         = { 64, 65, 130, 0 },
        .startKeyframeIdx = 570,
        .endKeyframeIdx   = 592
    },
    {
        .unused_0         = { 66, 67, 130, 0 },
        .startKeyframeIdx = 570,
        .endKeyframeIdx   = 579
    },
    {
        .unused_0         = { 68, 69, 131, 0 },
        .startKeyframeIdx = 580,
        .endKeyframeIdx   = 592
    },
    {
        .unused_0         = { 70, 71, 130, 0 },
        .startKeyframeIdx = 570,
        .endKeyframeIdx   = 592
    },
    {
        .unused_0         = { 72, 73, 131, 0 },
        .startKeyframeIdx = 582,
        .endKeyframeIdx   = 604
    },
    {
        .unused_0         = { 56, 57, 140, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 587,
    },
    {
        .unused_0         = { 58, 59, 141, 0 },
        .startKeyframeIdx = 588,
        .endKeyframeIdx   = 597
    },
    {
        .unused_0         = { 60, 61, 142, 0 },
        .startKeyframeIdx = 598,
        .endKeyframeIdx   = 607
    },
    {
        .unused_0         = { 62, 63, 143, 0 },
        .startKeyframeIdx = 608,
        .endKeyframeIdx   = 642
    },
    {
        .unused_0         = { 64, 65, 140, 0 },
        .startKeyframeIdx = 588,
        .endKeyframeIdx   = 597
    },
    {
        .unused_0         = { 66, 67, 140, 0 },
        .startKeyframeIdx = 588,
        .endKeyframeIdx   = 597
    },
    {
        .unused_0         = { 68, 69, 141, 0 },
        .startKeyframeIdx = 597,
        .endKeyframeIdx   = 597
    },
    {
        .unused_0         = { 70, 71, 140, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 587
    },
    {
        .unused_0         = { 72, 73, 140, 0 },
        .startKeyframeIdx = 599,
        .endKeyframeIdx   = 607
    },
    {
        .unused_0         = { 56, 57, 140, 0 },
        .startKeyframeIdx = 570,
        .endKeyframeIdx   = 579
    },
    {
        .unused_0         = { 58, 59, 141, 0 },
        .startKeyframeIdx = 582,
        .endKeyframeIdx   = 592
    },
    {
        .unused_0         = { 60, 61, 142, 0 },
        .startKeyframeIdx = 594,
        .endKeyframeIdx   = 604
    },
    {
        .unused_0         = { 62, 63, 153, 0 },
        .startKeyframeIdx = 605,
        .endKeyframeIdx   = 641
    },
    {
        .unused_0         = { 64, 65, 140, 0 },
        .startKeyframeIdx = 570,
        .endKeyframeIdx   = 592
    },
    {
        .unused_0         = { 66, 67, 140, 0 },
        .startKeyframeIdx = 570,
        .endKeyframeIdx   = 579
    },
    {
        .unused_0         = { 68, 69, 141, 0 },
        .startKeyframeIdx = 580,
        .endKeyframeIdx   = 592
    },
    {
        .unused_0         = { 70, 71, 140, 0 },
        .startKeyframeIdx = 570,
        .endKeyframeIdx   = 592
    },
    {
        .unused_0         = { 72, 73, 140, 0 },
        .startKeyframeIdx = 582,
        .endKeyframeIdx   = 604
    },
    {
        .unused_0         = { 56, 57, 170, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 574
    },
    {
        .unused_0         = { 58, 59, 170, 0 },
        .startKeyframeIdx = 574,
        .endKeyframeIdx   = 574
    },
    {
        .unused_0         = { 60, 61, 173, 0 },
        .startKeyframeIdx = 575,
        .endKeyframeIdx   = 579
    },
    {
        .unused_0         = { 62, 63, 173, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 568
    },
    {
        .unused_0         = { 64, 65, 170, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 574
    },
    {
        .unused_0         = { 66, 67, 170, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 574
    },
    {
        .unused_0         = { 68, 69, 170, 0 },
        .startKeyframeIdx = 574,
        .endKeyframeIdx   = 574
    },
    {
        .unused_0         = { 70, 71, 170, 0 },
        .startKeyframeIdx = 568,
        .endKeyframeIdx   = 574
    },
    {
        .unused_0         = { 72, 73, 172, 0 },
        .startKeyframeIdx = 574,
        .endKeyframeIdx   = 579
    }
};

const __pad = 0;
