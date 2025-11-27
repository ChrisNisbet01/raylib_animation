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
    float elapsed;
    float resize_time;
    float max_size;
    float current_size;
};

void
animation1_free(AnimationContext * const ctx)
{
    free(ctx);
}

static void
animation1_reset_state(AnimationContext* const ctx)
{
    ctx->elapsed = 0.f;
    ctx->current_size = 0.f;
    ctx->resize_time = 2.f;
}

static void
animation_coroutine(struct schedule * const s, void * const arg)
{
    AnimationContext * const ctx = arg;

    animation1_reset_state(ctx);

    while (ctx->elapsed < ctx->resize_time)
    {
        float const f = ctx->elapsed / ctx->resize_time;

        ctx->current_size = Lerp(0.f, ctx->max_size, ease_out_cubic(f));
        ctx->elapsed += ctx->env->delta;
        coroutine_yield(s);
    }
    ctx->current_size = ctx->max_size;
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

    if (coroutine_active_count(ctx->schedule) > 0)
    {
        coroutine_resume(ctx->schedule);
    }

    int const screen_width = GetScreenWidth();
    int const screen_height = GetScreenHeight();
    int const screen_center_x = screen_width / 2;
    int const screen_center_y = screen_height / 2;

    int const square_size = ctx->current_size;
    int const pos_x = screen_center_x - (square_size / 2);
    int const pos_y = screen_center_y - (square_size / 2);

    DrawRectangle(pos_x, pos_y, square_size, square_size, RED);
}

