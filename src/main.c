#include "animation1.h"
#include "button1.h"

#include <raylib.h>

int main(void)
{
    int const screenWidth = 800;
    int const screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Animation");

    SetTargetFPS(60);

    AnimationContext * const ctx = animation1_init();

    float const screen_width = GetScreenWidth();
    float const screen_height = GetScreenHeight();
    float const button_height = 50.f;
    float const button_width = 100.f;
    float const button_x = (screen_width - button_width) / 2.f;
    float const button_y = (screen_height - button_height) / 2.f;

    ButtonContext * const button = button1_init(button_x, button_y, button_width, button_height);


    // Main game loop
    while (!WindowShouldClose())
    {
        Environment const env = {
            .delta = GetFrameTime(),
        };

        if (IsKeyPressed(KEY_R))
        {
            animation1_reset(ctx);
        }

        animation1_update(ctx, &env);
        button1_update(button, &env);

        BeginDrawing();

            ClearBackground(RAYWHITE);

            //DrawText("Hello, World!", 190, 200, 20, LIGHTGRAY);
            animation1_draw(ctx);
            button1_draw(button);

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    animation1_free(ctx);

    return 0;
}

