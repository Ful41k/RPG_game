/**
 * @file missions.c
 * @brief Logic and flow for all story missions and combat encounters.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "missions.h"
#include "player.h"
#include "color.h"
#include "utils.h"
#include "final.h"
#include "save.h"

#define TEXT_SPEED 20

// Prototypes
static void runRottingSwamp(Player* hero);
static void runHauntedMansion(Player* hero);
static void runCrystalCave(Player* hero);
static int roll_d6(void);
static void showMissionInventory(Player* hero);
static int tryUseHealingPotion(Player* hero);
static void drawBar(const char* label, int current, int max, const char* color);
static void respawnPlayer(Player* hero); 
void displayStatusBar(Player* hero);
void battleNormalFight(Player* hero, const char* name, int fatalStrike, int dmg, int reward, int* killed);

/**
 * @brief Entry point for selecting and starting a mission biome.
 * @param hero Pointer to the player struct.
 * @param missionId The ID of the mission to load.
 */
void runMission(Player* hero, int missionId) {
    int b;
    if (missionId == MISSION_SWAMP) b = 1;
    else if (missionId == MISSION_MANSION) b = 2;
    else if (missionId == MISSION_CAVE) b = 3;
    else b = 4; // Mission 4 is the Dark Lord's Castle

    char* biomeName;
    if (b == 1) biomeName = "Rotting Swamp";
    else if (b == 2) biomeName = "Haunted Mansion";
    else if (b == 3) biomeName = "Crystal Cave";
    else biomeName = "Dark Lord's Castle";

    while (1) {
        printf(YELLOW "\n--- %s Menu ---\n" RESET, biomeName);
        printf("1. Enter Biome (Explore/Quest)\n");
        printf("2. Return to Village\n");
        printf("3. Exit Game\n");
        printf("Choice: ");
        int c;
        if (scanf("%d", &c) != 1) {
            while ( (c = getchar()) != '\n' && c != EOF ); //bug fix: clear invalid input
            continue;
        }
        if (c == 3) {
            return;
        } else if (c == 1) {
            if (b == 1) runRottingSwamp(hero);
            else if (b == 2) runHauntedMansion(hero);
            else if (b == 3) runCrystalCave(hero);
            else {
                // bug fix: final mission
                runFinalMission(hero);
                return;
            }
            // bug fix: check if player died and respawned
            if (hero->isDay == 1) {
                return; // bug fix: Return to village
            }
        } else if (c == 2) {
            // bug fix: Return to village
            if (hero->coins >= 50) {
                hero->coins -= 50;
                printf("\nYou paid 50 coins to safely return to the village.\n");
                waitForEnter();
                return;
            } else {
                printf(RED "\nNot enough coins to return!\n" RESET);
                waitForEnter();
            }
        } else {
            // added special case handling
            printf(RED "\nInvalid choice. Try again.\n" RESET);
            waitForEnter();
        }
    }
}

/**
 * @brief Helper to roll a 6-sided die.
 * @return Integer between 1 and 6.
 */
static int roll_d6(void) { return (rand() % 6) + 1; }

/**
 * @brief Draws a visual progress bar (HP or Rage).
 * @param label Text to display before the bar.
 * @param current Current value.
 * @param max Maximum possible value.
 * @param color ANSI color code for the bar.
 */
static void drawBar(const char* label, int current, int max, const char* color) {
    if (current < 0) current = 0;
    int safeMax = (max <= 0) ? 100 : max; 
    printf("%s: %s[", label, color);
    int segments = 10;
    int filled = (current * segments) / safeMax;
    if (filled > 10) filled = 10;
    for (int i = 0; i < segments; i++) {
        if (i < filled) printf("#");
        else printf("-");
    }
    printf("] %d/%d" RESET "\n", current, max);
}

/**
 * @brief Handles player death, coin penalty, and teleporting back to village.
 * @param hero Pointer to the player struct.
 */
