#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "player.h"
#include "village-menu.h"
#include "save.h"
#include "cheats.h"
#include "color.h"
#include "utils.h"

int main() {
    // Кодировка только для Windows
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    srand(time(NULL));

    print_typewriter(RED "\n=== SHADOWS OF ARXEN ===" RESET, 60);

    while (1) {
        print_typewriter("\n=== Main Menu ===", 20);
        print_typewriter("1. New Game", 20);
        print_typewriter("2. Load Game", 20);
        print_typewriter("3. Exit", 20);
        print_prompt("Choose: ", 20);

        int choice;
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        if (choice == 1) {
            Player hero = createNewHero();
            enterVillage(&hero);
        } else if (choice == 2) {
            Player hero;
            if (loadGame(&hero)) {
                print_typewriter(GREEN "Save loaded successfully." RESET, 20);
                enterVillage(&hero);
            } else {
                print_typewriter(RED "No save file found." RESET, 20);
            }
        } else if (choice == 3) {
            print_typewriter("Closing game...", 40);
            break;
        }
    }
    return 0;
}