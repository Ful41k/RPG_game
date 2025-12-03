#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "cheats.h"
#include "player.h"
#include "utils.h"
#include "village-menu.h"
#include "save.h"

void showInitialMenu(bool cheatsVisible) {
    printf("\n=== Legends of the Dark Lord ===\n");
    printf("1. New Game\n");
    printf("2. Load Game\n");
    if (cheatsVisible) {
        printf("3. Cheats\n");
    }
    printf("Choose an action: ");
}

void processInitialMenu(CheatState* cheats, Save* savedGames, int savedCount) {
    char input[100];
    showInitialMenu(cheats->cheatsUnlocked);

    if (!fgets(input, sizeof(input), stdin)) {
        return;
    }

    for (int i = 0; i < strlen(input); i++) {
        char ch = input[i];
        if (ch == ' ' || ch == '\n') continue;
        processKonamiInput(cheats, ch);
    }
    if (cheats->cheatsUnlocked) {
        return;  // prevents "Invalid choice"
    }

    int choice = atoi(input);

    if (choice == 1) {
        Player hero = createNewHero();
        villageIntro(&hero);
        villageMenu(&hero);
        return;
    }
    if (choice == 2) {
        loadGameMenu();
        return;
    }
    if (choice == 3 && cheats->cheatsUnlocked) {
        showCheatsMenu(savedGames, savedCount);
        return;
    }

    printf("Invalid choice.\n");
}



int main(void) {
    Save savedGames[10];
    int savedCount = 0;
    CheatState cheat = {0};

    while (1) {
        processInitialMenu(&cheat, savedGames, savedCount);
    }

    return 0;
}