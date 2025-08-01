#include <iostream>
#include <string>
#include "common.hpp"
#include "raylib.h"

#include "inventory-manager.hpp"
#include "entity-manager.hpp"
#include "shapes.hpp"
#include "items.hpp"

#define ADD_MENU(x) invMenuEntityIds.push_back(x)

const uint8_t ALLOWED_INVENTORY_ITEMS[] = {
    ITEM_NONE,
    ITEM_BLUEPRINT,
    ITEM_HAMMER,
    ITEM_ASSAULT_RIFLE,
    ITEM_PISTOL,
    ITEM_FURNACE,
};

InventoryManager im = InventoryManager();

InventoryManager::InventoryManager() {
    ubIndex = -1;
    em.entityIdInUse = 0;
    ubInitPending = true;
    invMenuEntityIds.reserve(64);
    inventoryShown = false;
    entityMenuShown = false;
}

void InventoryManager::initUtilBelt() {
    const uint16_t dim = INV_SLOT_DIMENSION;

    Item blueprintItem = Item(ITEM_BLUEPRINT);
    Item hammerItem = Item(ITEM_HAMMER);
    Item pistolItem = Item(ITEM_PISTOL);
    Item furnaceItem = Item(ITEM_FURNACE);

    Image blueprintImage = LoadImage(iconPaths[ITEM_BLUEPRINT].c_str());
    Image hammerImage = LoadImage(iconPaths[ITEM_HAMMER].c_str());
    Image pistolImage = LoadImage(iconPaths[ITEM_PISTOL].c_str());
    Image furnaceImage = LoadImage(iconPaths[ITEM_FURNACE].c_str());

    resizeImage(dim, dim, blueprintImage);
    resizeImage(dim, dim, hammerImage);
    resizeImage(dim, dim, pistolImage);
    resizeImage(dim, dim, furnaceImage);

    blueprintItem.invImageTexture = imageToTexture(dim, dim, blueprintImage);
    hammerItem.invImageTexture = imageToTexture(dim, dim, hammerImage);
    pistolItem.invImageTexture = imageToTexture(dim, dim, pistolImage);
    furnaceItem.invImageTexture = imageToTexture(dim, dim, furnaceImage);

    utilityBelt[0].item = blueprintItem;
    utilityBelt[1].item = hammerItem;
    utilityBelt[2].item = pistolItem;
    utilityBelt[3].item = furnaceItem;
    utilityBelt[4].item = Item(ITEM_NONE);
    utilityBelt[5].item = Item(ITEM_NONE);

    ubInitPending = false;
}

uint32_t InventoryManager::getItemMenuEntityId(string key) {
    if (itemMenuMap.find(key) != itemMenuMap.end()) {
        return itemMenuMap[key];
    }

    return 0;
}

void InventoryManager::updateItemMenuText(string key, string newText) {
    cout << "\nupdating menu item text (" << key << ": " << newText;

    uint32_t id = getItemMenuEntityId(key);

    if (id == 0) { return; }

    Entity2D e = em.get2DEntity(id);

    #define TEXT_DATA   s.shapeData.textData

    if (!e.shapes.empty() && (*(e.shapes.begin())).type == SHAPE_2D_TEXT) {
        Shape2D s = (*(e.shapes.begin()));

        em.update2DEntity(
            id,
            Entity2D(id, newText, s.x, s.y, TEXT_DATA.fontSize, TEXT_DATA.color)
        );
    }

    #undef TEXT_DATA
}

void InventoryManager::showUtilityBelt() {
    if (ubInitPending) { initUtilBelt(); }

    const uint16_t dim = INV_SLOT_DIMENSION;
    const uint16_t dimGap = dim + INV_SLOT_GAP;
    const uint16_t y = UTIL_BOX_Y;

    for (uint16_t i=0, x = INV_SLOT_START_X; i<UTIL_BELT_SLOTS; ++i, x += dimGap) {
        if (ubIndex == i) {
            // Active utility belt slot
            utilityBelt[i].rectEntityId = em.addEntity(INV_RECTA(x, y, dim));
        } else {
            utilityBelt[i].rectEntityId = em.addEntity(INV_RECT(x, y, dim));
        }
        utilityBelt[i].imgEntityId = em.addEntity(UTIL_BELT_IMG(x, y, dim, i));
    }
}

