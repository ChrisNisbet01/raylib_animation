#include "animation1.h"

#include <raylib.h>

/*
 * Draws a red square in the center of the screen.
 */
void
DrawAnimation1(void)
{
    int const screen_width = GetScreenWidth();
    int const screen_height = GetScreenHeight();
	int const screen_center_x = screen_width / 2;
	int const screen_center_y = screen_height / 2;

    int const square_size = 100;
    int const pos_x = screen_center_x - (square_size / 2);
    int const pos_y = screen_center_y - (square_size / 2);

    DrawRectangle(pos_x, pos_y, square_size, square_size, RED);
}
