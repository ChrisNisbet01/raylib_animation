#include "raylib.h"
#include "animation1.h"

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    int const screenWidth = 800;
    int const screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Animation");

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawAnimation1();

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}

