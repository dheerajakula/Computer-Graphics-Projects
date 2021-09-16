#include <math.h>
#include "Utility.h"

void drawLine1(int x1, int y1 ,int x2, int y2) // m > 0 and |m| < 1
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	int d_initial = 2 * dy - dx; // 2*function value at midpoint of NE and E pixel
	int d_running = d_initial;

	int incrE = 2 * dy;
	int incrNE = 2 * (dy - dx);

	int x = x1;
	int y = y1;
	plotPixel(x, y);

	while (x < x2)
	{
		if (d_running <= 0) // choose E pixel
		{
			d_running = d_running + incrE;
		}
		else // choose NE pixel
		{
			d_running = d_running + incrNE;
			y++;
		}
		x++;
		plotPixel(x, y);
	}
}

void drawLine2(int x1, int y1, int x2, int y2) // m < 0 and |m| < 1
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	int d_initial = dy + 2 * dx; // 2*function value at midpoint of S and SE pixel
	int d_running = d_initial;

	int incrS = 2 * dy;
	int incrSE = 2 * (dy + dx);

	int x = x1;
	int y = y1;
	plotPixel(x, y);

	while (x < x2)
	{
		if (d_running >= 0) // choose E pixel
		{
			d_running = d_running + incrS;
		}
		else // choose SE pixel
		{
			d_running = d_running + incrSE;
			y--;
		}
		x++;
		plotPixel(x, y);
	}
}

void drawLine3(int x1, int y1, int x2, int y2) // m > 0 and |m| < 1
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	int d_initial = dy - 2 * dx; // 2*function value at midpoint of N and NE pixel
	int d_running = d_initial;

	int incrN = -2 * dx;
	int incrNE = 2 * (dy - dx);

	int x = x1;
	int y = y1;
	plotPixel(x, y);

	while (y < y2)
	{
		if (d_running >= 0) // choose N pixel
		{
			d_running = d_running + incrN;
		}
		else // choose NE pixel
		{
			d_running = d_running + incrNE;
			x++;
		}
		y++;
		plotPixel(x, y);
	}
}

void drawLine4(int x1, int y1, int x2, int y2) // m < 0 and |m| > 1
{
	int dx = x2 - x1;
	int dy = y2 - y1;

	int d_initial = dy + 2 * dx; // 2*function value at midpoint of E and SE pixel
	int d_running = d_initial;

	int incrS = 2 * dx;
	int incrSE = 2 * (dy + dx);

	int x = x1;
	int y = y1;
	plotPixel(x, y);

	while (y > y2)
	{
		if (d_running <= 0) // choose S pixel
		{
			d_running = d_running + incrS;
		}
		else // choose SE pixel
		{
			d_running = d_running + incrSE;
			x++;
		}
		y--;
		plotPixel(x, y);
	}
}

void MidpointLineDrawing(int x0, int y0, int x1, int y1)
{
	// swap the points if the first point has greater x co-ordinate
	if (x0 > x1)
	{
		int tempx = x0;
		int tempy = y0;
		x0 = x1;
		y0 = y1;
		x1 = tempx;
		y1 = tempy;
	}

	int dx = x1 - x0;
	int dy = y1 - y0;

	if (abs(dx) >= abs(dy)) // if |m| <= 1
	{
		if (dy >= 0) // m > 0
			drawLine1(x0, y0, x1, y1);
		else
			drawLine2(x0, y0, x1, y1);
	}
	else
	{
		if (dy >= 0) // m > 0
			drawLine3(x0, y0, x1, y1);
		else
			drawLine4(x0, y0, x1, y1);
	}
}