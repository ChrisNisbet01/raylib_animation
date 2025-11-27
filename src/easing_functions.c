#include "easing_functions.h"

#include <math.h>

float
ease_out_cubic(float val)
{
    return 1.f - (1.f - powf(val, 3.f));
}

