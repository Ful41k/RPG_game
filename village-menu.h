/**
 * @file village-menu.h
 * @brief Header for the village hub system.
 * * This module acts as the central navigation hub for the game. It connects
 * the player to the Tavern, the Blacksmith, and the Mission systems.
 */

#ifndef VILLAGE_MENU_H
#define VILLAGE_MENU_H

#include "player.h"

/**
 * @brief Enters the main village square loop.
 * * This is the primary game state. It displays the village menu and 
 * handles navigation between sub-locations. It also manages the 
 * visibility of locations based on whether it is Day or Night.
 * * @param hero Pointer to the Player structure to track location and time state.
 */
void enterVillage(Player* hero);

#endif