/**
 * @file village-menu.c
 * @brief Implementation of the central hub and Blacksmith interactions.
 * * Acts as the primary state manager for the player's loop. It controls
 * access to missions based on time of day and handles permanent player
 * upgrades through the Blacksmith.
 */

#include <stdio.h>
#include <stdlib.h>
#include "village-menu.h"
#include "player.h"
#include "tavern.h"
#include "missions.h"
#include "save.h"
#include "color.h"
#include "utils.h"
#include "cheats.h"

#define TEXT_SPEED 20

/**
 * @brief Renders the visual status bar at the top of the terminal.
 * * Displays:
 * - Player identity (Name)
 * - Vitality (HP/MaxHP)
 * - Wealth (Coins)
 * - Combat stats (ATK/DEF)
 * - Temporal status (Day/Night)
 * - Equipment (Sword/Armor)
 * * @param hero Pointer to the player structure.
 */
void displayStatusBar(Player* hero) {
    printf(GREEN "==========================================================================\n" RESET);
    // Removed XP and Level mentions
    printf(WHITE " HERO: %-12s  | HP: %d/%d          | COINS: %d\n" RESET, 
            hero->name, hero->hp, hero->maxhp, hero->coins);
    
    // Cleaned up ATK/DEF and simplified STATUS to just show DAY/NIGHT
    printf(WHITE " ATK: %-14d | DEF: %-13d | STATUS: %s\n" RESET, 
            hero->atk, hero->def, 
            hero->isDay ? YELLOW "DAY" RESET : BLUE "NIGHT" RESET);
    printf(GREEN "==========================================================================\n" RESET);

    printf(WHITE " WEAPON: %-12s| ARMOR: %-13s\n" RESET,
            (hero->hasHeroSword ? "Hero Sword" : (hero->hasBaseSword ? "Short Sword" : "None")),
            (hero->hasBaseArmor ? "Leather" : "None"));
    printf(GREEN "==========================================================================\n" RESET);
}

/**
 * @brief Internal logic for the Blacksmith equipment shop.
 * * Implements a "Buy then Upgrade" progression:
 * 1. If item is not owned: Buy for 100c.
 * 2. If item is owned: Sharpen/Reinforce for 50c.
 * Updates the @ref Player structure's atk and def values directly.
 * * @param hero Pointer to the player structure.
 */
static void enterBlacksmith(Player* hero) {
    clear_screen();
    displayStatusBar(hero);
    print_typewriter(YELLOW "\nYou enter the smoky Blacksmith shop." RESET, 30);
    print_typewriter("Gorr: \"Make it quick. I'm busy.\"", 30);

    while(1) {
        // Clear and refresh status every time an action happens
        clear_screen();
        displayStatusBar(hero);
        printf("\n=== Blacksmith ===\n");
        
        if (!hero->hasBaseSword) 
            printf("1. Buy Iron Sword (100c) [+2 Atk]\n");
        else 
            printf("1. Upgrade Sword (50c) [+1 Atk]\n");

        if (!hero->hasBaseArmor)
            printf("2. Buy Leather Armor (100c) [+2 Def]\n");
        else
            printf("2. Reinforce Armor (50c) [+1 Def]\n");

        printf("3. Back\n");
        print_prompt("Choice: ", TEXT_SPEED);

        int c;
        if (scanf("%d", &c) != 1) { while(getchar() != '\n'); continue; }
        while(getchar() != '\n');

        if (c == 3) break;

        if (c == 1) {
            if (!hero->hasBaseSword) {
                if (hero->coins >= 100) {
                    hero->coins -= 100; hero->hasBaseSword = 1; hero->atk += 2;
                    print_typewriter(GREEN "Bought Iron Sword!" RESET, 20);
                } else print_typewriter(RED "Not enough coins!" RESET, 20);
            } else {
                if (hero->coins >= 50) {
                    hero->coins -= 50; hero->atk += 1; hero->upgradeLevelSword++;
                    print_typewriter(GREEN "Sword sharpened!" RESET, 20);
                } else print_typewriter(RED "Not enough coins!" RESET, 20);
            }
        }
        else if (c == 2) {
            if (!hero->hasBaseArmor) {
                if (hero->coins >= 100) {
                    hero->coins -= 100; hero->hasBaseArmor = 1; hero->def += 2;
                    print_typewriter(GREEN "Bought Leather Armor!" RESET, 20);
                } else print_typewriter(RED "Not enough coins!" RESET, 20);
            } else {
                if (hero->coins >= 50) {
                    hero->coins -= 50; hero->def += 1; hero->upgradeLevelArmor++;
                    print_typewriter(GREEN "Armor reinforced!" RESET, 20);
                } else print_typewriter(RED "Not enough coins!" RESET, 20);
            }
        }
        delay_ms(600); // Let the player see the result before clearing
    }
}

