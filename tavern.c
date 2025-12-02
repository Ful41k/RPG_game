#include <stdio.h>
#include <stdlib.h>

#include "player.h"
#include "tavern.h"
#include "color.h"
#include <time.h>
#include "utils.h"

void tavern(Player* hero) {
    int choice;
    //Tavern Master intro:
    printf("Tavern Master: \"Ahoy, there, lad! Anything yer fancy?\" \n");
     delay_ms(2000);

    while (1) {
        printf("\n===== Tavern Shop =====\n");
        printf("Your coins: %d\n", hero->coins);
        printf("1. Buy Small Healing Potion (5 coins)\n");
        printf("2. Buy Big Healing Potion (12 coins)\n");
        printf("3. Buy Damage Boost (+2 dmg for next fight) - 15 coins\n");
        printf("0. Exit Tavern\n");
        printf("Choose an option: ");

        scanf(" %d", &choice);

        switch (choice) {
            case 1:
                if (hero->coins >= 5) {
                    hero->coins -= 5;
                    // hero->potionsSmall++;
                    addItemToInventory(hero, "Small Potion");
                    printf("Tavern Master: \"Arrr! Patchin' things up yer?\"\n");
                    printf(GREEN "You purchased a Small Potion!\n" RESET);
                } else {
                    printf("Tavern Master: \"Arrr! Sorreh lad! Come back when yer pockets a it heavy!\"\n");
                    printf(RED "Not enough coins!\n" RESET);

                }
                break;

            case 2:
                if (hero->coins >= 12) {
                    hero->coins -= 12;
                    // hero->potionsBig++;
                    addItemToInventory(hero, "Big Potion");
                    printf("Tavern Master: \"Yaharr! Needin' a big one to patch yer up?\"\n");
                    printf(GREEN"You purchased a Big Potion!\n" RESET);
                } else {
                    printf("Tavern Master: \"Ah, ye don’t have enough coins for that, lad!\"\n");
                    printf(RED "Not enough coins!\n" RESET);

                }
                break;

            case 3:
                if (hero->coins >= 15) {
                    hero->coins -= 15;
                    // hero->damageBoost += 2;
                    addItemToInventory(hero, "Damage Boost");
                    printf("Tavern Master: \"Arrr! A mighty fine choice, lad! That’ll have ye fightin’ like a legend in no time!\"\n");
                    printf(GREEN "You feel stronger! (+2 damage on next fight)\n" RESET);
                } else {
                    printf("Tavern Master: \"Yer trynna make meh tavern bankrupt?\"\n");
                    printf(RED "Not enough coins!\n" RESET);
                }
                break;

            case 0:
                printf("Tavern Master: \"Arrr! off with ye, lad! Come back when yer want a drink!\"\n");
                delay_ms(1000);
                printf("Leaving the tavern...\n");
                delay_ms(2000);
                return;

            default:
                printf("Tavern Master: \"EYY yer wastin' my time here lad!\"\n");

        }
    }
}
