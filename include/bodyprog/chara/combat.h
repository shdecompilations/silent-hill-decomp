#ifndef _BODYPROG_CHARA_COMBAT_H
#define _BODYPROG_CHARA_COMBAT_H

struct _AttackInfo;
struct _SubCharacter;

/** @brief Chainsaw and Rock Drill sounds driven by `Combat_GasWeaponSfxUpdate`. */
typedef enum _GasWeaponSfx
{
    GasWeaponSfx_Contact        = 0, /** `Sfx_GasWeaponContact` */
    GasWeaponSfx_ChainsawRun    = 1, /** `Sfx_ChainsawRun` */
    GasWeaponSfx_RockDrillStart = 2, /** `Sfx_RockDrillStart` */
    GasWeaponSfx_RockDrillRun   = 3, /** `Sfx_RockDrillRun` */

    GasWeaponSfx_Hack = NO_VALUE // @hack Force enum to be treated as `s32`.
} e_GasWeaponSfx;

/** @brief Character damage info. */
typedef struct _CharaDamage
{
    /* 0x0 */ VECTOR3 position;
    /* 0xC */ q19_12  amount;
} s_CharaDamage;

/** @brief Character attach states. Used for `s_CharaAttack::state`. */
typedef enum _CharaAttackState
{
    CharaAttackState_Idle      = 0,
    CharaAttackState_Requested = 1, /** Set by character code to start an attack or when its animation restarts. */
    CharaAttackState_Started   = 2, /** First tick of hit checks. Melee attacks set up their hit line history. */
    CharaAttackState_Active    = 3,
    CharaAttackState_Finished  = NO_VALUE
} e_CharaAttackState;

/** @brief State of the attack a character is currently performing.
 *
 * Hits are checked along a hit line which runs along the aim from `rangeMin` to `rangeMax` past `origin`.
 * Character code moves them with the animation (e.g. Harry's hand origin).
 * The last tick's hit line is kept too, so melee checks can also check the swing in between * in case a fast animation
 * moved past something hittable between ticks.
 *
 * @note SH2 keeps similar data to this inside `shBattleInfo` and seemed to move some `struct _SubCharacter` fields
 * (`flags`, `field_40`, `attackReceived`) inside it.
 */
typedef struct _CharaAttack
{
    /* 0x0  */ s16     state;           /** `e_CharaAttackState`. Reset to `CharaAttackState_Idle` by `Combat_AttackCleanup` when `CharaFlag_AttackTicked` is clear. */
    /* 0x2  */ u8      weaponAttack;    /** Packed `WEAPON_ATTACK` being performed. Index into `g_Items_WeaponAttackInfos`. */
    /* 0x3  */ u8      shotsLeft;       /** Gunshots only. Set to 100 on start, decremented per shot. Attack ends at 0. */
    /* 0x4  */ u8      shotsFired;      /** Gunshots only. Compared against `s_AttackShot::shotCountMax`. */
    /* 0x5  */ s8      __pad_5[3];
    /* 0x8  */ s32     hitFlags;        /** Bit `1 << npcIdx` per NPC hit by this attack, `NO_VALUE` (all bits set) if the player is hit. */
    /* 0xC  */ q3_12   aimYaw;
    /* 0xE  */ q3_12   aimPitch;        /** Measured from vertical. 90 degrees is level. */
    /* 0x10 */ q3_12   prevAimYaw;
    /* 0x12 */ q3_12   prevAimPitch;
    /* 0x14 */ q19_12  hitCheckTime;    /** Animation time that hit checks have been run up to, clamped to the attack's hit window. */
    /* 0x18 */ VECTOR3 origin;          /** Q19.12 | Attack origin or ray start for gunshots. */
    /* 0x24 */ VECTOR3 hitLineStart[2]; /** Q19.12 | Hit line start (`origin + direction * rangeMin`). [0]: this step, [1]: previous step. */
    /* 0x3C */ VECTOR3 prevOrigin;      /** Q19.12 | `origin` at the end of the previous tick. */
    /* 0x48 */ VECTOR3 hitLineVec[2];   /** Q19.12 | Hit line (`direction * (rangeMax - rangeMin)`). [0]: this step, [1]: previous step.
                                          * Melee attacks run 4 steps per tick. Gunshots put the pellet ray in [0] and the pellet direction in [1]. */
    /* 0x60 */ VECTOR3 prevTip;         /** Q19.12 | Previous tick's hit line end relative to `prevOrigin`. Read by `Combat_HitApply` but never written.
                                          * Always 0 after the `bzero` during character spawn? */
} s_CharaAttack;

