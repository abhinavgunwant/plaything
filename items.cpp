#include <iostream>

#include "raylib.h"

#include "common.hpp"
#include "entity-manager.hpp"
#include "inventory-manager.hpp"
#include "shapes.hpp"
#include "items.hpp"

using namespace std;

string iconPaths[]= {
    "",
    "./assets/blueprint_invIcon.png",
    "./assets/hammer_invIcon.png",
    "",
    "./assets/pistol_invIcon.png",
    "./assets/furnace_invIcon.png",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
};

Item::Item() {
    type = ITEM_NONE;
    invImageTexture = { 0, 0, 0, 0, 0 };
    clip = 0;
    ammo = 0;
    health = 0;
}

Item::Item(ItemType type, Image invImage) {
    this->type = type;
    invImageTexture = LoadTextureFromImage(invImage);
    clip = 0;
    ammo = 0;
    health = 0;
}

Item::Item(ItemType type) {
    this->type = type;
    invImageTexture = { 0, 0, 0, 0, 0 };
    clip = 0;
    ammo = 0;
    health = 0;
}

Item::~Item() { }

void Item::onClick() {
    if (im.isInventoryShown()) { return; }

    switch (this->type) {
        case ITEM_BLUEPRINT: {
            cout << "\nPlace building unit.";
            EntityCollision ec = em.getRayCastCollision(ECT_TERRAIN, 10);

            if (ec.entityId != 0 && ec.collision.hit) {
                Vector3 p = ec.collision.point;
                p.y -= 0.6;
                Entity3D foundation(Cube(p, 2, 2, 2, BLUE));
                foundation.item.type = ITEM_BUILDING_FOUNDATION;
                em.addEntity(foundation);
            }

            break;
        }

        case ITEM_HAMMER: {
            cout << "\nHeal building units.";
            break;
        }

        case ITEM_ASSAULT_RIFLE: {
            cout << "\nFire bullet from assault rifle";
            break;
        }

        case ITEM_PISTOL: {
            cout << "\nFire bullet from pistol";
            EntityCollision ec = em.getRayCastCollision(ECT_OTHER, 1000);

            Color colors[] = { RED, GREEN, YELLOW, BLUE, ORANGE, PURPLE, BLACK, WHITE };

            if (ec.entityId != 0 && ec.collision.hit) {
                cout << "\nentity got hit!";

                Entity3D entity = em.get3DEntity(ec.entityId);
                cout << "\nEntity's item type: " << getItemTypeName(entity.item.type);

                if (entity.item.type == ITEM_BUILDING_FOUNDATION) {
                    cout << "\nDeleting foundation";
                    em.deleteEntity(ec.entityId);
                } else if ((*(entity.shapes.begin())).type == SHAPE_3D_CUBE) {
                    Color oldColor = (*(entity.shapes.begin())).shapeData.cubeData.color;
                    Color newColor;

                    do {
                        newColor = colors[rand() % 8];
                    } while(COLOR_EQUAL(newColor, oldColor));

                    (*(entity.shapes.begin())).shapeData.cubeData.color = newColor;
                }

                cout << "\nupdating entity";
                em.update3DEntity(entity.getId(), entity);
            }

            break;
        }

        case ITEM_FURNACE: {
            cout << "\nPlace furnace";
            EntityCollision ec = em.getRayCastCollision(ECT_TERRAIN, 1000);

            if (ec.entityId != 0 && ec.collision.hit) {
                Vector3 spawnPoint = ec.collision.point;
                Entity3D furnaceEntity = Entity3D(em.lastId() + 1, Cube(spawnPoint, 1, 5, 2, ORANGE));
                Item furnaceItem = Item(ITEM_FURNACE);

                furnaceItem.itemData.furnace = DEMO_FURNACE(
                    1000, 1000, 100, FARM_ITEM_METAL_FRAGS, 100,
                    FARM_ITEM_METAL_ORE, true
                );

                furnaceEntity.item = furnaceItem;
                uint32_t id = em.addEntity(furnaceEntity);
            }

            break;
        }

        default: return;
    }
}

void Item::onRightClick() {
    switch (this->type) {
        case ITEM_BLUEPRINT: {
            cout << "\nSelect building item";
            break;
        }

        case ITEM_HAMMER: {
            cout << "\nSelect building item's upgrade options";
            break;
        }

        case ITEM_ASSAULT_RIFLE: {
            cout << "\nADS";
            break;
        }

        case ITEM_PISTOL: {
            cout << "\nADS";
            break;
        }

        default: return;
    }
}

