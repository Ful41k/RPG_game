/**
 * @file cheats.c
 * @brief Implementation of developer cheat functions.
 */

#include <stdio.h>
#include "cheats.h"
#include "player.h"
#include "utils.h"
#include "color.h"

#define TEXT_SPEED 20

/**
 * @brief Displays the secret dev menu and handles input for stat modification.
 * * Options:
 * 1. Add 1000 Coins.
 * 2. Unlock all missions and grant quest items (Sword/Key).
 * 3. Set all stats to 999.
 * * @param hero Pointer to the player structure.
 */
void runCheatMenu(Player* hero) {
    print_typewriter(MAGENTA "\n=== SECRET DEV MENU ===" RESET, TEXT_SPEED);
    print_typewriter("1. Add 1000 Coins", TEXT_SPEED);
    print_typewriter("2. Unlock All Missions", TEXT_SPEED);
    print_typewriter("3. Creative Mode (Huge Stats)", TEXT_SPEED);
    print_typewriter("4. Back", TEXT_SPEED);
    printf("Choose: ");

    int choice;
    if (scanf("%d", &choice) != 1) {
        int c; while ((c = getchar()) != '\n' && c != EOF) {}
        return;
    }

    if (choice == 1) {
        hero->coins += 1000;
        print_typewriter(GREEN "1000 Coins added." RESET, TEXT_SPEED);
    } 
    else if (choice == 2) {
        hero->maxUnlockedMission = 4; // Открываем всё до замка лорда

        hero->hasHeroSword = 1;
        hero->hasCastleKey = 1;

        print_typewriter(GREEN "All missions unlocked. Hero's Sword and Castle Key granted." RESET, TEXT_SPEED);
    }
    else if (choice == 3) {
        hero->atk = 999;
        hero->def = 999;
        hero->maxhp = 999;
        hero->hp = 999;
        print_typewriter(RED "CREATIVE MODE ACTIVATED." RESET, TEXT_SPEED);
    }
    else {
        print_typewriter("Closing dev menu...", TEXT_SPEED);
    }
}