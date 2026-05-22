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


void initTechTree(TechTree *tree)
{
    // Index 0: Knight (Needs Pawn - Start)
    strcpy(tree->upgrades[0].name, "Knight");
    tree->upgrades[0].cost = 5;
    tree->upgrades[0].required1 = -1; 
    tree->upgrades[0].required2 = -1;

    // Index 1: Envoy (Needs Pawn - Start)
    strcpy(tree->upgrades[1].name, "Envoy");
    tree->upgrades[1].cost = 5;
    tree->upgrades[1].required1 = -1;
    tree->upgrades[1].required2 = -1;

    // Index 2: Extra Money (Needs Pawn - Start)
    strcpy(tree->upgrades[2].name, "Town Income");
    tree->upgrades[2].cost = 10;
    tree->upgrades[2].required1 = -1;
    tree->upgrades[2].required2 = -1;

    // Index 3: UPG Platform I (Needs Pawn - Start)
    strcpy(tree->upgrades[3].name, "UPG PLAT I");
    tree->upgrades[3].cost = 15;
    tree->upgrades[3].required1 = -1;
    tree->upgrades[3].required2 = -1;

    // Index 4: Rook (Needs Knight)
    strcpy(tree->upgrades[4].name, "Rook");
    tree->upgrades[4].cost = 8;
    tree->upgrades[4].required1 = 0; // Knight
    tree->upgrades[4].required2 = -1;

    // Index 5: Bishop (Needs Knight)
    strcpy(tree->upgrades[5].name, "Bishop");
    tree->upgrades[5].cost = 8;
    tree->upgrades[5].required1 = 0; // Knight
    tree->upgrades[5].required2 = -1;

    // Index 6: Lancer (Needs Envoy + Knight)
    strcpy(tree->upgrades[6].name, "Lancer");
    tree->upgrades[6].cost = 12;
    tree->upgrades[6].required1 = 1; // Envoy
    tree->upgrades[6].required2 = 0; // Knight

    // Index 7: Mage (Needs Lancer + Bishop)
    strcpy(tree->upgrades[7].name, "Mage");
    tree->upgrades[7].cost = 15;
    tree->upgrades[7].required1 = 6; // Lancer
    tree->upgrades[7].required2 = 5; // Bishop

    // Index 8: Catapult (Needs Mage + Rook)
    strcpy(tree->upgrades[8].name, "Catapult");
    tree->upgrades[8].cost = 18;
    tree->upgrades[8].required1 = 7; // Mage
    tree->upgrades[8].required2 = 4; // Rook

    // Index 9: Queen (Needs Rook + Catapult)
    strcpy(tree->upgrades[9].name, "Queen");
    tree->upgrades[9].cost = 20;
    tree->upgrades[9].required1 = 4; // Rook
    tree->upgrades[9].required2 = 8; // Catapult

    // Index 10: Extra Pieces (Needs Extra Money)
    strcpy(tree->upgrades[10].name, "Spawn Extra");
    tree->upgrades[10].cost = 15;
    tree->upgrades[10].required1 = 2; // Extra Money
    tree->upgrades[10].required2 = -1;

    // Index 11: UPG Platform II (Needs UPG Platform I)
    strcpy(tree->upgrades[11].name, "UPG PLAT II");
    tree->upgrades[11].cost = 18;
    tree->upgrades[11].required1 = 3; // Plat I
    tree->upgrades[11].required2 = -1;

    // Index 12: UPG Platform III (Needs UPG Platform II)
    strcpy(tree->upgrades[12].name, "UPG PLAT III");
    tree->upgrades[12].cost = 25;
    tree->upgrades[12].required1 = 11; // Plat II
    tree->upgrades[12].required2 = -1;

    tree->upgradePlatformLevel = 0;

    for (int i = 0; i < MAX_UPGRADES; i++) tree->upgrades[i].unlocked = 0;
}

