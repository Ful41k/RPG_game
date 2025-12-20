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

static void enterBlacksmith(Player* hero) {
    print_typewriter(YELLOW "\nYou enter the smoky Blacksmith shop." RESET, 30);
    print_typewriter("Gorr: \"Make it quick. I'm busy.\"", 30);

    while(1) {
        printf("\n=== Blacksmith (Coins: %d) ===\n", hero->coins);
        printf("Atk: %d | Def: %d\n", hero->atk, hero->def);
        
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

        // --- МЕЧ ---
        if (c == 1) {
            if (!hero->hasBaseSword) {
                if (hero->coins >= 100) {
                    hero->coins -= 100;
                    hero->hasBaseSword = 1;
                    hero->atk += 2;
                    print_typewriter(GREEN "Bought Iron Sword!" RESET, 20);
                } else print_typewriter(RED "Not enough coins!" RESET, 20);
            } else {
                if (hero->coins >= 50) {
                    hero->coins -= 50;
                    hero->atk += 1;
                    hero->upgradeLevelSword++;
                    print_typewriter(GREEN "Sword sharpened!" RESET, 20);
                } else print_typewriter(RED "Not enough coins!" RESET, 20);
            }
        }
        // --- БРОНЯ ---
        else if (c == 2) {
            if (!hero->hasBaseArmor) {
                if (hero->coins >= 100) {
                    hero->coins -= 100;
                    hero->hasBaseArmor = 1;
                    hero->def += 2;
                    print_typewriter(GREEN "Bought Leather Armor!" RESET, 20);
                } else print_typewriter(RED "Not enough coins!" RESET, 20);
            } else {
                if (hero->coins >= 50) {
                    hero->coins -= 50;
                    hero->def += 1;
                    hero->upgradeLevelArmor++;
                    print_typewriter(GREEN "Armor reinforced!" RESET, 20);
                } else print_typewriter(RED "Not enough coins!" RESET, 20);
            }
        }
    }
}

void enterVillage(Player* hero) {
    print_typewriter("\nYou stand in the village square.", 30);

    while (1) {
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
                print_typewriter("Wait for nightfall to hunt.", 30);
            } else {
                while(1) {
                    print_typewriter("\n--- Select Mission (Night) ---", TEXT_SPEED);
                    print_typewriter("1. Rotting Swamp (Lvl 1+)", TEXT_SPEED);
                    
                    if (hero->maxUnlockedMission >= 2) print_typewriter("2. Haunted Mansion (Lvl 3+)", TEXT_SPEED);
                    else print_typewriter("2. [Locked]", TEXT_SPEED);
                    
                    if (hero->maxUnlockedMission >= 3) print_typewriter("3. Crystal Cave (Lvl 5+)", TEXT_SPEED);
                    else print_typewriter("3. [Locked]", TEXT_SPEED);

                    // --- PLUG FINAL MISSION HERE ---
                    if (hero->maxUnlockedMission >= 4) {
                        printf(RED "4. THE DARK LORD'S CASTLE [BOSS]\n" RESET);
                    }

                    print_typewriter("5. Back", TEXT_SPEED); // Changed from 4 to 5
                    print_prompt("Choose: ", TEXT_SPEED);

                    int mChoice;
                    if (scanf("%d", &mChoice) != 1) { while(getchar() != '\n'); continue; }
                    while(getchar() != '\n');

                    if (mChoice == 5) break; // Updated to match new Back button
                    if (mChoice == 1) runMission(hero, 1);
                    if (mChoice == 2 && hero->maxUnlockedMission >= 2) runMission(hero, 2);
                    if (mChoice == 3 && hero->maxUnlockedMission >= 3) runMission(hero, 3);
                    
                    // Trigger for final.c logic
                    if (mChoice == 4 && hero->maxUnlockedMission >= 4) {
                        if (hero->hasHeroSword) {
                            runMission(hero, 4); // MISSION_DARK_LORD is 4
                        } else {
                            print_typewriter(RED "You cannot face him without the Hero's Sword!" RESET, 30);
                        }
                    }
                    
                    if (hero->isDay) break; 
                }
            }






            /* else {
                while(1) {
                    print_typewriter("\n--- Select Mission (Night) ---", TEXT_SPEED);
                    print_typewriter("1. Rotting Swamp (Lvl 1+)", TEXT_SPEED);
                    
                    if (hero->maxUnlockedMission >= 2) print_typewriter("2. Haunted Mansion (Lvl 3+)", TEXT_SPEED);
                    else print_typewriter("2. [Locked]", TEXT_SPEED);
                    
                    if (hero->maxUnlockedMission >= 3) print_typewriter("3. Crystal Cave (Lvl 5+)", TEXT_SPEED);
                    else print_typewriter("3. [Locked]", TEXT_SPEED);

                    print_typewriter("4. Back", TEXT_SPEED);
                    print_prompt("Choose: ", TEXT_SPEED);

                    int mChoice;
                    if (scanf("%d", &mChoice) != 1) { while(getchar() != '\n'); continue; }
                    while(getchar() != '\n');

                    if (mChoice == 4) break;
                    if (mChoice == 1) runMission(hero, 1);
                    if (mChoice == 2 && hero->maxUnlockedMission >= 2) runMission(hero, 2);
                    if (mChoice == 3 && hero->maxUnlockedMission >= 3) runMission(hero, 3);
                    
                    if (hero->isDay) break; 
                }
            } */
        }
        else if (choice == 2) enterTavern(hero);
        else if (choice == 3) {
            if (hero->blacksmithUnlocked) {
                if (hero->isDay) enterBlacksmith(hero);
                else print_typewriter(RED "The shop is locked. Gorr is sleeping." RESET, 30);
            } else {
                print_typewriter("The door is locked. Maybe ask at the tavern?", 30);
            }
        }
        else if (choice == 4) {
            saveGame(hero);
            print_typewriter(GREEN "Progress saved." RESET, 30);
        }
        else if (choice == 5) {
            if (hero->isDay) {
                print_typewriter(MAGENTA "You wait by the fire until the sun sets..." RESET, 40);
                print_typewriter(BLUE "Night has fallen. The beasts awaken." RESET, 40);
                hero->isDay = 0;
            } else {
                print_typewriter("You cannot just wait for day here. Go sleep in the Tavern.", 30);
            }
        }
        else if (choice == 6) return;
        else if (choice == 9) runCheatMenu(hero);
    }
}