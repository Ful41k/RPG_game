#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "player.h"
#include "village-menu.h"
#include "utils.h"
#include "color.h"


void rest(Player* hero) {
    printf("\nYou take a rest at the cabin...\n");
    printf("Recovering HP");

    // Simple rest animation
    for (int i = 0; i < 6; i++) {
        printf(".");
        fflush(stdout);   // execute very fast
        #ifdef _WIN32     // For windows 500 milisec, for MacOS/Linux 1200 milisec
            delay_ms(500);
        #else
            delay_ms(1200);
        #endif
    }

    hero->hp = hero->maxhp;  // fully restore HP

    printf("\nYou feel refreshed! Your HP is now full (%d/%d).\n",
           hero->hp, hero->maxhp);
}

// Tavern option
void tavern(Player* hero) {
    int choice;

    while (1) {
        printf("\n===== Tavern Shop =====\n");
        printf("Your coins: %d\n", hero->coins);
        printf("1. Buy Small Healing Potion (5 coins)\n");
        printf("2. Buy Big Healing Potion (12 coins)\n");
        printf("3. Buy Damage Boost (+2 dmg for next fight) - 15 coins\n");
        printf("0. Exit Tavern\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (hero->coins >= 5) {
                    hero->coins -= 5;
                    hero->potionsSmall++;
                    printf("You purchased a Small Potion!\n");
                } else {
                    printf("Not enough coins!\n");
                }
                break;

            case 2:
                if (hero->coins >= 12) {
                    hero->coins -= 12;
                    hero->potionsBig++;
                    printf("You purchased a Big Potion!\n");
                } else {
                    printf("Not enough coins!\n");
                }
                break;

            case 3:
                if (hero->coins >= 15) {
                    hero->coins -= 15;
                    hero->damageBoost += 2;
                    printf("You feel stronger! (+2 damage on next fight)\n");
                } else {
                    printf("Not enough coins!\n");
                }
                break;

            case 0:
                printf("Leaving the tavern...\n");
                return;

            default:
                printf("Invalid choice!\n");
        }
    }
}

void startMission(Player* hero) {
    printf("\nMission system not ready yet. Placeholder.\n");
}

// The story where it begins 
void villageIntro(Player* hero) {
	printf("Long ago, when the world was still young, a great knight named " GREEN "Arthur" RESET " the Radiant fought the " RED "Dark Lord" RESET " in the Battle of the Eclipse.\n");
	delay_ms(7000);
	printf(GREEN "Arthur" RESET " stood alone against the rising shadows, his blade shining like the sun… but even he could not defeat the Dark Lord.\n");
	delay_ms(7000);
	printf(GREEN "Arthur" RESET " sealed the " RED "Dark Lord" RESET " deep within the Obsidian Keep, but the knight was gravely wounded and vanished from history.\n");
	delay_ms(7000);
	printf("A thousand years passed — until now.\n");
	delay_ms(5000);
	printf("Rumors spread across the land that the seal has begun to weaken. The shadows grow stronger each day.\n");
	delay_ms(7000);
	printf("And so, fate has led a new hero to the village of Eldoria…\n");
	delay_ms(3000);
	printf("Your name is %s. You are young, unproven, but something burns within you — a spark of the same light " GREEN "Arthur" RESET " once carried.\n", hero->name);
	delay_ms(7000);
	printf(" -I’ve been expecting you, %s,- he says.\n", hero->name);
	delay_ms(4000);
	printf(" -My name is " BLUE " Master Edward. " RESET " I served " GREEN "Arthur" RESET " long ago… and now I will help you walk the same path.\n");
	delay_ms(8000);
	printf(" -Rest in the cabin, gather supplies in the tavern, and when you are ready — enter the Dark Forest.\n The " RED "Dark Lord" RESET " stirs again… and only you can stop him.\n");
	delay_ms(5000);
}

// village menu
void villageMenu(Player* hero) {
     int choice;

    while (1) {
        printf("\n=== Eldoria Village ===\n");
        printf("1. Knight's Cabin (Rest)\n");
        printf("2. Tavern (Shop)\n");
        printf("3. Dark Forest (Mission)\n");
        printf("4. Leave Game\n");
        printf("Choose: ");

        scanf("%d", &choice);

        switch(choice) {
            case 1: rest(hero); break;
            case 2: tavern(hero); break;
            case 3: startMission(hero); return;
            case 4: exit(0);
            default: printf("Invalid.\n");
        }
    }


}

