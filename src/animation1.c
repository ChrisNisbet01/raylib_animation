#include "animation1.h"

#include "dynamic_array.h"
#include "easing_functions.h"

#include <coroutine.h>

#include <raylib.h>
#include <raymath.h>

#include <assert.h>
#include <stdlib.h>

typedef struct square_animation_st
{
    struct AnimationContext * ctx;
    coroutine_t * co;
    float max_size;
    float current_size;
    float current_angle;
    float pos_x;
    float pos_y;

} square_animation_st;

typedef struct square_animations_st {
    square_animation_st * * items;
    size_t count;
    size_t capacity;
} square_animations_st;

struct AnimationContext
{
    struct schedule * schedule;

    Environment const * env;
    square_animations_st animations;
};

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

    expand_square(ani, 0.25f);
    animation_sleep(ani, 0.25f);
    rotate_square(ani, 45.f, 0.25f);
    animation_sleep(ani, 0.25f);
    rotate_square(ani, -45.f, 0.25f);
    animation_sleep(ani, 0.25f);
    shrink_square(ani, 0.25f);
}

static void
update_animations(AnimationContext * const ctx)
{
    if (coroutine_active_count(ctx->schedule) > 0)
    {
        for (size_t i = 0; i < ctx->animations.count; i++)
        {
            square_animation_st * const ani = ctx->animations.items[i];

            coroutine_resume_co(ctx->schedule, ani->co);
        }
    }
}

void
animation1_reset(AnimationContext * const ctx)
{
    for (size_t i = 0; i < ctx->animations.count; i++)
    {
        square_animation_st * const ani = ctx->animations.items[i];

        if (!coroutine_is_active(ani->co))
        {
            ani->co = coroutine_new(ani->ctx->schedule, animation_coroutine, ani, 10000);
        }
        animation1_reset_state(ani);
    }
}

AnimationContext *
animation1_init(void)
{
    AnimationContext * ctx = calloc(1, sizeof(*ctx));

    assert(ctx != NULL);

    ctx->schedule = coroutine_open();
    assert(ctx->schedule != NULL);

    int const screen_width = GetScreenWidth();
    int const screen_height = GetScreenHeight();
    int const screen_center_x = screen_width / 2;
    int const screen_center_y = screen_height / 2;
    float const step = 100.f;

    for (size_t i = 0; i < 4; i++)
    {
        square_animation_st * const ani = calloc(1, sizeof(*ani));

        assert(ani != NULL);

        ani->pos_x = (screen_center_x - step) + (i * step);
        ani->pos_y = (screen_center_y - step) + (i * step);
        ani->ctx = ctx;
        ani->max_size = 100.f;

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

        int const square_size = ani->current_size;

        DrawRectanglePro(
            (Rectangle){ ani->pos_x, ani->pos_y, square_size, square_size },
            (Vector2) { square_size / 2.f, square_size / 2.f },
            ani->current_angle,
            RED
        );
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

