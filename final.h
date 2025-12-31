/**
 * @file final.h
 * @brief Entry point for the final confrontation with Arxen Voidheart.
 */

#ifndef FINAL_H
#define FINAL_H

#include "player.h"

/**
 * @brief Main sequence for the Dark Lord's Castle mission.
 * Handles the 10-corridor progression, random Void whispers, 
 * and triggers the final boss battle.
 * @param hero Pointer to the player structure.
 */
void runFinalMission(Player* hero);

/**
 * @brief A recursive mathematical requirement (Padovan sequence).
 * @param n The index in the sequence.
 * @return The n-th Padovan number.
 */
int padovan(int n);

#endif