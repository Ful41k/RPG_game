#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "final.h"
#include "utils.h"
#include "color.h"

void finalBossBattle(Player* hero);

void runFinalMission(Player* hero) {
    clear_screen();
    
    // CORRECTED ARRAY: Only strings, no numbers.
    const char* whispers[] = {
        "\"I saw the kingdoms burn... I only wanted to put out the fire.\"",
        "\"Freedom is the soil where greed grows. I am merely the gardener.\"",
        "\"The mages were blind. They called it peace; I called it a delay of the inevitable.\"",
        "\"Is it better to be free and dead, or shackled and alive?\"",
        "\"The heartbeat you hear is the rhythm of a world refusing to die quietly.\"",
        "\"Every beast you slew was a soul I 'saved' from the burden of choice.\"",
        "\"You are a leaf in a hurricane, little anomaly. Why fight the wind?\"",
        "\"I have watched you die ten thousand times. This time, I shall not look away.\"",
        "\"You are not a savior; you are the architect of the world's return to chaos.\""
    };

    print_typewriter(RED "=== THE DARK LORD'S CASTLE: HEART OF THE VOID ===" RESET, 30);
    print_typewriter("The air is heavy with the scent of sulfur and cold certainty.", 30);

    for (int room = 1; room <= 10; room++) {
        printf(CYAN "\n[Corridor %d/10] " RESET, room);

        if (room == 10) {
            finalBossBattle(hero);
            return;
        }

        // 1. Logic for Whispers (60% chance)
        int chance = rand() % 100; 

        if (chance < 60) { 
            printf(MAGENTA "\nThe Void echoes: " RESET);
            // This makes the string appear character-by-character
            print_typewriter(whispers[room - 1], 30); 
            // printf(MAGENTA ")" RESET "\n");
        } 
        // 2. Logic for rare Guard attacks (25% chance)
        else if (chance < 85) { 
            print_typewriter(RED "\nA Shadow Guard—a hollow shell of a man—attacks! (-3 HP)" RESET, 20);
            hero->hp -= 3;
        } 
        // 3. Logic for rare help/traps (15% chance)
        else { 
            if (rand() % 2 == 0) {
                print_typewriter(RED "\nA spike trap emerges from the floor! (-2 HP)" RESET, 20);
                hero->hp -= 2;
            } else {
                print_typewriter(YELLOW "\nYou find a pool of starlight. (+5 HP)" RESET, 20);
                hero->hp += 5;
                if (hero->hp > hero->maxhp) hero->hp = hero->maxhp;
            }
        }

        if (hero->hp <= 0) {
            print_typewriter(RED "\nThe Void consumes you. Arxen's certainty prevails." RESET, 40);
            return;
        }
        
        printf("\nPress Enter to move forward...");
        while(getchar() != '\n');
    }
}

void finalBossBattle(Player* hero) {
    clear_screen();
    // Arxen's Description from Documentation
    print_typewriter(MAGENTA "Arxen Voidheart stands before you, wrapped in robes that swallow light." RESET, 40);
    print_typewriter("His eyes glow like distant stars seen through a storm.", 40);
    print_typewriter("\n\"Do you know how many times I have seen you die in the futures I have watched?\"", 40);
    print_typewriter("\"And yet here you are again, little anomaly, asking me to trust chaos over certainty.\"", 40);
    print_typewriter("\"I tore out my heart to save this world from its own greed. What have you sacrificed?\"", 40);

    int heroScore = 0, lordScore = 0;
    // Arxen has 30 HP as per documentation, represented here by 3 Victory Points
    
    while (heroScore < 3 && lordScore < 3) {
        printf(YELLOW "\n--- THE FINAL DUEL (Score: You %d | Arxen %d) ---\n" RESET, heroScore, lordScore);
        printf("1. SHIELD (Rock) | 2. MAGIC (Paper) | 3. SWORD (Scissors)\n");
        print_prompt("Choose your move: ", 20);

        int choice;
        if (scanf("%d", &choice) != 1) { while(getchar() != '\n'); continue; }
        while(getchar() != '\n');

        int lordChoice = (rand() % 3) + 1;
        const char* moves[] = {"", "SHIELD", "MAGIC", "SWORD"};

        printf("\nYou clash with %s! Arxen uses %s!\n", moves[choice], moves[lordChoice]);

        if (choice == lordChoice) {
            print_typewriter("Neither side yields. The Void trembles.", 20);
        } else if ((choice == 1 && lordChoice == 3) || // Shield > Sword
                   (choice == 2 && lordChoice == 1) || // Magic > Shield
                   (choice == 3 && lordChoice == 2)) { // Sword > Magic
            print_typewriter(GREEN "You strike! Proof that hope can overcome control!" RESET, 20);
            heroScore++;
        } else {
            print_typewriter(RED "Arxen's cold certainty strikes you down!" RESET, 20);
            lordScore++;
        }
    }


    // End Screens
    if (heroScore == 3) {
        clear_screen();
        printf(GREEN "**********************************************************\n");
        printf("*             VICTORY: THE WORLD CHOOSES HOPE            *\n");
        printf("**********************************************************\n" RESET);
        print_typewriter("Arxen Voidheart falls. The literal void in his chest closes.", 40);
        print_typewriter("The Orcs, demons, and traps begin to fade as his control breaks.", 40);
        print_typewriter("You have proven that the world can choose its own path and survive.", 60);
    } else {
        clear_screen();
        printf(RED "##########################################################\n");
        printf("#             DEFEAT: THE AGE OF ORDER BEGINS            #\n");
        printf("##########################################################\n" RESET);
        print_typewriter("Arxen sighs as you fall. \"Chaos is finally silent.\"", 40);
        print_typewriter("The world enters an age of cold, eternal certainty.", 60);
        hero->hp = 0;
    }
}