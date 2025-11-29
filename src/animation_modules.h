#pragma once

#include "environment.h"

typedef void
(*animation_reset_fn)(void * ctx);

typedef void
(*animation_free_fn)(void * ctx);

typedef void
(*animation_update_fn)(void * ctx, Environment const * env);

typedef void
(*animation_draw_fn)(void const * ctx);

typedef struct animation_handlers_st {
    animation_reset_fn reset;
    animation_free_fn free;
    animation_update_fn update;
    animation_draw_fn draw;
} animation_handlers_st;

