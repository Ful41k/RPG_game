#include <stdio.h>
#include <ctype.h>
#include "cheats.h"

/* Сам Konami-код: w w s s a d a d b a [space] */
static const char KONAMI_CODE[KONAMI_LENGTH] = {
    'w','w','s','s','a','d','a','d','b','a',' '
};

void initCheatState(CheatState *state) { //* это типо ключ к Cheatstate
    if (!state) return;
    state->cheatsUnlocked = 0;
    state->konamiIndex = 0;
}

/*
 * Каждый раз, когда пользователь что-то вводит в Initial Game Menu,
 * передаем сюда введённый символ (ch).
 * Функция будет двигать konamiIndex и,
 * если вся последовательность введена правильно – включит cheatsUnlocked.
 */
void processKonamiInput(CheatState *state, int ch) {
    if (!state) return;

    if (state->cheatsUnlocked) {
        return; // уже открыт, дальше можно не проверять
    }

    if (ch == KONAMI_CODE[state->konamiIndex]) {
        state->konamiIndex++;

        if (state->konamiIndex == KONAMI_LENGTH) {
            state->cheatsUnlocked = 1;
            state->konamiIndex = 0;
            printf("\n*** CHEATS UNLOCKED! ***\n\n");
        }
    } else {
        // Сбросить прогресс и проверить, не является ли ch началом новой попытки
        state->konamiIndex = 0;
        if (ch == KONAMI_CODE[0]) {
            state->konamiIndex = 1;
        }
    }
}

/* Вспомогательная функция: очистить остатки строки после scanf */
static void clearLine(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
}

void showCheatsMenu(Save *saves, int saveCount) {
    if (!saves || saveCount <= 0) {
        printf("No saved games available for cheats.\n");
        return;
    }

    printf("Saved Games:\n"); //последние сохр игры
    for (int i = 0; i < saveCount; ++i) {
        printf("%d. %d HP, %d coins, %d items, %d completed missions, final mission %s\n",
               i + 1,
               saves[i].lifePoints,
               saves[i].coins,
               saves[i].itemsCount,
               saves[i].completedMissions,
               saves[i].finalMissionUnlocked ? "UNLOCKED" : "LOCKED");
    }

    int index = 0;

    printf("Select a save to modify [1-%d]: ", saveCount);
    if (scanf("%d", &index) != 1) {
        clearLine();
        printf("Invalid input.\n");
        return;
    }
    clearLine();

    if (index < 1 || index > saveCount) {
        printf("Invalid save index.\n");
        return;
    }

    Save *s = &saves[index - 1];

    int choice;
    do {
        printf("\nCheats Menu (Save #%d):\n", index);
        printf("1. Edit Life Points\n");
        printf("2. Edit Coins\n");
        printf("3. Unlock Final Mission\n");
        printf("4. Back\n");
        printf("Choose an action [1-4]: ");

        if (scanf("%d", &choice) != 1) {
            clearLine();
            printf("Invalid input.\n");
            continue;
        }
        clearLine();

        switch (choice) {
            case 1: {
                int newHP;
                printf("Enter new Life Points (0-20): ");
                if (scanf("%d", &newHP) != 1) {
                    clearLine();
                    printf("Invalid input.\n");
                    break;
                }
                clearLine();
                if (newHP < 0) newHP = 0;
                if (newHP > 20) newHP = 20;
                s->lifePoints = newHP;
                printf("Life Points updated to %d.\n", s->lifePoints);
                break;
            }
            case 2: {
                int newCoins;
                printf("Enter new Coins (>= 0): ");
                if (scanf("%d", &newCoins) != 1) {
                    clearLine();
                    printf("Invalid input.\n");
                    break;
                }
                clearLine();
                if (newCoins < 0) newCoins = 0;
                s->coins = newCoins;
                printf("Coins updated to %d.\n", s->coins);
                break;
            }
            case 3:
                s->finalMissionUnlocked = 1;
                printf("Final mission UNLOCKED for this save.\n");
                break;

            case 4:
                printf("Returning from Cheats Menu.\n");
                break;

            default:
                printf("Invalid choice.\n");
                break;
        }

    } while (choice != 4);
}
