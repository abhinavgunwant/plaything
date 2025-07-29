#include <iostream>
#include "raylib.h"

#include "entity-manager.hpp"
#include "process-inputs.hpp"
#include "common.hpp"

using namespace std;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib test");
    SetTargetFPS(60);
    DisableCursor();
    setupEntities();

    // Main loop
    while (!WindowShouldClose()) {
        processInputs();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        em.manageEntities();

        EndDrawing();
    }
 
    CloseWindow();

    return 0;
}

