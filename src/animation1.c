#include "animation1.h"

#include "raylib.h" /* External headers */

/*
 * Draws a red square in the center of the screen.
 */
void
DrawRedSquareInMiddle(void)
{
    /* ## 3. Variables and Types - Declaration Scope */
    /* ## 5. Const Correctness - Use Const Aggressively */
    int const screen_width = GetScreenWidth();
    int const screen_height = GetScreenHeight();

    /* ## 1. Formatting - Whitespace After Declarations */

    int const square_size = 100;

    int const pos_x = (screen_width / 2) - (square_size / 2);
    int const pos_y = (screen_height / 2) - (square_size / 2);

    /* ## 1. Formatting - Whitespace After Declarations */

    DrawRectangle(pos_x, pos_y, square_size, square_size, RED);
}
