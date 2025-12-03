#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "player.h"
#include "tavern.h"
#include "village-menu.h"
#include "utils.h"
#include "color.h"

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

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


void startMission(Player* hero) {
    printf("\nMission system not ready yet. Placeholder.\n");
}

// The story where it begins 
void villageIntro(Player* hero) {
	printf("Long ago, when the world was still young, a great knight named " GREEN "Arthur" RESET " the Radiant fought the " RED "Dark Lord" RESET " in the Battle of the Eclipse.\n");
	delay_ms(5000);
	printf(GREEN "Arthur" RESET " stood alone against the rising shadows, his blade shining like the sun… but even he could not defeat the Dark Lord.\n");
	delay_ms(5000);
	printf(GREEN "Arthur" RESET " sealed the " RED "Dark Lord" RESET " deep within the Obsidian Keep, but the knight was gravely wounded and vanished from history.\n");
	delay_ms(5000);
	printf("A thousand years passed — until now.\n");
	delay_ms(4000);
	printf("Rumors spread across the land that the seal has begun to weaken. The shadows grow stronger each day.\n");
	delay_ms(5000);
	printf("And so, fate has led a new hero to the village of Eldoria…\n");
	delay_ms(3000);
	printf("Your name is %s. You are young, unproven, but something burns within you — a spark of the same light " GREEN "Arthur" RESET " once carried.\n", hero->name);
	delay_ms(6000);
	printf(" -I’ve been expecting you, %s,- he says.\n", hero->name);
	delay_ms(4000);
	printf(" -My name is " BLUE " Master Edward. " RESET " I served " GREEN "Arthur" RESET " long ago… and now I will help you walk the same path.\n");
	delay_ms(6000);
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
        clearInputBuffer();

        switch(choice) {
            case 1: rest(hero); break;
            case 2: tavern(hero); break;
            case 3: startMission(hero); break;
            case 4: printf("Leaving Village... Goodbye!\n"); delay_ms(2000); return; //exit(0);
            default: printf("Invalid.\n");
        }
    }


}
