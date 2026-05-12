#include "techTree.h"
#include <stdio.h>
#include <string.h>
#include "../game.h"

/*

Piece Upgrades
- mage
- lancer
- catapualt
- Envoy

Building upgrades
- increased town radius
- increased gold income

Generic/ random upgrades
- 25% to gain half captured piece value
- random spawing piece
- upgrade platforms

*/






void initTechTree(TechTree *tree)
{
    strcpy(tree->upgrades[0].name, "Lancer");
    tree->upgrades[0].cost = 30;
    tree->upgrades[0].required = -1;

    strcpy(tree->upgrades[1].name, "Catapult");
    tree->upgrades[1].cost = 50;
    tree->upgrades[1].required = 0;

    strcpy(tree->upgrades[2].name, "Mage");
    tree->upgrades[2].cost = 70;
    tree->upgrades[2].required = 1;

    strcpy(tree->upgrades[3].name, "Town Income");
    tree->upgrades[3].cost = 40;
    tree->upgrades[3].required = -1;

    strcpy(tree->upgrades[5].name, "Spawn Extra");
    tree->upgrades[5].cost = 60;
    tree->upgrades[5].required = 0;

    for (int i = 0; i < MAX_UPGRADES; i++)
        tree->upgrades[i].unlocked = 0;
}

void unlockUpgrade(TechTree *tree, int index, int *gold)
{
    Upgrade *u = &tree->upgrades[index];

    if (u->required != -1 && !tree->upgrades[u->required].unlocked)
    {
        printf("Unlock %s first!\n", tree->upgrades[u->required].name);
        return;
    }

    if (u->unlocked)
    {
        printf("Already unlocked\n");
        return;
    }

    if (*gold >= u->cost)
    {
        *gold -= u->cost;
        u->unlocked = 1;
        printf("Unlocked: %s\n", u->name);
    }
    else
    {
        printf("Not enough gold\n");
    }
}

void printTechTree(AppState *app)
{
    app->techText.x = 930;
    app->techText.y = 50;
    app->techText.h = 200;
    app->techText.w = 200;

    drawText(app, app->font, "TECH TREE", app->techText);


    // for (int i = 0; i < MAX_UPGRADES; i++)
    // {
    //     printf("%d. %s | Cost: %d | %s\n",
    //            i,
    //            tree->upgrades[i].name,
    //            tree->upgrades[i].cost,
    //            tree->upgrades[i].unlocked ? "Unlocked" : "Locked");
    // }
}

bool isUpgradeUnlocked(TechTree *tree, int index)
{
    if (index < 0 || index >= MAX_UPGRADES)
    return false;

return tree->upgrades[index].unlocked;
}
