/**
 * @file save.h
 * @brief Binary serialization for player progress.
 */

#ifndef SAVE_H
#define SAVE_H
#include "player.h"

/**
 * @brief Saves the current Hero state to a binary file (.dat).
 * Automatically finds the next available slot (save_1.dat, save_2.dat, etc.).
 */
void saveGame(Player* hero);

/**
 * @brief Displays available saves and handles Loading or Deletion.
 * @return 1 if a game was successfully loaded, 0 otherwise.
 */
int loadGameMenu(Player* hero);

#endif