static void respawnPlayer(Player* hero) {
    print_typewriter(RED "\n...Darkness takes you..." RESET, 40);
    print_typewriter(CYAN "But a mysterious force pulls you back from the abyss." RESET, 40);
    print_typewriter(YELLOW "You wake up in the tavern, aching and confused." RESET, 40);
    print_typewriter(".  ", 25);

    hero->hp = 1;
    hero->isDay = 1;

    int amountToLose = hero->coins / 2;
    hero->coins -= amountToLose;

    printf(RED "Penalty: Lost %d Coins.\n" RESET, amountToLose);
    printf(GREEN "\nYou have %d coins remaining.\n" RESET, hero->coins);
    printf("Press Enter to return to the village...");
    fflush(stdout);

    int c;
    while ((c = getchar()) != '\n' && c != EOF); //bug fix: no input buffer
}

/**
 * @brief Displays the mission-specific status and potion inventory.
 * @param hero Pointer to the player struct.
 */
static void showMissionInventory(Player* hero) {
    print_typewriter("\n--- STATUS ---", TEXT_SPEED);
    drawBar("HP  ", hero->hp, hero->maxhp, RED);
    drawBar("RAGE", hero->rage, hero->maxRage, YELLOW);
    if (hero->potionsSmall > 0) printf("1. Healing Potion (%d)\n", hero->potionsSmall);
    printf("0. Back\n");
}

/**
 * @brief Attempts to consume a healing potion from the player's inventory.
 * @param hero Pointer to the player struct.
 * @return 1 if a potion was used, 0 if inventory was empty.
 */
static int tryUseHealingPotion(Player* hero) {
    if (hero->potionsSmall <= 0) { print_typewriter(RED "No potions!" RESET, TEXT_SPEED); return 0; }
    int heal = roll_d6() + 5;
    hero->potionsSmall--;
    hero->hp += heal;
    if (hero->hp > hero->maxhp) hero->hp = hero->maxhp;
    printf(GREEN "Healed %d HP.\n" RESET, heal);
    return 1;
}

/**
 * @brief Core combat logic for a turn-based fight.
 * @param hero Pointer to player.
 * @param name Name of the enemy.
 * @param fatalStrike Number needed on a D6 roll to hit the enemy.
 * @param dmg Damage taken if the player fails the roll.
 * @param reward Coins granted on victory.
 * @param killed Pointer to int flag to notify the mission if enemy died.
 */
void battleNormalFight(Player* hero, const char* name, int fatalStrike, int dmg, int reward, int* killed) {
    *killed = 0;
    printf(RED "\nBATTLE START: %s\n" RESET, name);

    while (hero->hp > 0) {
        printf("\n%s's HP: " RED "%d" RESET " | Potions: " GREEN "%d" RESET " | Coins: " YELLOW "%d" RESET "\n", hero->name, hero->hp, hero->potionsSmall, hero->coins);
        printf("1. Roll Attack (Req: %d+)\n", fatalStrike);
        printf("2. Use Healing Potion\n");
        print_prompt("Choice: ", 0);
        int choice;
        if (scanf("%d", &choice) != 1) { while(getchar() != '\n'); continue; }

        if (choice == 1) {
            int roll = (rand() % 6) + 1; 
            printf(">> You rolled: " YELLOW "%d\n" RESET, roll);

            if (roll >= fatalStrike) {
                printf(GREEN "VICTORY! You defeated the %s!\n" RESET, name);
                hero->coins += reward;
                if (reward > 0) printf(YELLOW "Loot found: %d coins.\n" RESET, reward);
                *killed = 1;
                return; 
            } else {
                int finalDmg = dmg;
                if (hero->hasBaseArmor) finalDmg--; 
                if (finalDmg < 0) finalDmg = 0;
                printf(RED "FAILURE! The %s strikes you for %d damage.\n" RESET, name, finalDmg);
                hero->hp -= finalDmg;
            }
        } 
        else if (choice == 2) {
            if (hero->potionsSmall > 0) {
                tryUseHealingPotion(hero);
            } else {
                printf(RED "You have no potions left!\n" RESET);
            }
        }

        if (hero->hp <= 0) {
            clear_screen();
            hero->hp = 1; 
            hero->isDay = 1;
            respawnPlayer(hero);
            return;
        }
    }
}

/**
 * @brief Helper to pause the game and wait for user input.
 */
void waitForEnter() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF); // Clear buffer
    printf("Press Enter to continue...");
    getchar(); // Wait for actual press
}

/**
 * @brief Mission 2: A 10-room dungeon ending with Lord Vaelren.
 * @param hero Pointer to player.
 */
