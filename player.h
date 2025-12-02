#ifndef PLAYER_H
#define PLAYER_H

#define INVENTORY_SIZE 10 //set inventory size as 10

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

    //inventory stuff
    char inventory[INVENTORY_SIZE][30]; //inventory array
    int inventoryCount; //counts items in inventory
    
} Player;

Player createNewHero();
void addItemToInventory(Player* hero, const char* itemName);

#endif
