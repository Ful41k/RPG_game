#include <stdio.h>
#include <stdlib.h>
#include "tavern.h"
#include "player.h"
#include "color.h"
#include "utils.h"

#define TEXT_SPEED 20

static void talkToSeller(Player* hero);
static void talkToSadMan(Player* hero);
static void talkToWoman(Player* hero);
static void buyInstantFood(Player* hero);
static void playDiceGame(Player* hero);
static void sleepInTavern(Player* hero);

void enterTavern(Player* hero) {
    if (hero->isDay) {
        // --- ДЕНЬ ---
        hero->aleDrankCount = 0; // Сброс счетчика днем
        print_typewriter(YELLOW "\n[DAY] The tavern is lively." RESET, 30);
        
        while (1) {
            printf("\n=== Tavern (Coins: %d) ===\n", hero->coins);
            printf("1. Seller (Barnaby)\n");
            
            if (hero->knowsSadMan) 
                printf("2. " BLUE "Garrick" RESET "\n");
            else 
                printf("2. " BLUE "Sad Man" RESET "\n");

            if (hero->knowsWoman)
                printf("3. " MAGENTA "Elara" RESET " %s\n", (hero->elaraQuestStatus == 2) ? GREEN "(!)" RESET : "");
            else
                printf("3. " MAGENTA "Woman" RESET "\n");

            printf("4. Leave\n");
            print_prompt("Choose: ", TEXT_SPEED);

            int choice;
            if (scanf("%d", &choice) != 1) { while(getchar() != '\n'); continue; }
            while(getchar() != '\n');

            if (choice == 1) talkToSeller(hero);
            else if (choice == 2) talkToSadMan(hero);
            else if (choice == 3) talkToWoman(hero);
            else if (choice == 4) break;
        }
    } else {
        // --- НОЧЬ ---
        print_typewriter(BLUE "\n[NIGHT] The tavern is quiet. Shadows dance on the walls." RESET, 30);
        
        while(1) {
            printf("\n=== Tavern Night (Coins: %d) ===\n", hero->coins);
            printf("1. Buy Ale/Food\n");
            printf("2. Sleep (FREE - Thanks to Master Edward)\n");
            printf("3. Play Dice with " BLUE "Garrick" RESET "\n");
            printf("4. Leave\n");
            print_prompt("Choose: ", TEXT_SPEED);

            int choice;
            if (scanf("%d", &choice) != 1) { while(getchar() != '\n'); continue; }
            while(getchar() != '\n');

            if (choice == 1) buyInstantFood(hero);
            else if (choice == 2) {
                sleepInTavern(hero);
                break; 
            }
            else if (choice == 3) playDiceGame(hero);
            else if (choice == 4) break;
        }
    }
}

static void talkToSeller(Player* hero) {
    if (hero->introBarnabyDone == 0) {
        print_typewriter(CYAN "\nBarnaby: \"Welcome to The Rusty Tankard. What do you need?\"" RESET, TEXT_SPEED);
        
        printf("1. \"Master Edward sent me.\"\n");
        print_prompt("Choose: ", TEXT_SPEED); 
        
        int temp;
        while(1) {
            if (scanf("%d", &temp) == 1) {
                while(getchar() != '\n'); 
                if (temp == 1) break; 
            } else {
                while(getchar() != '\n'); 
            }
            print_prompt("Please enter 1: ", TEXT_SPEED);
        }

        print_typewriter(CYAN "\nBarnaby: \"Ah! Edward sent you?\"" RESET, TEXT_SPEED);
        print_typewriter(CYAN "Barnaby: \"He is a good man. For his friends, my beds are always free.\"" RESET, TEXT_SPEED);
        print_typewriter(CYAN "Barnaby: \"Eat, drink, and rest well, hero.\"" RESET, TEXT_SPEED);
        
        hero->introBarnabyDone = 1;
        print_typewriter(GREEN "(Feature Unlocked: Free Sleep)" RESET, 20);
    }

    while(1) {
        print_typewriter(CYAN "\nBarnaby: \"Greetings.\"" RESET, TEXT_SPEED);
        printf("1. Buy Supplies (Food/Drink/Potions)\n");
        printf("2. Ask about Blacksmith\n");
        printf("3. Back\n");
        print_prompt("Choice: ", TEXT_SPEED);

        int c;
        if (scanf("%d", &c) != 1) { while(getchar() != '\n'); continue; }
        while(getchar() != '\n');

        if (c == 1) buyInstantFood(hero);
        else if (c == 2) {
            print_typewriter(CYAN "Barnaby: \"Gorr is moody. But he opens shop during the day.\"" RESET, TEXT_SPEED);
            if (!hero->blacksmithUnlocked) {
                hero->blacksmithUnlocked = 1;
                print_typewriter(GREEN ">>> Blacksmith Unlocked! <<<" RESET, 30);
            }
        }
        else if (c == 3) break;
    }
}