static void runHauntedMansion(Player* hero) {
    print_typewriter("\n--- Haunted Mansion ---", TEXT_SPEED);
    
    while(1) {
        printf("\n1. Explore the Mansion (Story Dungeon)\n");
        printf("2. Leave\n");
        print_prompt("Choice: ", TEXT_SPEED);

        int c;
        if (scanf("%d", &c) != 1) { while(getchar() != '\n'); continue; }
        if (c == 2) break;
        
        if (c == 1) {
            print_typewriter(YELLOW "\nEntering the Cursed Halls of House Vaelren..." RESET, 30);
            int vampireKilled = 0;

            for (int room = 1; room <= 10; room++) {
                printf(WHITE "\n=== Room %d/10 ===\n" RESET, room);

                if (room == 5) {
                    printf(MAGENTA "\n[!] The air grows cold and tastes of copper and old parchment.\n" RESET);
                    print_typewriter("A creature formed of flickering shadows and broken glass emerges.", 40);
                    print_typewriter("\nKAEL'ZHAR: 'Swear to me, little knight. Swear you will not turn back.'", 50);
                    print_typewriter("'And when you break your word… I will be there to collect what is left of you.'", 50);
                    
                    int killed = 0;
                    battleNormalFight(hero, "Kael'Zhar, the Oath-Eater", 4, 6, 20, &killed);
                    if (hero->isDay == 1) return; // Player died and respawned
                    if (killed) print_typewriter(CYAN "\nThe creature shatters into whispers: 'I shall wait for the crack...'\n" RESET, 30);
                    waitForEnter();
                    continue; 
                }

                if (room == 10) {
                    printf(MAGENTA "\n[!] You reach the grand balcony. A pale figure stares at the dark horizon.\n" RESET);
                    print_typewriter("LORD VAELREN: 'I once stood where you stand. I swore the same oaths.'", 50);
                    print_typewriter("'Arxen took my world and gave me this... hunger.'", 50);
                    print_typewriter("'I guard this key because no mortal can face him and keep their soul intact.'", 50);
                    print_typewriter("'Show me you are more than just a memory of a hero!'", 50);
                    
                    int killed = 0;
                    battleNormalFight(hero, "Lord Vaelren", 5, 4, 100, &killed);
                    if (hero->isDay == 1) return; // bug fix: player death and respawn
                    
                    if (killed) {
                        vampireKilled = 1;
                        hero->hasCastleKey = 1;
                        print_typewriter(GREEN "\nVAELREN: 'So... the light still has teeth. Do not let my sacrifice be in vain.'", 40);
                        print_typewriter(YELLOW "\n>>> YOU HAVE OBTAINED THE CASTLE KEY <<<" RESET, 30);
                    }
                    continue;
                }
                int roll = (rand() % 5) + 1;
                int killed = 0;
                if (roll == 1) { 
                    printf(RED "TRAP: Dark Hatch! You fall and lose 3 HP.\n" RESET);
                    hero->hp -= 3;
                } else if (roll == 2) {
                    battleNormalFight(hero, "Cursed Bat", 2, 2, 5, &killed);
                } else if (roll == 3) {
                    battleNormalFight(hero, "Vaelren Zombie", 3, 2, 10, &killed);
                } else if (roll == 4) {
                    battleNormalFight(hero, "Screaming Phantom", 4, 2, 15, &killed);
                } else {
                    print_typewriter("The mansion whispers your name... but no one is there.", 30);
                }

                if (hero->hp <= 0) {
                    if (hero->isDay == 0) respawnPlayer(hero);
                    return;
                }
                if (hero->isDay == 1) return;
                
                if (room < 10) waitForEnter();
            }
            if (vampireKilled) {
                if (hero->maxUnlockedMission < 3) hero->maxUnlockedMission = 3;
                print_typewriter(GREEN "\nThe Curse of Vaelren is lifted. The Crystal Cave is now open." RESET, 30);
                break; //bug fix: get out of loop
            }
        }
    }
}

/**
 * @brief Determines if a number is part of the Padovan sequence.
 * @param n The number to check.
 * @return 1 if true, 0 if false.
 */
