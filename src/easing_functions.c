#include "easing_functions.h"

#include <math.h>

float
ease_out_cubic(float const val)
{
    return 1.f - (1.f - powf(val, 3.f));
}

float
ease_in_cubic(float const val)
{
    return val * val * val;
}

float
ease_out_quint(float const val)
{
    return 1 - powf(1.f - val, 5);
}