/**
 * @brief The main loop for village interaction.
 * * This function handles the "Day/Night" cycle constraints:
 * - **Missions:** Only available at Night.
 * - **Blacksmith:** Only available during the Day.
 * - **Tavern:** Always available (but menu changes internally).
 * - **Waiting:** Allows manual transition from Day to Night.
 * * @param hero Pointer to the player structure.
 */
void enterVillage(Player* hero) {
    clear_screen();
    print_typewriter("\nYou stand in the village square.", 30);
    delay_ms(1500); 


    while (1) {
        clear_screen();
        displayStatusBar(hero);

        if (hero->isDay) printf(YELLOW "\n=== Village Square (DAY) ===\n" RESET);
        else printf(BLUE "\n=== Village Square (NIGHT) ===\n" RESET);

        printf("1. Missions (The Wilds) %s\n", hero->isDay ? "[CLOSED]" : "[OPEN]");
        printf("2. Tavern\n");
        
        if (hero->blacksmithUnlocked)
            printf("3. Blacksmith %s\n", hero->isDay ? "[OPEN]" : "[CLOSED]");
        else
            printf("3. ???\n");
        
        printf("4. Save Game\n");
        
        if (hero->isDay) printf("5. Wait for Nightfall\n");
        else printf("5. [Wait for Day - Go Sleep in Tavern]\n");
        
        printf("6. Quit to Main Menu\n");
        print_prompt("Choose: ", TEXT_SPEED);

        int choice;
        if (scanf("%d", &choice) != 1) { while (getchar() != '\n'); continue; }
        while (getchar() != '\n');

        if (choice == 1) {
            if (hero->isDay) {
                print_typewriter(YELLOW "The sun is too bright. Monsters hide in the shadows." RESET, 30);
                delay_ms(1000);
            } else {
                // Mission Menu Logic
                while(1) {
                    clear_screen();
                    displayStatusBar(hero);
                    printf("\n--- Select Mission (Night) ---\n");
                    printf("1. Rotting Swamp\n");
                    printf("2. %s\n", (hero->maxUnlockedMission >= 2) ? "Haunted Mansion " : "[Locked]");
                    printf("3. %s\n", (hero->maxUnlockedMission >= 3) ? "Crystal Cave " : "[Locked]");
                    if (hero->maxUnlockedMission >= 4) printf(RED "4. THE DARK LORD'S CASTLE [BOSS]\n" RESET);
                    printf("5. Back\n");

                    int mChoice;
                    print_prompt("Choose: ", TEXT_SPEED);
                    if (scanf("%d", &mChoice) != 1) { while(getchar() != '\n'); continue; }
                    while(getchar() != '\n');

                    if (mChoice == 5) break;
                    if (mChoice == 1) runMission(hero, 1);
                    else if (mChoice == 2 && hero->maxUnlockedMission >= 2) runMission(hero, 2);
                    else if (mChoice == 3 && hero->maxUnlockedMission >= 3) runMission(hero, 3);
                    else if (mChoice == 4 && hero->maxUnlockedMission >= 4) {
                        if (hero->hasHeroSword) runMission(hero, 4);
                        else {
                            print_typewriter(RED "You cannot face him without the Hero's Sword!" RESET, 30);
                            delay_ms(1000);
                        }
                    }
                    if (hero->isDay) break; 
                }
            }
        }
        else if (choice == 2) enterTavern(hero);
        else if (choice == 3) {
            if (hero->blacksmithUnlocked) {
                if (hero->isDay) enterBlacksmith(hero);
                else {
                    print_typewriter(RED "The shop is locked. Gorr is sleeping." RESET, 30);
                    delay_ms(1000);
                }
            } else {
                print_typewriter("The door is locked. Maybe ask at the tavern?", 30);
                delay_ms(1000);
            }
        }
        else if (choice == 4) {
            saveGame(hero);
            print_typewriter(GREEN "Progress saved." RESET, 30);
            delay_ms(1000);
        }
        else if (choice == 5) {
            if (hero->isDay) {
                print_typewriter(MAGENTA "You wait by the fire until the sun sets..." RESET, 40);
                hero->isDay = 0;
            } else {
                print_typewriter("You cannot just wait for day here. Go sleep in the Tavern.", 30);
            }
            delay_ms(1000);
        }
        else if (choice == 6) return;
    }
}