/** @brief Computes a Q12 square root using the GTE leading zero count and the `SQRT` table.
 *
 * TODO: Could be handwritten asm based on the register pins needed here, maybe as a separate `.s` from this file since
 * this also gets called from player_control.c. Uncertain though, the rest of the code does match as C.
 *
 * @note Same algorithm as PsyQ `SquareRoot12`.
 *
 * @param x Value to use in the computation.
 * @return Square root of `x`.
 */
u32 Math_SquareRootQ12(s32 x);

/** @brief @unused Returns 0. */
s32 func_8008A0CC(void);

/** @brief @unused Returns 0. */
s64 func_8008A0D4(void);

/** @brief Starts, continues, or cancels an attack and runs its hit checks for the current tick.
 *
 * Called each tick by the attacking character's update. Not calling it cancels the attack. See `CharaFlag_AttackTicked`.
 *
 * @param request `CharaAttackState_Requested` (or any other active state) starts the attack if idle, otherwise continues it.
 *                `CharaAttackState_Idle` or `CharaAttackState_Finished` cancels it.
 *                Callers pass `CharaAttackState_Requested`, or their own `s_CharaAttack::state` to continue until it goes idle.
 * @param weaponAttack Packed weapon attack.
 * @param chara Attacking character.
 * @param attackPos Attack position.
 * @param unused Unused character.
 * @param aimYaw Aim Y angle.
 * @param aimPitch Aim X angle.
 * @return Player: `NPC_COUNT_MAX - (index of the lowest NPC hit)`, or `NO_VALUE` if none.
 *         Others: 0 if the player has an `attackReceived` set, otherwise `NO_VALUE`.
 */
s32 Combat_AttackPerform(e_CharaAttackState request, s32 weaponAttack, struct _SubCharacter* chara, VECTOR3* attackPos,
                         struct _SubCharacter* unused, q3_12 aimYaw, q3_12 aimPitch);

/** @brief Gets the first keyframe of an attack's hit window from `s_AttackInfo::hitStartKeyframe`.
 * Returns 0 while a gas weapon's engine is revved, so the hit window is always open.
 *
 * @param weaponAttack Packed weapon attack.
 */
u32 Combat_AttackStartKeyframeGet(s32 weaponAttack);

/** @brief Gets the keyframe count of an attack's hit window from `s_AttackInfo::hitKeyframeCount`.
 * Revved Chainsaw and Rock Drill attacks use fixed counts instead.
 */
u32 Combat_AttackKeyframeCountGet(s32 weaponAttack);

/** @brief Checks if a keyframe is inside an attack's hit window.
 *
 * Reads `s_AttackInfo::hitStartKeyframe` and `s_AttackInfo::hitKeyframeCount` directly, unlike
 * `Combat_AttackStartKeyframeGet` and `Combat_AttackKeyframeCountGet`, ignores the revved gas weapon overrides.
 *
 * @param attackInfo Attack to check.
 * @param keyframe Keyframe relative to the start of the attack animation.
 * @return `true` if `keyframe` is in `[hitStartKeyframe, hitStartKeyframe + hitKeyframeCount)`, otherwise `false`.
 */
bool Combat_AttackHitWindowCheck(struct _AttackInfo* attackInfo, s32 keyframe);

/** @brief Clears `CharaFlag_AttackTicked`. Called before a character's update. */
void Combat_CharaAttackTickedClear(struct _SubCharacter* chara);

/** @brief Sets `CharaFlag_AttackTicked`. Called by `Combat_AttackUpdate`. */
void Combat_CharaAttackTickedSet(struct _SubCharacter* chara);

/** @brief Resets an attack a character stopped performing.
 *
 * Character code ends an attack by no longer calling `Combat_AttackPerform` each tick, so `Combat_AttackUpdate`
 * doesn't run and doesn't set `CharaFlag_AttackTicked`.
 * This function checks if `CharaFlag_AttackTicked` is still unset and changes state to `CharaAttackState_Idle` if not.
 * `Combat_AttackUpdate` is then called with the state set to idle to clear hit flags/timers.
 *
 * @param chara Character to update.
 */