static void buyInstantFood(Player* hero) {
    while(1) {
        printf("\n--- Menu ---\n");
        printf("1. Water (2c) -> +5 HP\n");
        printf("2. Bread (10c) -> +15 HP\n");
        printf("3. Roasted Meat (20c) -> +30 HP\n");
        
        int chance = (hero->aleDrankCount == 0) ? 20 : (hero->aleDrankCount == 1 ? 60 : 100);
        printf("4. Ale (5c) -> +10 HP (Risk: %d%%)\n", chance);
        
        if (hero->isDay) printf("5. Healing Potion (50c) -> Inventory\n");
        else printf(RED "5. Potions [LOCKED at Night]\n" RESET);
        
        printf("6. Back\n");
        print_prompt("Buy: ", TEXT_SPEED);

        int c;
        if (scanf("%d", &c) != 1) { while(getchar() != '\n'); continue; }
        while(getchar() != '\n');

        if (c == 6) break;

        if (c == 5) {
            if (!hero->isDay) {
                print_typewriter(CYAN "Barnaby: \"Supply closet is locked till morning.\"" RESET, TEXT_SPEED);
                continue;
            }
            if (hero->coins >= 50) {
                hero->coins -= 50;
                hero->potionsSmall++;
                print_typewriter(GREEN "Potion added to inventory." RESET, TEXT_SPEED);
            } else print_typewriter(RED "Not enough coins." RESET, TEXT_SPEED);
            continue;
        }

        int cost = 0, heal = 0;
        if (c == 1) { cost = 2; heal = 5; }
        if (c == 2) { cost = 10; heal = 15; }
        if (c == 3) { cost = 20; heal = 30; }
        if (c == 4) { cost = 5; heal = 10; }

        if (hero->coins >= cost) {
            hero->coins -= cost;
            hero->hp += heal;
            if (hero->hp > hero->maxhp) hero->hp = hero->maxhp;
            print_typewriter(GREEN "Consumed." RESET, TEXT_SPEED);

            if (c == 4) {
                hero->aleDrankCount++;
                int roll = rand() % 100;
                int threshold = (hero->aleDrankCount == 1) ? 20 : (hero->aleDrankCount == 2 ? 60 : 100);
                
                if (roll < threshold) {
                    print_typewriter(RED "You feel dizzy... Hangover active!" RESET, TEXT_SPEED);
                    hero->hangover = 1;
                } else {
                    print_typewriter(GREEN "Refreshing!" RESET, TEXT_SPEED);
                }
            }
        } else {
            print_typewriter(RED "Not enough coins." RESET, TEXT_SPEED);
        }
    }
}

static void talkToSadMan(Player* hero) {
    while(1) {
        if (hero->knowsSadMan)
            printf(BLUE "\n--- Garrick ---\n" RESET);
        else
            printf(BLUE "\n--- Sad Man ---\n" RESET);

        printf("1. Why are you sad?\n");
        printf("2. What is your name?\n");
        printf("3. Join him for a drink (End Day)\n");
        printf("4. Back\n");
        print_prompt("Choice: ", TEXT_SPEED);

        int c;
        if (scanf("%d", &c) != 1) { while(getchar() != '\n'); continue; }
        while(getchar() != '\n');

        if (c == 1) print_typewriter(BLUE "\"My luck is gone...\"" RESET, TEXT_SPEED);
        else if (c == 2) {
            print_typewriter(BLUE "\"Garrick.\"" RESET, TEXT_SPEED);
            hero->knowsSadMan = 1;
        }
        else if (c == 3) {
            print_typewriter(BLUE "Garrick cheers up. You drink until you pass out." RESET, TEXT_SPEED);
            hero->hangover = 1;
            sleepInTavern(hero);
            return;
        }
        else if (c == 4) break;
    }
}

