#pragma once

#include "environment.h"

typedef struct ButtonContext ButtonContext;

void
button1_reset(ButtonContext * ctx);

ButtonContext *
button1_init(float x, float y, float width, float height);

void
button1_free(ButtonContext * ctx);

void
button1_update(ButtonContext * ctx, Environment const * env);

void
button1_draw(ButtonContext const * ctx);

