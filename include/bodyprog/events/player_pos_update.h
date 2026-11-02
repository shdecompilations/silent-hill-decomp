#ifndef _BODYPROG_EVENTS_PLAYERPOSUPDATE_H
#define _BODYPROG_EVENTS_PLAYERPOSUPDATE_H

/** @brief Sets the player position from a map point. Used when transitioning between rooms.
 *
 * @param mapPoint Reference map point.
 */
void Game_PlayerMapPositionSet(s_MapPoint2d* mapPoint);

/** @brief Snaps the player to the current floor and initializes world objects? */
void Game_PlayerHeightUpdate(void);

#endif
