#include "techTree.h"
#include <stdio.h>
#include <string.h>
#include "../game.h"

/*

Piece Upgrades


Building upgrades
- increased gold income

Generic/ random upgrades
- 25% to gain half captured piece value
- upgrade platforms

*/
/*
pre-requisites to add

**Queen**
Needs Rook and Catapult

**Rook**
Needs Knight

**Bishop**
Needs Knight

**Knight**
Needs Pawn (Start)

**Catapult**
Needs Mage and Rook

**Mage**
Needs Lancer and Bishop

**Lancer**
Needs Envoy and Knight

**Envoy**
Needs Pawn(Start)

**Extra Money from Towns**
Needs Pawn(Start)

**Extra set of pieces**
Needs Extra Money from Towns

**Upgrade Platform III**
Needs Upgrade Platform II

**Upgrade Platform II**
Needs Upgrade Platform I

**Upgrade Platform I**
Needs Pawn (Start)


*/

void initTechTree(TechTree *tree)
{
    strcpy(tree->upgrades[0].name, "knight");
    tree->upgrades[0].cost = 5;
    tree->upgrades[0].required = -1;

    strcpy(tree->upgrades[1].name, "bishop");
    tree->upgrades[1].cost = 8;
    tree->upgrades[1].required = 0;

    strcpy(tree->upgrades[2].name, "rook");
    tree->upgrades[2].cost = 8;
    tree->upgrades[2].required = 1;

    strcpy(tree->upgrades[3].name, "queen");
    tree->upgrades[3].cost = 10;
    tree->upgrades[3].required = -1;

    strcpy(tree->upgrades[4].name, "Envoy");
    tree->upgrades[4].cost = 15;
    tree->upgrades[4].required = 3;
    
    strcpy(tree->upgrades[5].name, "Lancer");
    tree->upgrades[5].cost = 5;
    tree->upgrades[5].required = 0;

    strcpy(tree->upgrades[6].name, "Mage");
    tree->upgrades[6].cost = 12;
    tree->upgrades[6].required = 0;

    strcpy(tree->upgrades[7].name, "Catapult");
    tree->upgrades[7].cost = 12;
    tree->upgrades[7].required = 0;

    strcpy(tree->upgrades[8].name, "UPG PLAT II");
    tree->upgrades[8].cost = 60;
    tree->upgrades[8].required = 0;

    strcpy(tree->upgrades[9].name, "UPG PLAT III");
    tree->upgrades[9].cost = 60;
    tree->upgrades[9].required = 0;

    for (int i = 0; i < MAX_UPGRADES; i++)tree->upgrades[i].unlocked = 0;
    
}

// In techTree.c
bool unlockUpgrade(TechTree *tree, int index, int *gold)
{
    Upgrade *u = &tree->upgrades[index];

    if (u->required != -1 && !tree->upgrades[u->required].unlocked) return false;
    if (u->unlocked) return false;

    if (*gold >= u->cost)
    {
        *gold -= u->cost;
        u->unlocked = 1;
        printf("Unlocked: %s\n", u->name);
        return true; // Success!
    }
    return false;
}

void printTechTree(AppState *app)
{
    // 1. Identify which tree to display
    TechTree *tree = (app->currentPlayer == 1) ? &app->techTreeP1 : &app->techTreeP2;

    SDL_FRect titleRect = {900, 50, 400, 60};
    drawText(app, app->fontLarge, "TECH TREE", titleRect);

    char buffer[128];

    for (int i = 0; i < MAX_UPGRADES; i++)
    {
        // 2. Use the 'tree' pointer instead of 'app->techTreeP1'
        sprintf(buffer,
            "%d. %s - %dG [%s]",
            i + 1,
            tree->upgrades[i].name,
            tree->upgrades[i].cost,
            tree->upgrades[i].unlocked ? "UNLOCKED" : "LOCKED"
        );

        SDL_FRect rect = {
            780,
            170 + i * 70,
            700,
            50
        };

        drawText(app, app->font, buffer, rect);
    }
}

bool isUpgradeUnlocked(TechTree *tree, int index)
{
    if (index < 0 || index >= MAX_UPGRADES) return false;

    return tree->upgrades[index].unlocked;
}
