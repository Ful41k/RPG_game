#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "player.h"
#include "utils.h"
#include "village-menu.h"

// This defines a new type called Button.
typedef struct {
    char label[20];
} Button;

// This function checks if the user selected a particular menu option.
static bool pointInMenu(int choice, int buttonIndex) {
    return choice == buttonIndex + 1;
}

// Function to Draw the Menu
static void drawMenu(Button buttons[], int count) {
    printf("\n=== Legends of the Dark Lord ===\n");
    for (int i = 0; i < count; ++i) {
        printf("%d. %s\n", i + 1, buttons[i].label);
    }
    printf("\nChoose an action [1-%d]: ", count);
}


void showInitialMenu(bool cheatsVisible) {
    printf("\n=== Initial Game Menu ===\n");
    printf("1. New Game\n");
    printf("2. Load Game\n");
    if (cheatsVisible) {
        printf("3. Cheats\n");
    }
    printf("Choose an action: ");
}
/*
void processInitialMenu() {
    bool cheatsUnlocked = false;
    int choice;

    while (1) {
        showInitialMenu(cheatsUnlocked);

        // Read input
        int result = scanf("%d", &choice);

        if (result == 1) {
            // Player entered a number
            if (choice == 1) {
                newGame();
            }
            else if (choice == 2) {
                loadGameMenu();
            }
            else if (choice == 3 && cheatsUnlocked) {
                cheatsMenu();
            }
            else {
                printf("Invalid input. Try again.\n");
            }
        }
        else {
            // Player typed a character → check for Konami code
            char c;
            scanf("%c", &c); // read the non-numeric char

            if (detectKonamiCode(c)) {
                cheatsUnlocked = true;
                printf("\n*** Cheats unlocked! ***\n");
            } 
        }
    }
} */


int main(void) {
    Player hero = createNewHero();

    // showInitialMenu();
    villageIntro(&hero);
    villageMenu(&hero);
    return 0;
}