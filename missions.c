#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "missions.h"
#include "color.h"
#include "utils.h"
#include "final.h"

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

static void battleNormalFight(Player* hero, const char* enemyName, int fatalStrike, int enemyDamage, int coinsReward, int xpReward, int* killedEnemy);

void runMission(Player* hero, int missionId) {
    // ... (keep your existing hangover logic) ...
    
    switch (missionId) {
        case MISSION_SWAMP:   runRottingSwamp(hero); break;
        case MISSION_MANSION: runHauntedMansion(hero); break;
        case MISSION_CAVE:    runCrystalCave(hero); break;
        
        case MISSION_DARK_LORD: 
            runFinalMission(hero); 
            break; 
            
        default: 
            print_typewriter("Unknown mission.", TEXT_SPEED); 
            break;
    }
}


/* void runMission(Player* hero, int missionId) {
    if (hero->hangover) {
        print_typewriter(RED "You have a massive hangover! (-2 Atk)" RESET, 30);
    }

    switch (missionId) {
        case MISSION_SWAMP:     runRottingSwamp(hero); break;
        case MISSION_MANSION:   runHauntedMansion(hero); break;
        case MISSION_CAVE:      runCrystalCave(hero); break;
        default: print_typewriter("Unknown mission.", TEXT_SPEED); break;
    }

    if (hero->hangover) {
        hero->hangover = 0;
        print_typewriter(GREEN "Your hangover has faded." RESET, 30);
    }
} */

// --- UTILS ---
static int roll_d6(void) { return (rand() % 6) + 1; }

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

static void respawnPlayer(Player* hero) {
    print_typewriter(RED "\n...Darkness takes you..." RESET, 40);
    print_typewriter(CYAN "But a mysterious force pulls you back from the abyss." RESET, 40);
    print_typewriter(YELLOW "You wake up in the tavern, aching and confused." RESET, 40);
    
    hero->hp = 1;
    hero->isDay = 1; 
    
    int lostCoins = hero->coins / 2;
    hero->coins -= lostCoins;
    
    int lostXp = hero->xpToNextLevel / 10; 
    if (hero->xp > lostXp) hero->xp -= lostXp;
    else hero->xp = 0;

    printf(RED "Penalty: Lost %d Coins and some XP.\n" RESET, lostCoins);
    print_typewriter("Press any key to recover...", 0);
    getchar();
}

static void showMissionInventory(Player* hero) {
    print_typewriter("\n--- STATUS ---", TEXT_SPEED);
    drawBar("HP  ", hero->hp, hero->maxhp, RED);
    drawBar("RAGE", hero->rage, hero->maxRage, YELLOW);
    if (hero->potionsSmall > 0) printf("1. Healing Potion (%d)\n", hero->potionsSmall);
    printf("0. Back\n");
}

static int tryUseHealingPotion(Player* hero) {
    if (hero->potionsSmall <= 0) { print_typewriter(RED "No potions!" RESET, TEXT_SPEED); return 0; }
    int heal = roll_d6() + 5;
    hero->potionsSmall--;
    hero->hp += heal;
    if (hero->hp > hero->maxhp) hero->hp = hero->maxhp;
    printf(GREEN "Healed %d HP.\n" RESET, heal);
    return 1;
}

// --- BATTLE SYSTEM ---
static void battleNormalFight(Player* hero, const char* enemyName, int fatalStrike, int enemyDamage, int coinsReward, int xpReward, int* killedEnemy) {
    *killedEnemy = 0;
    int enemyHP = fatalStrike;
    
    int effectiveAtk = hero->atk;
    if (hero->hangover) effectiveAtk -= 2;
    if (effectiveAtk < 1) effectiveAtk = 1;

    print_typewriter(RED "Battle Start!" RESET, TEXT_SPEED);

    while (hero->hp > 0 && enemyHP > 0) {
        printf("\n");
        drawBar(hero->name, hero->hp, hero->maxhp, GREEN);
        drawBar("RAGE", hero->rage, hero->maxRage, YELLOW);
        printf("ENEMY: %s (%d HP)\n", enemyName, enemyHP);
        
        print_typewriter("1. Attack | 2. Block | 3. Skill | 4. Item", TEXT_SPEED);
        print_prompt("Act: ", TEXT_SPEED);
        int c;
        if (scanf("%d", &c) != 1) { while(getchar() != '\n'); continue; }

        if (c == 1) { // Attack
            int roll = roll_d6();
            int dmg = roll + effectiveAtk - 2; 
            if (dmg < 1) dmg = 1;
            if (roll == 6) { dmg *= 2; printf(YELLOW "CRITICAL!\n" RESET); }
            enemyHP -= dmg;
            hero->rage += 12;
            printf("You hit for %d dmg.\n", dmg);
        }
        else if (c == 2) { // Block
            hero->rage += 8;
            printf("Blocking stance.\n");
        }
        else if (c == 3) { // Skill
             if (hero->rage >= 30) {
                int dmg = (roll_d6() + effectiveAtk) * 2;
                enemyHP -= dmg;
                hero->rage -= 30;
                printf(MAGENTA "HEAVY STRIKE: %d dmg!\n" RESET, dmg);
             } else printf(RED "Need 30 Rage.\n" RESET);
        }
        else if (c == 4) { // Item
            showMissionInventory(hero);
            int i; scanf("%d", &i);
            if (i == 1) tryUseHealingPotion(hero);
            continue;
        }

        if (enemyHP <= 0) {
            print_typewriter(GREEN "Victory!" RESET, TEXT_SPEED);
            hero->coins += coinsReward; addXp(hero, xpReward);
            *killedEnemy = 1;
            return;
        }

        int dmgTaken = enemyDamage - hero->def;
        if (c == 2) dmgTaken /= 2;
        if (dmgTaken < 0) dmgTaken = 0;
        
        hero->hp -= dmgTaken;
        printf("%s hits you for %d.\n", enemyName, dmgTaken);

        if (hero->hp <= 0) {
            respawnPlayer(hero);
            return;
        }
    }
}

