/**
 * @file tavern.h
 * @brief Social hub for healing, gambling, and quest management.
 */

#ifndef TAVERN_H
#define TAVERN_H

#include "player.h"

/**
 * @brief Entry point for the Tavern. Changes behavior based on hero->isDay.
 * - Day: Trading and Quest dialogue.
 * - Night: Gambling and Sleeping.
 */
void enterTavern(Player* hero);

#endif