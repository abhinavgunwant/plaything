#include <iostream>
#include "raylib.h"

#include "entity-manager.hpp"
#include "shapes.hpp"
#include "common.hpp"

using namespace std;

int main() {
    InitWindow(800, 450, "raylib test");
    SetTargetFPS(60);
    DisableCursor();

    Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };

    bool cursorEnabled = false;

    EntityManager em = EntityManager();

    Entity grid = Entity(XAxis());
    grid.addShape(YAxis());
    grid.addShape(ZAxis());

    em.addEntity(Cube(VEC_ZERO, 5.0f, 5.0f, 5.0f, YELLOW));
    em.addEntity(grid);

    // Main loop
    while (!WindowShouldClose()) {
        UpdateCamera(em.getCamera(), CAMERA_FREE);

        if (IsKeyPressed(KEY_G)) {
            em.updateCameraTarget(VEC_ZERO);
        }

        if (IsKeyPressed(KEY_SPACE)) {
            if (cursorEnabled) {
                DisableCursor();
            } else {
                EnableCursor();
            }

            cursorEnabled = !cursorEnabled;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);

        em.drawEntities();

        DrawText("Raylib test", 0, 0, 20, GRAY);

        EndDrawing();
    }
 
    CloseWindow();

    return 0;
}