void InventoryManager::hideUtilityBelt() {
    em.deleteEntities({
        utilityBelt[0].rectEntityId, utilityBelt[0].imgEntityId,
        utilityBelt[1].rectEntityId, utilityBelt[1].imgEntityId,
        utilityBelt[2].rectEntityId, utilityBelt[2].imgEntityId,
        utilityBelt[3].rectEntityId, utilityBelt[3].imgEntityId,
        utilityBelt[4].rectEntityId, utilityBelt[4].imgEntityId,
        utilityBelt[5].rectEntityId, utilityBelt[5].imgEntityId,
    });
}

void InventoryManager::showInventoryMenu() {
    if (inventoryMenuBlocked) { return; }

    hideUtilityBelt();

    menuBlockTimer = 0.25;
    inventoryMenuBlocked = true;
    EnableCursor();

    const uint16_t dim = INV_SLOT_DIMENSION;
    const uint16_t dimGap = dim + INV_SLOT_GAP;
    const uint16_t xStart = INV_SLOT_START_X;
    const uint16_t xEnd = INV_SLOT_START_X + dimGap * 6 - INV_SLOT_GAP;
    const uint16_t yEnd = SCREEN_HEIGHT - dimGap * 2;
    const uint16_t yStart = yEnd - (INV_SLOT_GAP + dim)*4;

    const uint16_t outerBoxX = dimGap * 6 + INV_SLOT_GAP;
    const uint16_t outerBoxY = dimGap * 5 + INV_SLOT_GAP;

    ADD_MENU(ADD(Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, INV_OUTER_BOX_COLOR)));

    for (uint16_t y = yStart; y <= yEnd; y += dimGap) {
        for (uint16_t x = xStart; x <= xEnd; x += dimGap) {
            ADD_MENU(ADD(Rect(x, y, dim, dim, COLOR_INV_BG)));
        }
    }

    showUtilityBelt();
    inventoryShown = true;
}

void InventoryManager::hideInventoryMenu() {
    hideUtilityBelt();

    menuBlockTimer = 0.25;
    inventoryMenuBlocked = true;
    DisableCursor();

    em.deleteEntities(invMenuEntityIds);
    invMenuEntityIds.clear();

    showUtilityBelt();
    inventoryShown = false;
}

void InventoryManager::showEntityInteractionMenu(uint32_t id) {
    if (inventoryMenuBlocked) { return; }
    cout << "\nshowing menu";

    menuBlockTimer = 0.25;
    EnableCursor();

    Entity3D entity = em.get3DEntity(id);
    Color base = { 255, 255, 255, 80 };
    int xOffset = 1120;

    em.entityIdInUse = id;

    hideUtilityBelt();
    showInventoryMenu();

    switch (entity.item.type) {
        case ITEM_FURNACE: {
            #define FURNACE     entity.item.itemData.furnace
            #define KEY(a, b)   a + to_string(b+1) + "Qty"
            #define QTY(a)      "x" + to_string((int) FURNACE.a)

            const uint16_t d = INV_SLOT_DIMENSION;
            ADD_MENU(ADD(Rect(xOffset, 200, 400, 500, INV_OUTER_BOX_COLOR)));
            ADD_MENU(ADD("Furnace", xOffset + 160, 210, WHITE));

            ADD_MENU(ADD("Wood:", xOffset + 175, 240, WHITE));
            ADD_MENU(ADD(Rect(xOffset + 155, 260, d, d, base)));

            ADD_MENU(ADD("Input:", xOffset + 170, 360, WHITE));
            ADD_MENU(ADD(Rect(xOffset + 105, 380, d, d, base)));
            ADD_MENU(ADD(Rect(xOffset + 205, 380, d, d, base)));

            ADD_MENU(ADD("Output:", xOffset + 165, 480, WHITE));
            ADD_MENU(ADD(Rect(xOffset + 55, 500, d, d, base)));
            ADD_MENU(ADD(Rect(xOffset + 155, 500, d, d, base)));
            ADD_MENU(ADD(Rect(xOffset + 255, 500, d, d, base)));

            if (FURNACE.woodQty > 0) {
                uint32_t woodId = ADD(QTY(woodQty), xOffset + 155, 330, 20, WHITE);
                ADD_MENU(woodId);
                itemMenuMap["woodQty"] = woodId;

                Image wood = LoadImage("./assets/wood_invIcon.png");
                ADD_MENU(ADD(Img(xOffset + 155, 260, d, d, wood)));
            }

            for (
                int i=0, offset1 = xOffset + 105, offset2 = xOffset + 55;
                i<3;
                ++i, offset1 += 100, offset2 += 100
            ) {
                if (FURNACE.outputQty[i] > 0) {
                    switch(FURNACE.outputTypes[i]) {
                        case FARM_ITEM_METAL_FRAGS: {
                            Image metalFrags = LoadImage("./assets/metalFrags_invIcon.png");
                            ADD_MENU(ADD(Img(offset2, 500, d, d, metalFrags)));
                            break;
                        }
                        case FARM_ITEM_CHARCOAL: {
                            Image charcoal = LoadImage("./assets/charcoal_invIcon.png");
                            ADD_MENU(ADD(Img(offset2, 500, d, d, charcoal)));
                            break;
                        }
                        default: continue;
                    }

                    uint32_t id1 = ADD(QTY(outputQty[i]), offset2, 570, WHITE);
                    
                    itemMenuMap[KEY("out", i)] = id1;
                    ADD_MENU(id1);
                }

                if (i > 1) { continue; }

                if (FURNACE.oreQty[i] > 0) {
                    switch(FURNACE.oreTypes[i]) {
                        case FARM_ITEM_METAL_ORE: {
                            Image metalOre = LoadImage("./assets/metalOre_invIcon.png");
                            ADD_MENU(ADD(Img(offset1, 380, d, d, metalOre)));
                            break;
                        }
                        default: continue;
                    }

                    uint32_t id2 = ADD(QTY(oreQty[i]), offset1, 450, WHITE);
                    itemMenuMap[KEY("ore", i)] = id2;
                    ADD_MENU(id2);
                }
            }

            if (FURNACE.on) {
                ADD_MENU(ADD("Turn Off", xOffset + 155, 630, WHITE));
                ADD_MENU(ADD(Rect(xOffset + 145, 600, 110, d, { 255, 0, 0, 160 })));
            } else {
                ADD_MENU(ADD("Turn On", xOffset + 155, 630, WHITE));
                ADD_MENU(ADD(Rect(xOffset + 155, 600, d, d, { 255, 0, 0, 160 })));
            }

            #undef KEY
            #undef QTY
            #undef FURNACE
            break;
        }
        default: break;
    }

    showUtilityBelt();
}

