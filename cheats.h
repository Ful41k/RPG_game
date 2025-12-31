/**
 * @file cheats.h
 * @brief Development and debug tools for the RPG.
 */

#ifndef CHEATS_H
#define CHEATS_H

#include "player.h"

/**
 * @brief Launches the secret developer cheat menu.
 * * Allows for modifying player stats, granting items, and unlocking 
 * all missions for testing purposes.
 * * @param hero Pointer to the player structure to be modified.
 */
void runCheatMenu(Player* hero);

#endif