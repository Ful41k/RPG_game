#ifndef SAVE_H
#define SAVE_H

#include "player.h"

typedef struct {
    char name[50];
    int lifePoints;
    int level;
    int hp;
    int coins;
    int itemsCount;
    int completedMissions;
    int finalMissionUnlocked; // 0 = no, 1 = final mission unlocked
} Save;

void saveGame(Player* hero);
void loadGameMenu();

#endif