void InventoryManager::hideEntityInteractionMenu() {
    if (menuBlockTimer > 0) { return; }
    cout << "\nhiding menu";

    DisableCursor();
    menuBlockTimer = 0.25;

    em.deleteEntities(invMenuEntityIds);
    em.entityIdInUse = 0;
    itemMenuMap.clear();
}

int8_t InventoryManager::getUbIndex() { return ubIndex; }

void InventoryManager::setUbIndex(int8_t index) {
    Entity2D oldRectEntity = em.get2DEntity(utilityBelt[ubIndex].rectEntityId);

    if (oldRectEntity.getId() != 0) {
        try {
            (*oldRectEntity.shapes.begin()).shapeData.dimensionData.color = COLOR_INV_BG;
            em.update2DEntity(oldRectEntity.getId(), oldRectEntity);
        } catch (exception &e) {
            // cout << "\nException 1: " << e.what() << " total items: "
            //     << oldRectEntity.shapes.size();
        }
    }

    ubIndex = index;

    if (index == -1) { return; }

    Entity2D rectEntity = em.get2DEntity(utilityBelt[index].rectEntityId);

    if (rectEntity.getId() != 0) {
        try {
            (*rectEntity.shapes.begin()).shapeData.dimensionData.color = COLOR_INV_BG_ACTIVE;
            em.update2DEntity(rectEntity.getId(), rectEntity);
        } catch (exception &e) {
            // cout << "\nException 1: " << e.what() << " total items: "
            //     << rectEntity.shapes.size();
        }
    }
}

void InventoryManager::updateMenuInteraction(float time) {
    if (menuBlockTimer > 0) {
        menuBlockTimer -= time;
    } else if (menuBlockTimer <= 0) {
        menuBlockTimer = 0;
        inventoryMenuBlocked = false;
    }
}

bool InventoryManager::isUbActive() { return ubIndex > -1 && ubIndex < 6; }
bool InventoryManager::isInventoryShown() { return inventoryShown; }
bool InventoryManager::isInventoryMenuBlocked() { return inventoryMenuBlocked; }
bool InventoryManager::isEntityMenuShown() { return entityMenuShown; }

Item InventoryManager::getActiveUbItem() {
    if (isUbActive()) {
        return utilityBelt[ubIndex].item;
    }

    return Item(ITEM_NONE);
}