int isPadovan(int n) {
    if (n == 1 || n == 2) return 1;
    int p0 = 1, p1 = 1, p2 = 1, pNext = 0;

    while (pNext <= n) {
        pNext = p0 + p1;
        if (pNext == n) return 1;
        p0 = p1;
        p1 = p2;
        p2 = pNext;
    }
    return 0;
}

/**
 * @brief Mission 3: A 10-room dungeon featuring a math riddle with a dragon.
 * @param hero Pointer to player.
 */
static void runCrystalCave(Player* hero) {
    print_typewriter("\n--- Crystal Cave ---", TEXT_SPEED);
    
    while(1) {
        printf("\n1. Descend into Cave (Story Dungeon)\n");
        printf("2. Leave\n");
        print_prompt("Choice: ", TEXT_SPEED);

        int c;
        if (scanf("%d", &c) != 1) { while(getchar() != '\n'); continue; }
        if (c == 2) break;
        
        if (c == 1) {
            print_typewriter(MAGENTA "\nEntering the Crystal Depths... (10 Rooms)" RESET, 30);
            int dragonDefeated = 0;

            for (int room = 1; room <= 10; room++) {
                printf(WHITE "\n=== Room %d/10 ===\n" RESET, room);
                
                int roll = (room < 10) ? (rand() % 5) + 1 : 6;

                if (roll == 1) {
                    print_typewriter("The room is silent. Only the sound of dripping water...", 30);
                } else if (roll == 2) {
                    printf(RED);
                    print_typewriter("TRAP: Falling Crystals! You take 2 damage.", 30);
                    printf(RESET);
                    hero->hp -= 2;
                } else if (roll == 3) {
                    printf(YELLOW);
                    print_typewriter("TRAP: Unsafe Bridge! You drop 3 coins.", 30);
                    printf(RESET);
                    hero->coins -= 3; if(hero->coins < 0) hero->coins = 0;
                } else if (roll == 4) {
                    print_typewriter(CYAN "You find a Mysterious Chest! Heads or Tails?" RESET, 30);
                    if ((rand() % 2) == 0) {
                        print_typewriter(GREEN "Heads! You found 10 coins." RESET, 30); 
                        hero->coins += 10;
                    } else {
                        print_typewriter(RED "Tails! A mimic! You take 2 damage." RESET, 30); 
                        hero->hp -= 2;
                    }
                } else if (roll == 5) {
                    int d = (rand() % 6) + 1;
                    printf(RED);
                    char trapMsg[64];
                    sprintf(trapMsg, "TRAP: Steep Cliff! You take %d damage.", d);
                    print_typewriter(trapMsg, 30);
                    printf(RESET);
                    hero->hp -= d;
                } else if (roll == 6) {
                    clear_screen();
                    printf(MAGENTA "==========================================================\n");
                    printf("            THE GUARDIAN OF THE SKYWARD           \n");
                    printf("==========================================================\n" RESET);
    
                    print_typewriter("An enormous dragon, scales shimmering like cracked crystals, stirs in the dark.", 40);
                    print_typewriter("This is Seraphron. His eyes reflect a thousand fractured timelines.", 40);
                    print_typewriter("\nSERAPHRON: 'The Void... it tears at the pact. I see the sword...'", 50);
                    print_typewriter("'I see your birth... and your grave... all at once. It does not make SENSE!'", 50);
                    print_typewriter("\nHe claws at his head, his tail smashing pillars of quartz.", 40);
                    print_typewriter("'The patterns! Give me the pattern! Tell me the truth of the sequence!'", 50);


                    int riddleNum = (rand() % 500) + 1;
                    int correctAnswer = isPadovan(riddleNum);

                    printf(MAGENTA);
                    char riddleMsg[128];
                    sprintf(riddleMsg, "\nANCIENT DRAGON: 'Does the number %d belong to the Padovan sequence?'", riddleNum);
                    print_typewriter(riddleMsg, 40);
                    printf(RESET "\n(1: Yes, 0: No): ");
                    
                    int playerAnswer; 
                    if (scanf("%d", &playerAnswer) != 1) { while(getchar() != '\n'); playerAnswer = -1; }

                    if (playerAnswer == correctAnswer) {
                        print_typewriter(GREEN "DRAGON: 'Impressive... Your mind is as sharp as your blade. I shall not strike.'" RESET, 40);
                        dragonDefeated = 1;
                        if (!hero->hasHeroSword) {
                            hero->hasHeroSword = 1;
                            hero->atk += 2;
                            print_typewriter(YELLOW "\n>>> SUCCESS: YOU HAVE OBTAINED THE HERO'S SWORD! (+2 ATK) <<<" RESET, 30);
                        }
                        waitForEnter();
                    } else {
                        print_typewriter(RED "DRAGON: 'FOOL! The patterns of the universe are not for you! FEEL MY BURN!'" RESET, 40);
                        int killed = 0;
                        battleNormalFight(hero, "Ancient Dragon", 5, 10, 12, &killed);
                        if (hero->isDay == 1) return; //bug fix: player death and respawn

                        if (hero->hp > 0 && killed == 1) {
                            dragonDefeated = 1;
                            if (!hero->hasHeroSword) {
                                hero->hasHeroSword = 1;
                                hero->atk += 2;
                                print_typewriter(YELLOW "\n>>> SUCCESS: YOU HAVE OBTAINED THE HERO'S SWORD! (+2 ATK) <<<" RESET, 30);
                            }
                        }
                    }
                }
                if (hero->hp <= 0) {
                    if (hero->isDay == 0) respawnPlayer(hero);
                    return;
                }
                if (hero->isDay == 1) return; 

                if (room < 10) {
                    waitForEnter();
                }
            }

            if (dragonDefeated) {
                if (hero->maxUnlockedMission < 4) hero->maxUnlockedMission = 4;
                print_typewriter(MAGENTA "\nThe path to the Dark Lord's Castle is now open!" RESET, 30);
                break; 
            }
        }
    }
}

