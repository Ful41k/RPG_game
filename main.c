/**
 * @file main.c
 * @brief Game entry point and Konami Code handler.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "player.h"
#include "village-menu.h"
#include "save.h"
#include "cheats.h"
#include "color.h"
#include "utils.h"

/**
 * @brief Main execution loop.
 * * Features a custom Konami Code buffer (wwssadadba ) that unlocks 
 * the hidden cheat menu when typed at the main menu prompt.
 */
int main() {
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif

    srand(time(NULL));
    clear_screen();

    char konamiBuffer[13] = ""; 
    const char* SECRET_CODE = "wwssadadba ";
    int cheatsEnabled = 0;

    print_typewriter(RED "\n=== SHADOWS OF ARXEN ===" RESET, 60);

    while (1) {
        printf("\n=== Main Menu ===\n");
        printf("1. New Game\n");
        printf("2. Load Game\n");
        
        if (cheatsEnabled) {
            printf(MAGENTA "3. Cheats\n" RESET);
            printf("4. Exit\n");
        } else {
            printf("3. Exit\n");
        }

        print_prompt("Choose an action: ", 20);

        char inputLine[64];

        if (fgets(inputLine, sizeof(inputLine), stdin) == NULL) continue;
        

        inputLine[strcspn(inputLine, "\n")] = 0;


        for (int i = 0; inputLine[i] != '\0' || (i == 0 && inputLine[0] == '\0'); i++) {
            char c = (inputLine[0] == '\0') ? ' ' : inputLine[i]; 
            

            size_t currentLen = strlen(konamiBuffer);
            if (currentLen >= 11) {
                memmove(konamiBuffer, konamiBuffer + 1, 11);
                konamiBuffer[10] = c;
            } else {
                konamiBuffer[currentLen] = c;
                konamiBuffer[currentLen + 1] = '\0';
            }

            // Check for match
            if (strcmp(konamiBuffer, SECRET_CODE) == 0) {
                cheatsEnabled = 1;
                print_typewriter(MAGENTA "\nCHEATS UNLOCKED!\n" RESET, 30);
                break;
            }
            
            if (inputLine[0] == '\0') break; 
        }

        int choice = atoi(inputLine);

        if (choice == 1) {
            clear_screen();
            Player hero = createNewHero();
            enterVillage(&hero);
        } 
        else if (choice == 2) {
            Player hero;
            if (loadGameMenu(&hero)) {
                print_typewriter(GREEN "Save loaded successfully." RESET, 20);
                delay_ms(500);
                clear_screen();
                enterVillage(&hero);
            }
        } 
        else if (cheatsEnabled && choice == 3) {
            Player tempHero; 
            if (loadGameMenu(&tempHero)) { 
                runCheatMenu(&tempHero); 
                saveGame(&tempHero); 
                print_typewriter(MAGENTA "Cheat applied and save file updated!" RESET, 20);
            }
        }
        else if ((!cheatsEnabled && choice == 3) || (cheatsEnabled && choice == 4)) {
            print_typewriter("Closing game...", 40);
            break; 
        }
    }
    return 0;
}