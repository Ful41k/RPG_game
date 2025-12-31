#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "player.h"
#include "color.h"
#include "utils.h"

#define TEXT_SPEED 25

Player createNewHero() {
    Player hero;
    int validName = 0;

    print_typewriter(CYAN "\nDestiny calls out to a new soul..." RESET, 40);
    
    while (!validName) {
        print_prompt("What is the name of this hero? " GREEN, 30); 
        
        if (fgets(hero.name, sizeof(hero.name), stdin) != NULL) {
            size_t len = strlen(hero.name);
            if (len > 0 && hero.name[len - 1] == '\n') {
                hero.name[len - 1] = '\0';
                len--;
            }

            if (len == 0) {
                print_typewriter(RED "A hero cannot be nameless!" RESET, 30);
                continue;
            }

            int onlySpaces = 1;
            for (int i = 0; i < (int)len; i++) {
                if (!isspace(hero.name[i])) {
                    onlySpaces = 0;
                    break;
                }
            }

            if (onlySpaces) {
                print_typewriter(RED "A name of spaces is not enough!" RESET, 30);
            } else {
                validName = 1;
                printf(RESET);
            }
        }
    }

    print_typewriter("\n...", 60);
    print_typewriter("Long ago, when the world was still young, a great knight named " GREEN "Arthur" RESET " the Radiant fought the " RED "Dark Lord" RESET " in the Battle of the Eclipse.", TEXT_SPEED);
    print_typewriter(GREEN "Arthur" RESET " stood alone against the rising shadows, his blade shining like the sun but even he could not defeat the " RED "Dark Lord" RESET ".", TEXT_SPEED);
    print_typewriter(GREEN "Arthur" RESET " sealed the " RED "Dark Lord" RESET " deep within the Obsidian Keep, but the knight was wounded and vanished from history.", TEXT_SPEED);
    print_typewriter("A thousand years passed until now.", TEXT_SPEED);
    print_typewriter("Rumors spread across the land that the seal has begun to weaken. The shadows grow stronger each day.", TEXT_SPEED);
    print_typewriter("And so, fate has led a new hero to the village of Eldoria.", TEXT_SPEED);
    
    char heroIntro[256];
    sprintf(heroIntro, "Your name is %s. You are young, unproven, but something burns within you, a spark of the same light " GREEN "Arthur" RESET " once carried.", hero.name);
    print_typewriter(heroIntro, TEXT_SPEED);
    
    char masterGreet[128];
    sprintf(masterGreet, " I have been expecting you, %s, he says.", hero.name);
    print_typewriter(masterGreet, TEXT_SPEED);
    
    print_typewriter(" My name is " BLUE "Master Edward." RESET " I served " GREEN "Arthur" RESET " long ago and now I will help you walk the same path.", TEXT_SPEED);
    print_typewriter(" Rest in the cabin, gather supplies in the tavern, and when you are ready, enter the " GREEN "Dark Forest" RESET ".", TEXT_SPEED);
    print_typewriter(" The " RED "Dark Lord" RESET " stirs again and only you can stop him.", TEXT_SPEED);
    
    print_typewriter(BLUE "\nMaster Edward: \"Oh, and one more thing.\"" RESET, TEXT_SPEED);
    print_typewriter(BLUE "\"I have spoken to Barnaby at the tavern. Tell him I sent you, and he will give you a bed for free.\"" RESET, TEXT_SPEED);
    print_typewriter(BLUE "\"Rest well, hero. You will need your strength.\"" RESET, TEXT_SPEED);
    print_typewriter("...", 60);

    // Stats Init
    hero.hp = 20; hero.maxhp = 20;
    hero.atk = 2; hero.def = 0; 
    hero.coins = 50; 
    hero.rage = 0; hero.maxRage = 100;
    hero.potionsSmall = 0; hero.potionsBig = 0;
    hero.inventoryCount = 0;
    hero.maxUnlockedMission = 1; 
    hero.hasHeroSword = 0;

    // Time & Flags
    hero.isDay = 1; 
    hero.hasBaseSword = 0;
    hero.hasBaseArmor = 0;
    hero.hasCastleKey = 0;
    hero.upgradeLevelSword = 0;
    hero.upgradeLevelArmor = 0;
    
    hero.knowsSadMan = 0;
    hero.knowsWoman = 0;
    hero.blacksmithUnlocked = 0;
    hero.elaraQuestStatus = 0;
    hero.hangover = 0;
    hero.aleDrankCount = 0;
    hero.introBarnabyDone = 0;

    return hero;
}

void addXp(Player* hero, int amount) {

    }


void addItemToInventory(Player* hero, const char* itemName) {
    if (hero->inventoryCount < INVENTORY_SIZE) {
        strcpy(hero->inventory[hero->inventoryCount], itemName);
        hero->inventoryCount++;
        print_typewriter("Item added to inventory.", 10);
    }
}