#include "button1.h"

#include "dynamic_array.h"
#include "utils.h"

#include <coroutine.h>

#include <raylib.h>
#include <raymath.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct button_st button_st;
typedef struct button_state_st {
    bool was_pressed;
    bool is_pressed;
} button_state_st;
#define button_state_pressed (button_state_st){true}

typedef struct button_config_st {
    Rectangle rec;
    Color pressed_color;
    Color unpressed_color;
} button_config_st;

struct button_st
{
    button_state_st state;
    button_config_st config;
};

struct ButtonContext
{
    Environment const * env;
    button_st button;
};

static size_t const stack_size = 10000;

static void
draw_button(button_st const * const b)
{
    Color color = b->state.is_pressed ? b->config.pressed_color : b->config.unpressed_color;

    DrawRectangleRec(b->config.rec, color);
}

void
button1_free(ButtonContext * const ctx)
{
    free(ctx);
}

static void
button_update(ButtonContext * const ctx)
{
    button_st * const b = &ctx->button;

    // Update
    b->state.is_pressed = false;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        // Check if the mouse click was within the button boundaries
        if (CheckCollisionPointRec(GetMousePosition(), b->config.rec))
        {
            b->state.is_pressed = true;
        }
    }
    if (!b->state.was_pressed && b->state.is_pressed)
    {
        printf("pressed\n");
    }
    else if (b->state.was_pressed && !b->state.is_pressed)
    {
        printf("released\n");
    }
    b->state.was_pressed = b->state.is_pressed;
}

void
button1_reset(ButtonContext * const ctx)
{
    ctx->button.state.was_pressed = false;
}

ButtonContext *
button1_init(float const x, float const y, float const width, float const height)
{
    ButtonContext * ctx = calloc(1, sizeof(*ctx));
    assert(ctx != NULL);


    ctx->button.config.pressed_color = RED;
    ctx->button.config.unpressed_color = GREEN;

    ctx->button.config.rec = (Rectangle){.x = x, .y = y, .width = width, .height = height};

    return ctx;
}

void
button1_update(ButtonContext * const ctx, Environment const * const env)
{
    assert(ctx != NULL);
    ctx->env = env;

    button_update(ctx);
}

void
button1_draw(ButtonContext const * const ctx)
{
    draw_button(&ctx->button);
}