/**
 * @brief Mission 1: The Rotting Swamp. 
 * Features shuffled rooms, 3 mini-bosses (Generals), and the Elara Locket side-quest.
 * @param hero Pointer to the player struct.
 */
void runRottingSwamp(Player* hero) {
   while(1) {
    clear_screen(); 
    
    printf(GREEN "--- ROTTING SWAMP MENU ---\n" RESET);
    printf("1. Hunt the Orc Generals (Story Mission)\n");
        
    if (hero->elaraQuestStatus == 1) {
        printf(CYAN "2. Search for the Locket (Side Quest)\n" RESET);
    } else if (hero->elaraQuestStatus == 2) {
        printf(YELLOW "2. [Locket Found - Talk to Elara in Village]\n" RESET);
    }
        
    printf("3. Return to World Map\n");
    print_prompt("Choice: ", 0);

    int menuChoice;
    if (scanf("%d", &menuChoice) != 1) { while(getchar() != '\n'); continue; }

    if (menuChoice == 3) return; // Exit function
    
    if (menuChoice == 1) {
        int generalsDefeated = 0;
        int rooms[10] = {0, 0, 0, 0, 0, 0, 0, 1, 1, 1};

        for (int i = 0; i < 10; i++) {
            int j = rand() % 10;
            int temp = rooms[i];
            rooms[i] = rooms[j];
            rooms[j] = temp;
        }

        clear_screen();
        printf(GREEN "\n--- MISSION 1: THE ROTTING SWAMP ---\n" RESET);
        print_typewriter("You step into the sludge. The air is thick with rot...", 30);
        delay_ms(3000);

        print_typewriter(YELLOW "\nYou enter the territory of the \"Brothers of the Drowned War.\"" RESET, 30);
        print_typewriter("\nThree generals hold this marsh in the Dark Lord's name:", 30);
        
        printf(MAGENTA "\n\n1. GRASH THE BOUND" RESET);
        printf("\n   Wrapped in spectral chains of a broken oath.");
        
        printf(MAGENTA "\n2. VOR'RAK BONEBREAKER" RESET);
        printf("\n   A betrayed guard who wears human helmets as trophies.");
        
        printf(MAGENTA "\n3. MAELA THE ASH-WIND" RESET);
        printf("\n   A hollow shaman who traded her memories to see your future.");
        
        print_typewriter(RED "\n\nDefeat them all to break the Dark Lord's hold on the swamp." RESET, 30);
        
        printf("\n\n(Press Enter to begin the hunt...)");
        while(getchar() != '\n');

        for (int i = 0; i < 10; i++) {
            clear_screen();
            printf(WHITE "\nROOM %d / 10\n" RESET, i + 1);
            int killed = 0;

            if (rooms[i] == 1) { 
                generalsDefeated++;
                clear_screen();

                if (generalsDefeated == 1) {
                    printf(MAGENTA "\n[BOSS] GRASH THE BOUND\n" RESET);
                    printf(CYAN "\"The swamp belongs to the shadows now, as do I!\"\n" RESET);
                    printf("Black iron chains rattle as he raises his corrupted mace.\n");
                    battleNormalFight(hero, "Grash the Bound", 5, 3, 12, &killed);
                } 
                else if (generalsDefeated == 2) {
                    printf(MAGENTA "\n[BOSS] VOR'RAK BONEBREAKER\n" RESET);
                    printf(CYAN "\"I was your shield once... now I am your grave!\"\n" RESET);
                    printf("He points to a human helmet on his belt—it looks like yours.\n");
                    battleNormalFight(hero, "Vor'Rak Bonebreaker", 5, 3, 12, &killed);
                } 
                else {
                    printf(MAGENTA "\n[BOSS] MAELA THE ASH-WIND\n" RESET);
                    printf(CYAN "\"I do not remember my mother's face, but I see your death.\"\n" RESET);
                    printf("The air turns cold as the Void-shaman prepares her spell.\n");
                    battleNormalFight(hero, "Maela the Ash-Wind", 6, 3, 12, &killed);
                }

                // bug fix: check if player death and respawm
                if (hero->isDay == 1) return;

                // MISSION COMPLETE: HOLY FOUNTAIN
                if (hero->hp > 0 && generalsDefeated >= 3) {
                    clear_screen();
                    print_typewriter(CYAN "\n=== THE HOLY FOUNTAIN ===\n" RESET, 30);
                    print_typewriter("The military hold on the swamp is broken.\n", 30);
                    print_typewriter("The fountain's water glows with pure light.\n", 30);
                    print_typewriter("You are healed.\n", 30);
                    hero->hp = hero->maxhp; // Heal the player
                    if (hero->maxUnlockedMission < 2) hero->maxUnlockedMission = 2;
                    saveGame(hero);
                    waitForEnter();
                    break; // Back to swamp menu
                }
            } else { // NORMAL ENCOUNTER OR TRAP
                int roll = (rand() % 6) + 1;
                if (roll == 1) {
                    printf("\nA Poisonous Bog lies ahead. You must cross carefully...");
                    delay_ms(1000);
                    int trapRoll = (rand() % 2); // 50/50 chance
                    if (trapRoll == 0) {
                        int damage = (rand() % 6) + 1;
                        printf(RED "\nYou sank deep! You take %d poison damage.\n" RESET, damage);
                        hero->hp -= damage;
                    } else {
                        printf(GREEN "\nYou leaped across the bog safely!\n" RESET);
                    }
                } else if (roll == 2) battleNormalFight(hero, "Wild Dog", 2, 1, 0, &killed);
                else if (roll == 3) battleNormalFight(hero, "Goblin", 3, 2, 2, &killed);
                else if (roll == 4) battleNormalFight(hero, "Skeleton", 4, 2, 4, &killed);
                else battleNormalFight(hero, "Orc", 4, 4, 6, &killed);
            }

            // bug fix: Check if player died and respawn
            if (hero->isDay == 1) return;

            if (hero->hp <= 0) {
                respawnPlayer(hero);
                return;
            }

            if (i < 9) waitForEnter();
        }
    }

    if (menuChoice == 2 && hero->elaraQuestStatus == 1) {
        clear_screen();
        printf(CYAN "--- QUEST: THE LOST LOCKET ---\n" RESET);
        printf("You venture into a secluded, murky corner of the swamp...\n");
        
        int killed = 0;
        battleNormalFight(hero, "Mud Ghoul", 5, 4, 30, &killed);
        
         // bug fix: Check if player died and respaw
        if (hero->isDay == 1) return;
        
        if (hero->hp > 0 && killed) {
            print_typewriter(GREEN "\nYou found the Locket! Return to the village.\n" RESET, 30);
            hero->elaraQuestStatus = 2;
            waitForEnter();
        } else if (hero->hp <= 0) {
            respawnPlayer(hero);
            return;
        }
    }
   }
}

