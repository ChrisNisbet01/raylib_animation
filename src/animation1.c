#include "animation1.h"

#include "dynamic_array.h"
#include "easing_functions.h"
#include "utils.h"

#include <coroutine.h>

#include <raylib.h>
#include <raymath.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct AnimationContext AnimationContext;
typedef struct square_animation_st square_animation_st;

typedef void (*square_draw_fn)(square_animation_st * const ani);

struct square_animation_st
{
    struct AnimationContext * ctx;
    square_draw_fn draw;
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

static void
animation1_free(void * const pv)
{
    AnimationContext * const ctx = pv;
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

static float
update_fraction_complete(float const current_fraction, float const delta_time, float const total_time)
{
    float fraction_complete = current_fraction + delta_time / total_time;

    if (fraction_complete > 1.f)
    {
        fraction_complete = 1.f;
    }

    return fraction_complete;
}

static void expand_square(square_animation_st * const ani, float const resize_time)
{
    AnimationContext * const ctx = ani->ctx;
    float fraction_complete = 0.f;

    while (fraction_complete < 1.f)
    {
        fraction_complete = update_fraction_complete(
            fraction_complete, ctx->env->delta, resize_time);

        ani->current_size = Lerp(0.f, ani->max_size, ease_out_cubic(fraction_complete));
        coroutine_yield(ctx->schedule);
    }
}

static void shrink_square(square_animation_st * const ani, float const resize_time)
{
    AnimationContext * const ctx = ani->ctx;
    float fraction_complete = 0.f;

    while (fraction_complete < 1.f)
    {
        fraction_complete = update_fraction_complete(
            fraction_complete, ctx->env->delta, resize_time);

        ani->current_size = Lerp(ani->max_size, 0.f, ease_out_cubic(fraction_complete));
        coroutine_yield(ctx->schedule);
    }
}

static void
rotate_square(square_animation_st * const ani, float const angle_degrees, float const rotate_time)
{
    AnimationContext * const ctx = ani->ctx;
    float current_angle = 0;
    float start_angle = ani->current_angle;
    float fraction_complete = 0.f;

    while (fraction_complete < 1.f)
    {
        fraction_complete = update_fraction_complete(
            fraction_complete, ctx->env->delta, rotate_time);

        ani->current_angle = start_angle + Lerp(0.f, angle_degrees, ease_out_cubic(fraction_complete));
        coroutine_yield(ctx->schedule);
    }
}

static void animation_sleep(square_animation_st * const ani, float const sleep_time)
{
    AnimationContext * const ctx = ani->ctx;
    float fraction_complete = 0.f;

    while (fraction_complete < 1.f)
    {
        fraction_complete = update_fraction_complete(
            fraction_complete, ctx->env->delta, sleep_time);

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
    rotate_square(ani, 720.f + 45.f, sleep * 5);
    animation_sleep(ani, sleep);
    rotate_square(ani, -45.f, sleep);
    animation_sleep(ani, sleep);
    shrink_square(ani, sleep);
}

static void
animation_cleanup(void * const arg)
{
    square_animation_st * const ani = arg;

    (void)ani;
}

static void
update_animations(void * const pv)
{
    AnimationContext * const ctx = pv;

    if (coroutine_active_count(ctx->schedule) > 0)
    {
        coroutine_resume(ctx->schedule, coroutine_resume_all);
    }
}

static void
animation1_reset(void * const pv)
{
    AnimationContext * const ctx = pv;

    for (size_t i = 0; i < ctx->animations.count; i++)
    {
        square_animation_st * const ani = ctx->animations.items[i];

        if (ani->co != NULL)
        {
            coroutine_kill(ani->co);
        }
        coroutine_handlers_t const handlers = {
            .run = animation_coroutine,
            .cleanup = animation_cleanup,
        };
        ani->co = coroutine_new(ani->ctx->schedule, &handlers, ani, stack_size);

        animation1_reset_state(ani);
    }
}

void *
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

    ctx->sleep = 0.2f;

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

static void
animation1_update(void * const pv, Environment const * const env)
{
    AnimationContext * const ctx = pv;
    assert(ctx != NULL);
    ctx->env = env;

    update_animations(ctx);
}

static void
animation1_draw(void const * const pv)
{
    AnimationContext const * const ctx = pv;
    draw_animations(ctx);
}

static animation_handlers_st const
animation1_handlers = {
    .draw = animation1_draw,
    .free = animation1_free,
    .reset = animation1_reset,
    .update = animation1_update,
};

animation_handlers_st const *
get_animation1_animation_handlers(void)
{
    return &animation1_handlers;
}