// ... (keep your prototypes and existing helper functions) ...

static void runRottingSwamp(Player* hero) {
    print_typewriter("\n--- Rotting Swamp ---", TEXT_SPEED);
    
    while(1) {
        printf("\n1. Hunt Monsters\n");
        if (hero->elaraQuestStatus == 1) 
            printf(CYAN "2. Search for the Locket (QUEST)\n" RESET);
        printf("3. Leave\n");
        print_prompt("Choice: ", TEXT_SPEED);

        int c;
        if (scanf("%d", &c) != 1) { while(getchar() != '\n'); continue; }

        if (c == 3) break;
        
        if (c == 1) {
            int killed = 0;
            battleNormalFight(hero, "Swamp Stalker", 10, 3, 10, 20, &killed);
            
            // UNLOCK NEXT MISSION logic
            if (killed && hero->hp > 1) {
                if (hero->maxUnlockedMission < 2) {
                    hero->maxUnlockedMission = 2;
                    print_typewriter(GREEN "\nYou have cleared the swamp! The Haunted Mansion is now accessible." RESET, 30);
                }
            }
            if (hero->hp <= 1) return; 
        }
        else if (c == 2 && hero->elaraQuestStatus == 1) {
            // ... (keep your existing Locket quest logic) ...
            int killed = 0;
            battleNormalFight(hero, "Mud Ghoul", 25, 5, 50, 50, &killed);
            
            if (hero->hp > 1 && killed) {
                print_typewriter(GREEN "\nYou found the Silver Locket!" RESET, 30);
                hero->elaraQuestStatus = 2; 
                // Optional: Clear swamp also unlocks mission 2
                if (hero->maxUnlockedMission < 2) hero->maxUnlockedMission = 2;
            }
            if (hero->hp <= 1) return;
        }
    }
}

static void runHauntedMansion(Player* hero) {
    print_typewriter("\n--- Haunted Mansion ---", TEXT_SPEED);
    int killed = 0;
    battleNormalFight(hero, "Cursed Armor", 20, 4, 20, 40, &killed);

    // UNLOCK NEXT MISSION logic
    if (killed && hero->hp > 1) {
        if (hero->maxUnlockedMission < 3) {
            hero->maxUnlockedMission = 3;
            print_typewriter(GREEN "\nThe Curse is lifted! The path to the Crystal Cave is open." RESET, 30);
        }
    }
}

static void runCrystalCave(Player* hero) {
    print_typewriter("\n--- Crystal Cave ---", TEXT_SPEED);
    int killed = 0;
    battleNormalFight(hero, "Obsidian Shard", 30, 6, 40, 60, &killed);

    if (killed && hero->hp > 1) {
        print_typewriter(YELLOW "\nYou found the legendary Hero's Sword!" RESET, 30);
        hero->hasHeroSword = 1; 
        
        if (hero->maxUnlockedMission < 4) {
            hero->maxUnlockedMission = 4; 
            print_typewriter(MAGENTA "A dark path opens in the distance... The Dark Lord awaits." RESET, 30);
        }
    }
}






/*
static void runRottingSwamp(Player* hero) {
    print_typewriter("\n--- Rotting Swamp ---", TEXT_SPEED);
    
    while(1) {
        printf("\n1. Hunt Monsters\n");
        if (hero->elaraQuestStatus == 1) 
            printf(CYAN "2. Search for the Locket (QUEST)\n" RESET);
        printf("3. Leave\n");
        print_prompt("Choice: ", TEXT_SPEED);

        int c;
        if (scanf("%d", &c) != 1) { while(getchar() != '\n'); continue; }

        if (c == 3) break;
        
        if (c == 1) {
            int killed = 0;
            battleNormalFight(hero, "Swamp Stalker", 10, 3, 10, 20, &killed);
            if (hero->hp <= 1) return; 
        }
        else if (c == 2 && hero->elaraQuestStatus == 1) {
            print_typewriter(CYAN "\nYou search near the Old Willow..." RESET, 30);
            print_typewriter(RED "A Mud Ghoul rises from the filth!" RESET, 30);
            
            int killed = 0;
            battleNormalFight(hero, "Mud Ghoul", 25, 5, 50, 50, &killed);
            
            if (hero->hp > 1 && killed) {
                print_typewriter(GREEN "\nYou found the Silver Locket in the mud!" RESET, 30);
                hero->elaraQuestStatus = 2; 
            }
            if (hero->hp <= 1) return;
        }
    }
}

static void runHauntedMansion(Player* hero) {
    print_typewriter("\n--- Haunted Mansion ---", TEXT_SPEED);
    int killed = 0;
    battleNormalFight(hero, "Cursed Armor", 20, 4, 20, 40, &killed);
}

static void runCrystalCave(Player* hero) {
    print_typewriter("\n--- Crystal Cave ---", TEXT_SPEED);
    
    int killed = 0;
    battleNormalFight(hero, "Obsidian Shard", 30, 6, 40, 60, &killed);

    if (killed && hero->hp > 1) {
        print_typewriter(YELLOW "\nYou found the legendary Hero's Sword!" RESET, 30);
        hero->hasHeroSword = 1; 
        
        if (hero->maxUnlockedMission < 4) {
            hero->maxUnlockedMission = 4; 
            print_typewriter(MAGENTA "A dark path opens in the distance... The Castle awaits." RESET, 30);
        }
    }
} */