// In techTree.c
bool unlockUpgrade(TechTree *tree, int index, int *gold)
{
    Upgrade *u = &tree->upgrades[index];

    // Check first requirement
    if (u->required1 != -1 && !tree->upgrades[u->required1].unlocked) return false;
    
    // Check second requirement (if it exists)
    if (u->required2 != -1 && !tree->upgrades[u->required2].unlocked) return false;

    if (u->unlocked) return false;

    if (*gold >= u->cost)
    {
        *gold -= u->cost;
        u->unlocked = 1;

        if (index == 3)
        {
            tree->upgradePlatformLevel = 1;
        }
        else if (index == 11)
        {
            tree->upgradePlatformLevel = 2;
        }
        else if (index == 12)
        {
            tree->upgradePlatformLevel = 3;
        }

        printf("Unlocked: %s\n", u->name);
        return true;
    }
    return false;
}

void techTreeOverlay(AppState *app)
{
    SDL_FRect bgPos = { (WINDOW_WIDTH - 1200)/2, (WINDOW_HEIGHT - 800)/2, 1200, 800 };
    SDL_RenderTexture(app->renderer, app->techTreeBackground, NULL, &bgPos);

    // 1. Identify which tree to display
    TechTree *tree = (app->currentPlayer == 1) ? &app->techTreeP1 : &app->techTreeP2;
    int *gold = (app->currentPlayer == 1) ? &app->P1.p1Gold : &app->P2.p2Gold;


    SDL_FRect titleRect = {900, 50, 400, 60};
    drawText(app, app->fontLarge, "TECH TREE", titleRect);

    //button positions
    static const SDL_FRect buttonPositions[MAX_UPGRADES] = {
        { 730, 640, 140, 45 },  //  0 Knight (Center row, 2nd card)
        { 575, 870, 140, 45 },  //  1 Envoy (Bottom row, 1st card)
        { 560, 480, 160, 45 },  //  2 Town Income (Middle-Left)
        { 575, 300,  140, 45 },  //  3 UPG PLAT I (Top row, 1st card)
        { 1100, 520, 140, 45 }, //  4 Rook (Far right, top of split)
        { 910, 645, 140, 45 },  //  5 Bishop (Center row, 3rd card)
        { 730, 870, 140, 45 },  //  6 Lancer (Bottom row, 2nd card)
        { 910, 870, 140, 45 },  //  7 Mage (Bottom row, 3rd card)
        { 1100, 870, 140, 45 }, //  8 Catapult (Bottom row, 4th card)
        { 1325, 690, 140, 45 }, //  9 Queen (Final card)
        { 740, 480, 150, 45 }, // 10 Spawn Extra (Far right label)
        { 812, 300,  140, 45 },  // 11 UPG PLAT II (Top row, 2nd card)
        { 1050, 300,  140, 45 }, // 12 UPG PLAT III (Top row, 3rd card)
    };

    
    for (int i = 0; i < MAX_UPGRADES; i++) {
        // Skip indices that don't have a name yet
        if (strlen(tree->upgrades[i].name) == 0) continue;

        SDL_FRect btnRect = buttonPositions[i];
        SDL_FRect textRect = btnRect;
        textRect.y += 5; // Slight nudge to center text vertically in the button

        // Draw visual state
        if (tree->upgrades[i].unlocked) {
            SDL_SetRenderDrawColor(app->renderer, 40, 180, 40, 200); // Semi-transparent green
        } else {
            SDL_SetRenderDrawColor(app->renderer, 80, 80, 80, 180);  // Semi-transparent grey
        }
        
        SDL_RenderFillRect(app->renderer, &btnRect);
        drawText(app, app->font, tree->upgrades[i].name, textRect);


        // Handle Clicking
        if (app->input.mouseLeftPressed) {
            SDL_FPoint mouse = {app->input.mouseX, app->input.mouseY};
            if (SDL_PointInRectFloat(&mouse, &btnRect)) {
                unlockUpgrade(tree, i, gold);
            }
        }
    }
}


bool isUpgradeUnlocked(TechTree *tree, int index)
{
    if (index < 0 || index >= MAX_UPGRADES) return false;

    return tree->upgrades[index].unlocked;
}

bool isUpgradePlatformUnlocked(TechTree *tree)
{
    return tree->upgradePlatformLevel > 0;
}

bool isSelectedPieceOnUpgradeTile(AppState *app)
{
    if (!app->selectedPiece)
        return false;

    int row = app->selectedPiece->row;
    int col = app->selectedPiece->col;

    return map_data[row][col] == 'U';
}
