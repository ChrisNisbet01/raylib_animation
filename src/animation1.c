#include "animation1.h"

#include "dynamic_array.h"
#include "easing_functions.h"

#include <coroutine.h>

#include <raylib.h>
#include <raymath.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a)/sizeof((a)[0]))
#endif

typedef struct square_animation_st square_animation_st;

typedef void (*animation_draw_fn)(square_animation_st * const ani);

struct square_animation_st
{
    struct AnimationContext * ctx;
    animation_draw_fn draw;
    coroutine_t * co;
    float max_size;
    float current_size;
    float current_angle;
    float pos_x;
    float pos_y;
    Color color;
};

typedef struct square_animations_st {
    square_animation_st * * items;
    size_t count;
    size_t capacity;
} square_animations_st;

struct AnimationContext
{
    struct schedule * schedule;
    float sleep;
    Environment const * env;
    square_animations_st animations;
};

static size_t const stack_size = 10000;

static void
draw_square_animation(square_animation_st * const ani)
{
    float const square_size = ani->current_size;
    float const origin_offset = square_size / 2.f;

    DrawRectanglePro(
        (Rectangle){ ani->pos_x, ani->pos_y, square_size, square_size },
        (Vector2) { origin_offset, origin_offset },
        ani->current_angle,
        ani->color
    );
}

void
animation1_free(AnimationContext * const ctx)
{
    square_animations_st * const animations = &ctx->animations;

    for (size_t i = 0; i < animations->count; i++)
    {
        free(animations->items[i]);
    }
    free(ctx);
}

static void
animation1_reset_state(square_animation_st * const ani)
{
    ani->current_size = 0.f;
    ani->current_angle = 0.f;
}

static void expand_square(square_animation_st * const ani, float const resize_time)
{
    AnimationContext * const ctx = ani->ctx;
    float elapsed = 0;

    while (elapsed < resize_time)
    {
        float const f = elapsed / resize_time;

        ani->current_size = Lerp(0.f, ani->max_size, ease_out_cubic(f));
        elapsed += ctx->env->delta;
        coroutine_yield(ctx->schedule);
    }
    ani->current_size = ani->max_size;
}

static void shrink_square(square_animation_st * const ani, float const resize_time)
{
    AnimationContext * const ctx = ani->ctx;
    float elapsed = 0;

    while (elapsed < resize_time)
    {
        float const f = elapsed / resize_time;

        ani->current_size = Lerp(ani->max_size, 0, ease_out_cubic(f));
        elapsed += ctx->env->delta;
        coroutine_yield(ctx->schedule);
    }
    ani->current_size = 0;
}

static void
rotate_square(square_animation_st * const ani, float const angle_degrees, float const rotate_time)
{
    AnimationContext * const ctx = ani->ctx;
    float elapsed = 0;
    float current_angle = 0;
    float start_angle = ani->current_angle;

    while (elapsed < rotate_time)
    {
        float const f = elapsed / rotate_time;

        ani->current_angle = start_angle + Lerp(0.f, angle_degrees, ease_out_cubic(f));
        elapsed += ctx->env->delta;
        coroutine_yield(ctx->schedule);
    }
    ani->current_angle = start_angle + angle_degrees;
}

static void animation_sleep(square_animation_st * const ani, float const sleep_time)
{
    AnimationContext * const ctx = ani->ctx;
    float elapsed = 0;

    while (elapsed < sleep_time)
    {
        elapsed += ctx->env->delta;
        coroutine_yield(ctx->schedule);
    }
}

static void
animation_coroutine(struct schedule * const s, void * const arg)
{
    square_animation_st * const ani = arg;

    animation1_reset_state(ani);

    AnimationContext const * const ctx = ani->ctx;
    float const sleep = ctx->sleep;

    expand_square(ani, sleep);
    animation_sleep(ani, sleep);
    rotate_square(ani, 45.f, sleep);
    animation_sleep(ani, sleep);
    rotate_square(ani, -45.f, sleep);
    animation_sleep(ani, sleep);
    shrink_square(ani, sleep);
}

static void
update_animations(AnimationContext * const ctx)
{
    if (coroutine_active_count(ctx->schedule) > 0)
    {
        coroutine_resume(ctx->schedule, coroutine_resume_all);
    }
}

void
animation1_reset(AnimationContext * const ctx)
{
    for (size_t i = 0; i < ctx->animations.count; i++)
    {
        square_animation_st * const ani = ctx->animations.items[i];

        if (ani->co != NULL)
        {
            coroutine_kill(ani->co);
        }

        ani->co = coroutine_new(ani->ctx->schedule, animation_coroutine, ani, stack_size);

        animation1_reset_state(ani);
    }
}

AnimationContext *
animation1_init(void)
{
    AnimationContext * ctx = calloc(1, sizeof(*ctx));
    float const screen_width = GetScreenWidth();
    static Color const colors[] =
    {
        LIGHTGRAY,
        GRAY,
        DARKGRAY,
        YELLOW,
        GOLD,
        ORANGE,
        PINK,
        RED,
        MAROON,
        GREEN,
        LIME,
        DARKGREEN,
    };

    assert(ctx != NULL);

    ctx->schedule = coroutine_open();
    assert(ctx->schedule != NULL);

    ctx->sleep = 0.1f;

    float const row_height = 200.f;
    float const pad = 10.f;
    float row_y = pad;

    for (size_t i = 0, j=0; i < 15; i++, j++)
    {
        square_animation_st * const ani = calloc(1, sizeof(*ani));

        assert(ani != NULL);

        ani->max_size = 40.f + (i * i);

        float const step = ani->max_size + 10.f;
        float pos_x = (ani->max_size/ 2.f) + 10 + (j * step);

        if (pos_x + ani->max_size >= GetScreenWidth())
        {
            row_y += row_height + pad;
            j = 0;
            pos_x = (ani->max_size/ 2.f) + 10 + (j * step);
        }

        ani->pos_x = (ani->max_size/ 2.f) + 10 + (j * step);
        ani->pos_y = (ani->max_size/ 2.f) + row_y;
        ani->ctx = ctx;
        ani->draw = draw_square_animation;
        ani->color = colors[(i % ARRAY_SIZE(colors))];

        da_append(&ctx->animations, ani);
    }

    animation1_reset(ctx);

    return ctx;
}

static void
draw_animations(AnimationContext const * const ctx)
{
    for (size_t i = 0; i < ctx->animations.count; i++)
    {
        square_animation_st * const ani = ctx->animations.items[i];

        ani->draw(ani);
    }
}

void
animation1_update(AnimationContext * const ctx, Environment const * const env)
{
    assert(ctx != NULL);
    ctx->env = env;

    update_animations(ctx);
    draw_animations(ctx);
}

