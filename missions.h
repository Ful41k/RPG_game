/**
 * @file missions.h
 * @brief Function prototypes and constants for the RPG mission system.
 */

#ifndef MISSIONS_H
#define MISSIONS_H

#include "player.h"

/** @name Mission IDs
 * Numeric identifiers for the game's biomes.
 * @{ */
#define MISSION_SWAMP 1     /**< The Rotting Swamp (Mission 1) */
#define MISSION_MANSION 2   /**< The Haunted Mansion (Mission 2) */
#define MISSION_CAVE 3      /**< The Crystal Cave (Mission 3) */
#define MISSION_DARK_LORD 4 /**< The Dark Lord's Castle (Final Mission) */
/** @} */

/**
 * @brief Main entry point to start a specific mission biome.
 * @param hero Pointer to the player structure.
 * @param missionId The ID of the mission to be launched (1-4).
 */
void runMission(Player* hero, int missionId);

/**
 * @brief Utility function to pause execution until the user presses Enter.
 * Clears the input buffer before waiting.
 */
void waitForEnter();

#endif
