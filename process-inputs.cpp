#include <iostream>
#include "raylib.h"

#include "process-inputs.hpp"
#include "entity-manager.hpp"
#include "game-state.hpp"
#include "common.hpp"

void processInputs() {
	const float frameTime = GetFrameTime();

    if (IsKeyPressed(KEY_G)) {
        em.updateCameraTarget(VEC_ZERO);
    }

    if (IsKeyPressed(KEY_SPACE)) {
        if (globalState.cursorEnabled) {
            DisableCursor();
        }
        else {
            EnableCursor();
        }

        globalState.cursorEnabled = !globalState.cursorEnabled;
    }

    float mouseWheelMove = GetMouseWheelMove();

    if (mouseWheelMove != 0) {
        // cout << "\nMousewheel: ";
        short prevIndex = globalState.utilBeltActiveIndex;

        if (mouseWheelMove > 0) {
            // cout<<" up";
            if (globalState.utilBeltActiveIndex == -1) {
                globalState.utilBeltActiveIndex = 5;
            } else {
                globalState.utilBeltActiveIndex -= 1;
            }
        } else {
            // cout<<" down";
            if (globalState.utilBeltActiveIndex == 5) {
                globalState.utilBeltActiveIndex = -1;
            } else {
                globalState.utilBeltActiveIndex += 1;
            }
        }

        // cout << "\nUtil belt active index: " << globalState.utilBeltActiveIndex;
        // cout << "\nUtil belt previous index: " << prevIndex;

        if (globalState.utilBeltActiveIndex != -1) {
            const int entityId = globalState.utilBeltEntityIdStart + globalState.utilBeltActiveIndex;
            Entity2D activeInvItem = em.get2DEntity(entityId);

            if (activeInvItem.getId() != 0) {
                try {
                    (*activeInvItem.shapes.begin()).shapeData.dimensionData.color = COLOR_INV_BG_ACTIVE;
                    em.update2DEntity(activeInvItem.getId(), activeInvItem);
                } catch (exception &e) {
                    cout << "\nException 1: " << e.what() << " total items: "
                        << activeInvItem.shapes.size();
                }
            }
        }

        if (prevIndex != -1) {
            const int entityId = globalState.utilBeltEntityIdStart + prevIndex;
            Entity2D prevItem = em.get2DEntity(entityId);

            if (prevItem.getId() != 0) {
                try {
                    (*prevItem.shapes.begin()).shapeData.dimensionData.color = COLOR_INV_BG;
                    em.update2DEntity(prevItem.getId(), prevItem);
                } catch (exception &e) {
                    cout << "\nException 1: " << e.what() << " total items: "
                        << prevItem.shapes.size();
                }
            }
        }
    }

    // Check if user is using an item from utility belt
    if (globalState.utilBeltActiveIndex >= 0 && globalState.utilBeltActiveIndex < 6) {
        Item item = globalState.utilityItems[globalState.utilBeltActiveIndex];

        if (item.type != ITEM_NONE) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                item.onClick();
            } else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                item.onRightClick();
            }

            item.onKeyPress();
        }
    }

    if (IsKeyDown(KEY_E)) {
        EntityCollision ec = em.getRayCastCollision(ECT_OTHER, 10);

        if (ec.collision.hit) {
            cout << "\nUse hit.";
            Entity3D e = em.get3DEntity(ec.entityId);

            if (e.item.type != ITEM_NONE && !em.isEntityBlocked(ec.entityId)) {
                e.item.onUse();
                em.blockEntityInteraction(ec.entityId);

                // TODO: find a better place for this code!
                switch (e.item.type) {
                    case ITEM_FURNACE: {
                        if (em.isEntityInteractiveMenuShown()) {
                            em.hideEntityInteractionMenu();
                        } else {
                            em.showEntityInteractionMenu(ec.entityId);
                        }
                        break;
                    }
                        
                    default: break;
                }
            } else {
                cout << " Entity is temporarily blocked from using.";
            }
        }
    }

    // TODO: replace this code with inventory menu
    // if (IsKeyDown(KEY_TAB)) {
    //     if (em.isEntityInteractiveMenuShown()) {
    //         em.hideEntityInteractionMenu();
    //     } else {
    //         em.showEntityInteractionMenu(ec.entityId);
    //     }
    // }
}

