#pragma once

#include "environment.h"

typedef struct AnimationContext AnimationContext;

void
animation1_reset(AnimationContext * ctx);

AnimationContext *
animation1_init(void);

void
animation1_free(AnimationContext * ctx);

/*
 * Draws a red square in the center of the screen.
 */
void
animation1_update(AnimationContext * ctx, Environment const * env);

void
animation1_draw(AnimationContext const * const ctx);

