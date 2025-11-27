#include "animation1.h"

#include "easing_functions.h"

#include <raylib.h>
#include <raymath.h>

#include <assert.h>
#include <stdlib.h>


struct AnimationContext
{
    float elapsed;
    float max_size;
    float current_size;
};

static void
update_context(AnimationContext * const ctx, Environment const * const env)
{
    if (ctx->elapsed < 1.f)
    {
        ctx->current_size = Lerp(0.f, ctx->max_size, ease_out_cubic(ctx->elapsed));
    }
    else
    {
        ctx->current_size = ctx->max_size;
    }
    ctx->elapsed += env->elapsed;
}

void
animation1_free(AnimationContext * const ctx)
{
    free(ctx);
}

void
animation1_reset(AnimationContext * const ctx)
{
    ctx->elapsed = 0.f;
    ctx->current_size = 0.f;
}

AnimationContext *
animation1_init(void)
{
    AnimationContext * ctx = calloc(1, sizeof(*ctx));

    assert(ctx != NULL);

    ctx->max_size= 100;
}
/*
 * Draws a red square in the center of the screen.
 */
void
animation1_update(AnimationContext * const ctx, Environment const * const env)
{
    assert(ctx != NULL);
    update_context(ctx, env);

    int const screen_width = GetScreenWidth();
    int const screen_height = GetScreenHeight();
    int const screen_center_x = screen_width / 2;
    int const screen_center_y = screen_height / 2;

    int const square_size = ctx->current_size;
    int const pos_x = screen_center_x - (square_size / 2);
    int const pos_y = screen_center_y - (square_size / 2);

    DrawRectangle(pos_x, pos_y, square_size, square_size, RED);
}

