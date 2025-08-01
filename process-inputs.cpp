#include <iostream>
#include "raylib.h"

#include "process-inputs.hpp"
#include "entity-manager.hpp"
#include "inventory-manager.hpp"
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

        int8_t index = im.getUbIndex();

        if (mouseWheelMove > 0) {
            // cout<<" up";
            if (index == -1) {
                index = 5;
            } else {
                --index;
            }
        } else {
            // cout<<" down";
            if (index == 5) {
                index = -1;
            } else {
                ++index;
            }
        }

        im.setUbIndex(index);
    }

    // Check if user is using an item from utility belt
    if (im.isUbActive()) {
        Item item = im.getActiveUbItem();

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
                e.item.onUse(ec.entityId);
                em.blockEntityInteraction(ec.entityId);
            } else {
                cout << " Entity is temporarily blocked from using.";
            }
        }
    }

    if (IsKeyDown(KEY_TAB) && !im.isInventoryMenuBlocked()) {
        if (im.isInventoryShown()) {
            im.hideInventoryMenu();
        } else {
            im.showInventoryMenu();
        }
    }
}