static void talkToWoman(Player* hero) {
    while(1) {
        if (hero->knowsWoman)
            printf(MAGENTA "\n--- Elara ---\n" RESET);
        else
            printf(MAGENTA "\n--- Woman ---\n" RESET);

        printf("1. Hello\n");
        printf("2. What is your name?\n");
        
        if (hero->elaraQuestStatus == 0)
            printf("3. Can I help you?\n");
        else if (hero->elaraQuestStatus == 1)
            printf("3. About the locket (Quest Active)\n");
        else if (hero->elaraQuestStatus == 2)
            printf("3. I FOUND IT! (Give Locket)\n");
        else
            printf("3. How are you? (Quest Done)\n");

        printf("4. Back\n");
        print_prompt("Choice: ", TEXT_SPEED);

        int c;
        if (scanf("%d", &c) != 1) { while(getchar() != '\n'); continue; }
        while(getchar() != '\n');

        if (c == 1) print_typewriter(MAGENTA "\"Greetings, traveler.\"" RESET, TEXT_SPEED);
        else if (c == 2) {
            print_typewriter(MAGENTA "\"I am Elara.\"" RESET, TEXT_SPEED);
            hero->knowsWoman = 1;
        }
        else if (c == 3) {
            if (hero->elaraQuestStatus == 0) {
                print_typewriter(MAGENTA "Elara: \"I lost my silver locket in the Rotting Swamp.\"" RESET, TEXT_SPEED);
                print_typewriter(MAGENTA "Elara: \"A monster guards it near the Old Willow. Please, find it.\"" RESET, TEXT_SPEED);
                print_typewriter(YELLOW ">>> QUEST STARTED: THE LOST LOCKET <<<" RESET, 30);
                print_typewriter("(Go to Rotting Swamp at NIGHT to search)", 30);
                hero->elaraQuestStatus = 1;
            } else if (hero->elaraQuestStatus == 1) {
                print_typewriter(MAGENTA "Elara: \"Please, hurry. I cannot sleep without it.\"" RESET, TEXT_SPEED);
            } else if (hero->elaraQuestStatus == 2) {
                print_typewriter(GREEN "Elara: \"You found it! Oh, thank you!\"" RESET, TEXT_SPEED);
                print_typewriter(YELLOW "Reward: 150 Coins, 100 XP." RESET, 30);
                hero->coins += 150;
                addXp(hero, 100);
                hero->elaraQuestStatus = 3;
            } else {
                print_typewriter(MAGENTA "Elara: \"You are my hero.\"" RESET, TEXT_SPEED);
            }
        }
        else if (c == 4) break;
    }
}

static void sleepInTavern(Player* hero) {
    print_typewriter(MAGENTA "\nYou sleep soundly in a warm bed..." RESET, 40);
    hero->hp = hero->maxhp;
    hero->isDay = 1; 
    hero->aleDrankCount = 0; 
    
    print_typewriter(YELLOW "Dawn has broken. You feel refreshed." RESET, 30);
}

static void playDiceGame(Player* hero) {
    print_typewriter(BLUE "\nGarrick: \"High roll wins. Ready?\"" RESET, TEXT_SPEED);
    printf("Coins: %d\n", hero->coins);
    printf("Bet: 1. 10c | 2. 50c | 3. 100c | 4. Back\n");
    print_prompt("Bet: ", TEXT_SPEED);
    
    int c;
    if (scanf("%d", &c) != 1) { while(getchar() != '\n'); return; }
    while(getchar() != '\n');

    int bet = 0;
    if (c == 1) bet = 10;
    if (c == 2) bet = 50;
    if (c == 3) bet = 100;
    if (c == 4) return;

    if (hero->coins < bet) {
        print_typewriter(RED "Garrick: \"No coin, no game.\"" RESET, TEXT_SPEED);
        return;
    }

    hero->coins -= bet;
    print_typewriter("Rolling...", 20);
    int pRoll = (rand() % 6 + 1) + (rand() % 6 + 1);
    int gRoll = (rand() % 6 + 1) + (rand() % 6 + 1);
    
    printf(CYAN "You: %d" RESET " vs " BLUE "Garrick: %d" RESET "\n", pRoll, gRoll);
    
    if (pRoll > gRoll) {
        print_typewriter(GREEN "You Win!" RESET, 20);
        hero->coins += (bet * 2);
    } else if (pRoll < gRoll) {
        print_typewriter(RED "Garrick Wins." RESET, 20);
    } else {
        print_typewriter("Draw. Coins returned.", 20);
        hero->coins += bet;
    }
}