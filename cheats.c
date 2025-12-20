#include <stdio.h>
#include "cheats.h"
#include "player.h"
#include "utils.h"
#include "color.h"

#define TEXT_SPEED 20

void runCheatMenu(Player* hero) {
    print_typewriter(MAGENTA "\n=== SECRET DEV MENU ===" RESET, TEXT_SPEED);
    print_typewriter("1. Add 1000 Coins", TEXT_SPEED);
    print_typewriter("2. Full Heal", TEXT_SPEED);
    print_typewriter("3. Instant Level Up", TEXT_SPEED);
    print_typewriter("4. Unlock All Missions", TEXT_SPEED);
    print_typewriter("5. God Mode (Huge Stats)", TEXT_SPEED);
    print_typewriter("6. Back", TEXT_SPEED);
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
        hero->hp = hero->maxhp;
        print_typewriter(GREEN "HP fully restored." RESET, TEXT_SPEED);
    }
    else if (choice == 3) {
        // Добавляем достаточно опыта для уровня
        addXp(hero, hero->xpToNextLevel); 
    }
    else if (choice == 4) {
        hero->maxUnlockedMission = 4; // Открываем всё до замка лорда
        print_typewriter(GREEN "All missions unlocked." RESET, TEXT_SPEED);
    }
    else if (choice == 5) {
        hero->atk = 999;
        hero->def = 999;
        hero->maxhp = 999;
        hero->hp = 999;
        print_typewriter(RED "GOD MODE ACTIVATED." RESET, TEXT_SPEED);
    }
    else {
        print_typewriter("Closing dev menu...", TEXT_SPEED);
    }
}