void Combat_AttackCleanup(struct _SubCharacter* chara);

/** @brief Advances an attack's state and runs its hit checks for this tick.
 *
 * Gunshots fire one shot per elapsed `s_AttackShot::interval` since the last tick, tracing a ray for each pellet.
 * Other attacks check 4 hit lines spaced between the previous tick's aim and origin and the current ones, running
 * `Combat_MeleeHitCheck` on each. Non-Slash attacks stop at the first hit.
 * When one of Harry's attacks finishes, it's recorded in the ranking statistics with `func_8009151C`.
 * Also sets `CharaFlag_AttackTicked`, so `Combat_AttackCleanup` leaves the attack alone this tick.
 *
 * @param attacker Attacking character.
 * @return `1 << npcIdx` per NPC hit so far during the whole attack, `NO_VALUE` if the player was hit.
 *         0 when the attack is idle/finished/inactive.
 */
s32 Combat_AttackUpdate(struct _SubCharacter* attacker);

/** @brief Sets up a melee attack's hit line on its first active tick.
 *
 * Computes the hit line for the current aim with `Combat_AttackHitLineUpdate`, and sets `prevAimYaw`,
 * `prevAimPitch` and `prevOrigin` to the current values.
 *
 * @param attacker Attacking character.
 */
void Combat_AttackHitLineInit(struct _SubCharacter* attacker);

/** @brief Moves the current hit line into the history slot and computes a new one for the given aim.
 *
 * The hit line starts `s_AttackInfo::rangeMin` from `s_CharaAttack::origin` and ends at `rangeMax`.
 *
 * @param attacker Attacking character.
 * @param aimYaw Aim Y angle.
 * @param aimPitch Aim X angle.
 */
void Combat_AttackHitLineUpdate(struct _SubCharacter* attacker, q19_12 aimYaw, q19_12 aimPitch);

void Combat_GasWeaponSfxAllStop(void);

void Combat_GasWeaponContactSfxUpdate(q23_8 vol);

void Combat_RockDrillRunSfxUpdate(q23_8 vol, s32 pitch);

void Combat_GasWeaponEngineSfxUpdate(bool isChainsaw, q23_8 vol, s32 pitch);

bool Combat_GasWeaponSfxUpdate(e_GasWeaponSfx sfxIdx, q23_8 vol, s32 pitch);

void func_8008B664(VECTOR3* pos, u32 weaponAttack);

/** @brief Applies one hit from an attack to a target. Applies damage/push/vibration/impact SFX/blood.
 *
 * Damage is `s_AttackInfo::damage` with weapon specific bonuses and a random 87.5% to 112.5% spread.
 * Melee hits on NPCs without `CharaFlag_Hit` deal 4x damage, while slash attacks scale damage and shock by `damageScale`.
 * Vibration and impact SFX only play on the first hit of an in-progress attack against an NPC.
 *
 * @param attacker Attacking character.
 * @param target Character being hit.
 * @param hitPos Hit position for SFX and blood (Q19.12).
 * @param damageScale Slash attack damage and shock scale (Q19.12).
 * @return Target bit: `1 << npcIdx` for NPCs, `NO_VALUE` for the player. Also OR'ed into `s_CharaAttack::hitFlags`.
 */
s32 Combat_HitApply(struct _SubCharacter* attacker, struct _SubCharacter* target, VECTOR3* hitPos, q19_12 damageScale);

/** @brief Runs one step of a melee attack's hit check and applies damage to every character hit.
 *
 * Builds a box around the current hit line in attacker-local space padded to the `s_AttackFight` size.
 * Attacks from the player test against every NPC while NPC attacks only test against the player.
 * A hit is counted when it's within 3m, has line of sight from the attacker, and collision overlaps the hit hit line
 * box.
 *
 * @param attacker Attacking character.
 * @param yaw Attack yaw,. Used to rotate candidates into attacker-local space.
 * @param attackInfo Attack being performed.
 * @param damageScale Damage scale passed to `Combat_HitApply`.
 * @return `1 << npcIdx` per NPC hit, or `NO_VALUE` if the player is hit. Also OR'ed into `s_CharaAttack::hitFlags`.
 */
s32 Combat_MeleeHitCheck(struct _SubCharacter* attacker, q19_12 yaw, struct _AttackInfo* attackInfo, q19_12 damageScale);

#endif
