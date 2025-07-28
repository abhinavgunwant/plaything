#include <iostream>
#include "raylib.h"

#include "process-inputs.hpp"
#include "entity-manager.hpp"
#include "game-state.hpp"

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
        cout << "\nMousewheel moved: " << mouseWheelMove;
        short prevIndex = globalState.utilBeltActiveIndex;

        if (mouseWheelMove > 0) {
            cout<<" up";
            // globalState.utilBeltActiveIndex = ((globalState.utilBeltActiveIndex - 1) % 7) - 1
            if (globalState.utilBeltActiveIndex == -1) {
                globalState.utilBeltActiveIndex = 5;
            } else {
                globalState.utilBeltActiveIndex -= 1;
            }
        } else {
            cout<<" down";
            if (globalState.utilBeltActiveIndex == 5) {
                globalState.utilBeltActiveIndex = -1;
            } else {
                globalState.utilBeltActiveIndex += 1;
            }
        }

        cout << "\nUtil belt active index: " << globalState.utilBeltActiveIndex;
        cout << "\nUtil belt previous index: " << prevIndex;

        if (globalState.utilBeltActiveIndex != -1) {
            // cout << "\n!! 1";
            const int entityId = globalState.utilBeltEntityIdStart + globalState.utilBeltActiveIndex;
            Entity2D activeInvItem = em.get2DEntity(entityId);

            // cout << "\n!! 2 entityId: " << entityId;

            if (activeInvItem.getId() != 0) {
                // cout << "\n!! 3";
                try {
                    cout << "\n!! 4";
                    // if (activeInvItem.shapes.size()) {
                        // cout << "contains more than one element!";
                    // }
                    // shape2DInfo(activeInvItem.shapes.at(0));

                    // for (
                    //     vector<Shape2D>::iterator itr = activeInvItem.shapes.begin();
                    //     itr<activeInvItem.shapes.end();
                    //     ++itr
                    // ) {
                    //     cout << "\n!! 4 1";
                    //     (*itr).shapeData.dimensionData.color = GRAY;
                    // }

                    (*activeInvItem.shapes.begin()).shapeData.dimensionData.color = GRAY;

                    // activeInvItem.shapes.at(0).shapeData.dimensionData.color = GRAY;
                    em.update2DEntity(activeInvItem.getId(), activeInvItem);
                } catch (exception &e) {
                    cout << "\nException 1: " << e.what() << " total items: "
                        << activeInvItem.shapes.size();
                }
            }
        }

        if (prevIndex != -1) {
            // cout << "\n!! 5";
            const int entityId = globalState.utilBeltEntityIdStart + prevIndex;
            Entity2D prevItem = em.get2DEntity(entityId);
            // cout << "\n!! 6";

            if (prevItem.getId() != 0) {
                // cout << "\n!! 7";
                try {
                    cout << "\n!! 8";
                    // shape2DInfo(prevItem.shapes.at(0));

                    // for (
                    //     vector<Shape2D>::iterator itr = prevItem.shapes.begin();
                    //     itr<prevItem.shapes.end();
                    //     ++itr
                    // ) {
                    //     cout << "\n!! 8 1";
                    //     (*itr).shapeData.dimensionData.color = BLACK;
                    // }

                    (*prevItem.shapes.begin()).shapeData.dimensionData.color = BLACK;

                    // prevItem.shapes.at(0).shapeData.dimensionData.color = BLACK;
                    em.update2DEntity(prevItem.getId(), prevItem);
                } catch (exception &e) {
                    cout << "\nException 1: " << e.what() << " total items: "
                        << prevItem.shapes.size();
                }
            }
        }
    }
}

