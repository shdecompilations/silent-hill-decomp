#ifndef _MAPS_CHARACTERS_GHOSTDOCTOR_H
#define _MAPS_CHARACTERS_GHOSTDOCTOR_H

/** @brief Ghost Doctor character control states. */
typedef enum _GhostDoctorControl
{
    GhostDoctorControl_Still = 0,
    GhostDoctorControl_1     = 1
} e_GhostDoctorControl;

/** @brief Ghost Doctor character animation indices. */
typedef enum _GhostDoctorAnim
{
    GhostDoctorAnim_Still = 0,
    GhostDoctorAnim_1     = 1
} e_GhostDoctorAnim;

/** @brief Ghost Doctor character model bone indices. */
typedef enum _GhostDoctorBone
{
    GhostDoctorBone_Root = 0
} e_GhostDoctorBone;

extern s_AnimInfo GHOST_DOCTOR_ANIM_INFOS[];

/*s_AnimInfo GHOST_DOCTOR_ANIM_INFOS[] = {
    { Anim_BlendLinear, ANIM_STATUS(0, false), false, ANIM_STATUS(0, false), { Q12(0) }, NO_VALUE, 0 },
    { Anim_PlayLoop, ANIM_STATUS(0, true), false, NO_VALUE, { Q12(30) }, NO_VALUE, 1 },
    { Anim_BlendLinear, ANIM_STATUS(1, false), false, ANIM_STATUS(1, true), { Q12(5) }, NO_VALUE, 0 },
    { Anim_PlayLoop, ANIM_STATUS(1, true), false, NO_VALUE, { Q12(15) }, 0, 15 }
};*/

/** @brief Updates an active Ghost Doctor character.
 *
 * @param ghostDoc Ghost Doctor character to update.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 */
void GhostDoctor_Update(s_SubCharacter* ghostDoc, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

/** @brief Updates the animation of a Ghost Doctor character.
 *
 * @param ghostDoc Ghost Doctor character to update.
 * @param anmHdr Character animation data.
 * @param boneCoords Character model bone coords.
 */
void GhostDoctor_AnimUpdate(s_SubCharacter* ghostDoc, s_AnmHeader* anmHdr, GsCOORDINATE2* boneCoords);

/** @brief Updates the transform of the `coord` matrix to the character's position, converting from world-space to
 * geometry space.
 *
 * @param ghostDoc Reference Ghost Doctor character.
 * @param boneCoords Output bone coords to update.
 */
void GhostDoctor_CoordTransformUpdate(s_SubCharacter* ghostDoc, GsCOORDINATE2* boneCoords);

/** @brief Initializes a Ghost Doctor character.
 *
 * @param ghostDoc Ghost Doctor character to initialize.
 */
void GhostDoctor_Init(s_SubCharacter* ghostDoc);

/** @brief Updates the rotation of the 'coord' matrix to the character's rotation.
 *
 * @param ghostDoc Reference Ghost Doctor character.
 * @param boneCoords Output bone coords to update.
 */
void GhostDoctor_CoordRotationUpdate(s_SubCharacter* ghostDoc, GsCOORDINATE2* boneCoords);

#endif
