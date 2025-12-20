#ifndef MISSIONS_H
#define MISSIONS_H

#include "player.h"

#define MISSION_SWAMP 1
#define MISSION_MANSION 2
#define MISSION_CAVE 3
#define MISSION_DARK_LORD 4

void runMission(Player* hero, int missionId);

#endif
