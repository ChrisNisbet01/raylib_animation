#include "animation1.h"

#include "easing_functions.h"

#include <coroutine.h>

#include <raylib.h>
#include <raymath.h>

#include <assert.h>
#include <stdlib.h>


struct AnimationContext
{
    struct schedule * schedule;
    coroutine_t * co;

    Environment const * env;
    float max_size;
    float current_size;
    float current_angle;
};

void
animation1_free(AnimationContext * const ctx)
{
    free(ctx);
}

static void
animation1_reset_state(AnimationContext* const ctx)
{
    ctx->current_size = 0.f;
    ctx->current_angle = 0.f;
}

static void expand_square(AnimationContext * const ctx, float const resize_time)
{
    float elapsed = 0;

    while (elapsed < resize_time)
    {
        float const f = elapsed / resize_time;

        ctx->current_size = Lerp(0.f, ctx->max_size, ease_out_cubic(f));
        elapsed += ctx->env->delta;
        coroutine_yield(ctx->schedule);
    }
    ctx->current_size = ctx->max_size;
}

static void shrink_square(AnimationContext * const ctx, float const resize_time)
{
    float elapsed = 0;

    while (elapsed < resize_time)
    {
        float const f = elapsed / resize_time;

        ctx->current_size = Lerp(ctx->max_size, 0, ease_out_cubic(f));
        elapsed += ctx->env->delta;
        coroutine_yield(ctx->schedule);
    }
    ctx->current_size = 0;
}

static void
rotate_square(AnimationContext * const ctx, float const angle_degrees, float const rotate_time)
{
    float elapsed = 0;
    float current_angle = 0;
    float start_angle = ctx->current_angle;

    while (elapsed < rotate_time)
    {
        float const f = elapsed / rotate_time;

        ctx->current_angle = start_angle + Lerp(0.f, angle_degrees, ease_out_cubic(f));
        elapsed += ctx->env->delta;
        coroutine_yield(ctx->schedule);
    }
    ctx->current_angle = start_angle + angle_degrees;
}

static void animation_sleep(AnimationContext * const ctx, float const sleep_time)
{
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
    AnimationContext * const ctx = arg;

    animation1_reset_state(ctx);

    expand_square(ctx, 0.25f);
    animation_sleep(ctx, 0.25f);
    rotate_square(ctx, 45.f, 0.25f);
    animation_sleep(ctx, 0.25f);
    rotate_square(ctx, -45.f, 0.25f);
    animation_sleep(ctx, 0.25f);
    shrink_square(ctx, 0.25f);
}

static void
update_animations(AnimationContext * const ctx)
{
    if (coroutine_active_count(ctx->schedule) > 0)
    {
        coroutine_resume(ctx->schedule);
    }
}

void
animation1_reset(AnimationContext * const ctx)
{
    if (!coroutine_is_active(ctx->co))
    {
        ctx->co = coroutine_new(ctx->schedule, animation_coroutine, ctx, 10000);
    }
    animation1_reset_state(ctx);
}

AnimationContext *
animation1_init(void)
{
    AnimationContext * ctx = calloc(1, sizeof(*ctx));

    assert(ctx != NULL);

    ctx->max_size= 100;
    ctx->schedule = coroutine_open();
    assert(ctx->schedule != NULL);

    animation1_reset(ctx);

    return ctx;
}

void
animation1_update(AnimationContext * const ctx, Environment const * const env)
{
    assert(ctx != NULL);
    ctx->env = env;

    update_animations(ctx);

    int const screen_width = GetScreenWidth();
    int const screen_height = GetScreenHeight();
    int const screen_center_x = screen_width / 2;
    int const screen_center_y = screen_height / 2;

    int const square_size = ctx->current_size;
    int const pos_x = screen_center_x;
    int const pos_y = screen_center_y;

    DrawRectanglePro(
        (Rectangle){ pos_x, pos_y, square_size, square_size },
        (Vector2) { square_size / 2.f, square_size / 2.f },
        ctx->current_angle,
        RED
    );
}