void Item::onUse(uint32_t entityId) {
    cout << "\nInside Item::onUse()";
    switch (type) {
        case ITEM_BLUEPRINT: break;
        case ITEM_HAMMER: break;
        case ITEM_ASSAULT_RIFLE: break;
        case ITEM_PISTOL: break;
        case ITEM_FURNACE: {
            printState();

            // if (im.isInventoryShown()) {
            //     im.hideEntityInteractionMenu();
            //     im.hideInventoryMenu();
            // } else {
                im.showEntityInteractionMenu(entityId);
            // }
            break;
        }
        default: return;
    }
}

void Item::onKeyPress() {
    switch (this->type) {
        case ITEM_BLUEPRINT: break;
        case ITEM_HAMMER: break;
        case ITEM_ASSAULT_RIFLE: {
            if (IsKeyUp(KEY_R)) {
                cout << "\nReload";
            }
            break;
        }
        case ITEM_PISTOL: {
            if (IsKeyDown(KEY_R)) {
                cout << "\nReload";
            }
            break;
        }
        case ITEM_FURNACE: break;
        default: return;
    }
}

void Item::printState() {
    switch (type) {
        case ITEM_BLUEPRINT: break;
        case ITEM_HAMMER: break;
        case ITEM_ASSAULT_RIFLE: break;
        case ITEM_PISTOL: break;
        case ITEM_FURNACE: {
            cout << "\nFurnace { wood: { quantity: " << itemData.furnace.woodQty
                << ", health: " << (int)itemData.furnace.woodHealth
                << " }, ores: { slot1: { item: "
                << getItemTypeName(itemData.furnace.oreTypes[0])
                << ", health: " << (int)itemData.furnace.oreHealth[0]
                << ", quantity: " << itemData.furnace.oreQty[0]
                << " }, slot2: { item: "
                << getItemTypeName(itemData.furnace.oreTypes[1])
                << ", health: " << (int)itemData.furnace.oreHealth[1]
                << ", quantity: " << itemData.furnace.oreQty[1]
                << " } }, output: { slot1: { item: "
                << getItemTypeName(itemData.furnace.outputTypes[0])
                << ", quantity: " << itemData.furnace.outputQty[0]
                << " }, slot2: { item: "
                << getItemTypeName(itemData.furnace.outputTypes[1])
                << ", quantity: " << itemData.furnace.outputQty[1]
                << " }, slot3: { item: "
                << getItemTypeName(itemData.furnace.outputTypes[2])
                << ", quantity: " << itemData.furnace.outputQty[2]
                << " }, on: " << itemData.furnace.on << " } }";
            break;
        }
        default: return;
    }
}

string getItemTypeName(ItemType type) {
    switch (type) {
        case ITEM_NONE: return "ITEM_NONE";
        case ITEM_BLUEPRINT: return "";
        case ITEM_HAMMER: return "ITEM_HAMMER";
        case ITEM_ASSAULT_RIFLE: return "ITEM_ASSAULT_RIFLE";
        case ITEM_PISTOL: return "ITEM_PISTOL";
        case ITEM_FURNACE: return "ITEM_FURNACE";
        case ITEM_TREE: return "ITEM_TREE";
        case ITEM_NODE_STONE: return "ITEM_NODE_STONE";
        case ITEM_NODE_METAL: return "ITEM_NODE_METAL";
        case ITEM_NODE_SULFUR: return "ITEM_NODE_SULFUR";
        case ITEM_BUILDING_FOUNDATION: return "ITEM_BUILDING_FOUNDATION";
        case FARM_ITEM_WOOD: return "FARM_ITEM_WOOD";
        case FARM_ITEM_CHARCOAL: return "FARM_ITEM_CHARCOAL";
        case FARM_ITEM_STONES: return "FARM_ITEM_STONES";
        case FARM_ITEM_METAL_ORE: return "FARM_ITEM_METAL_ORE";
        case FARM_ITEM_METAL_FRAGS: return "FARM_ITEM_METAL_FRAGS";
        case FARM_ITEM_HQM_ORE: return "FARM_ITEM_HQM_ORE";
        case FARM_ITEM_HQM: return "FARM_ITEM_HQM";
        case FARM_ITEM_SULFUR_ORE: return "FARM_ITEM_SULFUR_ORE";
        case FARM_ITEM_SULFUR: return "FARM_ITEM_SULFUR";
        default: return "";
    }
}

