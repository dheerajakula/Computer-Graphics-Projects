#include "Utility.h"

void CircleDrawing(int radius, int val, int x_offset, int y_offset)
{
	int x = 0;
	int y = radius;

	int h = 1 - radius;

	plotCirclePixel(x, y, x_offset, y_offset);

	while (y > x)
	{
		if (h < 0) // choose E
		{
			h = h + 2 * x + 3;
		}
		else
		{
			h = h + 2 * (x - y) + 5;
			y--;
		}
		x++;
		plotCirclePixel(x, y, x_offset, y_offset);
	}
}