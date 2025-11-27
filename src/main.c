#include "animation1.h"

#include <raylib.h>

int main(void)
{
    int const screenWidth = 800;
    int const screenHeight = 450;
    AnimationContext * const ctx = animation1_init();

    InitWindow(screenWidth, screenHeight, "Animation");

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose())
    {
        Environment const env = {
            .elapsed = GetFrameTime(),
        };

        BeginDrawing();

            ClearBackground(RAYWHITE);
            animation1_update(ctx, &env);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    animation1_free(ctx);

    return 0;
}

