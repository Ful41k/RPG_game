#ifndef PLAYER_H
#define PLAYER_H

// player stats
typedef struct {
    char name[30];
    int hp;
    int maxhp;
    int atk;
    int def;
    int coins;

    // shop items
    int potionsSmall;
    int potionsBig;
    int damageBoost;
    
} Player;

Player createNewHero();

#endif
