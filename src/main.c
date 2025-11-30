#include "animation1.h"
#include "animation_modules.h"
#include "button1.h"

#include <raylib.h>

int main(void)
{
    int const screenWidth = 800;
    int const screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Animation");

    SetTargetFPS(60);

    animation_handlers_st const * const animation1_handlers = get_animation1_animation_handlers();
    void * const ctx = animation1_init();

    float const screen_width = GetScreenWidth();
    float const screen_height = GetScreenHeight();
    float const button_height = 50.f;
    float const button_width = 100.f;
    float const button_x = (screen_width - button_width) / 2.f;
    float const button_y = (screen_height - button_height) / 2.f;

    animation_handlers_st const * const button_handlers = get_button_animation_handlers();
    void * const button = button1_init(button_x, button_y, button_width, button_height);

    // Main game loop
    while (!WindowShouldClose())
    {
        Environment const env = {
            .delta = {GetFrameTime()},
        };

        if (IsKeyPressed(KEY_R))
        {
            animation1_handlers->reset(ctx);
        }

        animation1_handlers->update(ctx, &env);
        button_handlers->update(button, &env);

        BeginDrawing();

            ClearBackground(RAYWHITE);

            //DrawText("Hello, World!", 190, 200, 20, LIGHTGRAY);
            animation1_handlers->draw(ctx);
            button_handlers->draw(button);

        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    animation1_handlers->free(ctx);
    button_handlers->free(button);

    return 0;
}

