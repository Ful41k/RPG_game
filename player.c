#include <stdio.h>
#include <string.h>

#include "player.h"
#include "color.h"

Player createNewHero() {
    Player hero;                  // Creating the Player variable

    printf("Enter your hero's name: ");
    // Read name from user input (up to 29 characters)
    fgets(hero.name, sizeof(hero.name), stdin); 
    // Taking the user`s name

    size_t len = strlen(hero.name);
    if (len > 0 && hero.name[len - 1] == '\n') {
    hero.name[len - 1] = '\0';
    }
    // cutting the \n from fgets(); 

    hero.hp = 20;                 // Set default life points
    hero.maxhp = 20;              // restore hp
    hero.atk = 5;                 // Default attack
    hero.def = 2;                 // Default defense
    hero.coins = 0;               // Start with 0 coins

    //shop items
    // hero.potionsSmall = 0;
    // hero.potionsBig = 0;
    // hero.damageBoost = 0;



    return hero;                  // 3. Return the hero
}

//inventory counting system:
void addItemToInventory(Player* hero, const char* itemName) {
    if (hero->inventoryCount < INVENTORY_SIZE) {
        strncpy(hero->inventory[hero->inventoryCount], itemName, 29);
        hero->inventory[hero->inventoryCount][29] = '\0';
        hero->inventoryCount++;
    } else {
        printf(RED "Inventory full! You left the item %s!\n" RESET, itemName);
    }
}