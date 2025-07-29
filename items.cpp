#include <iostream>

#include "raylib.h"

#include "entity-manager.hpp"
#include "shapes.hpp"
#include "items.hpp"

using namespace std;

Item::Item() {
    type = ITEM_NONE;
    invImageTexture = { 0, 0, 0, 0, 0 };
    clip = 0;
    ammo = 0;
    health = 0;
}

Item::Item(ItemType type, Image invImage) {
    this->type = type;
    this->invImageTexture = LoadTextureFromImage(invImage);
    clip = 0;
    ammo = 0;
    health = 0;
}

Item::Item(ItemType type) {
    this->type = type;
    this->invImageTexture = { 0, 0, 0, 0, 0 };
    clip = 0;
    ammo = 0;
    health = 0;
}

void Item::onClick() {
    switch (this->type) {
        case ITEM_BLUEPRINT: {
            cout << "\nPlace building unit.";
            EntityCollision ec = em.getRayCastCollision(ECT_TERRAIN);

            if (ec.entityId != 0 && ec.collision.hit) {
                Vector3 p = ec.collision.point;
                p.y -= 0.6;
                em.addEntity(Cube(p, 2, 2, 2, BLUE));
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
            EntityCollision ec = em.getRayCastCollision(ECT_OTHER);

            Color colors[] = { RED, GREEN, YELLOW, BLUE, ORANGE, PURPLE, BLACK, WHITE };

            if (ec.entityId != 0 && ec.collision.hit) {
                cout << "\nentity got hit!";
                Entity3D entity = em.get3DEntity(ec.entityId);

                if ((*(entity.shapes.begin())).type == SHAPE_3D_CUBE) {
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

void Item::onKeyPress() {
    switch (this->type) {
        case ITEM_BLUEPRINT: {
            break;
        }

        case ITEM_HAMMER: {
            break;
        }

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

        default: return;
    }
}

