#pragma once

#include "animation_modules.h"

#include "environment.h"


void *
button1_init(float x, float y, float width, float height);

animation_handlers_st const *
get_button_animation_handlers(void);

