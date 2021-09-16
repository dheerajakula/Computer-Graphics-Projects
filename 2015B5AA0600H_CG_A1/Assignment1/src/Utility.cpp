#include <GL/glut.h>
#include "Utility.h"
#include "MidpointLineDrawing.h"
#include "CircleDrawing.h"

void plotPixel(int x, int y)
{
	glVertex2i(x, y);
}

void plotLine(int x0, int y0, int x1, int y1)
{
	MidpointLineDrawing(x0, y0, x1, y1);
}

void plotCirclePixel(int x, int y, int x_offset, int y_offset)
{
	// 8 way symmetry
	glVertex2i(x + x_offset, y + y_offset);
	glVertex2i(y + x_offset, x + y_offset);
	glVertex2i(y + x_offset, -x + y_offset);
	glVertex2i(x + x_offset, -y + y_offset);
	glVertex2i(-x + x_offset, -y + y_offset);
	glVertex2i(-y + x_offset, -x + y_offset);
	glVertex2i(-y + x_offset, x + y_offset);
	glVertex2i(-x + x_offset, y + y_offset);

}

void plotCircle(int radius, int val, int x_offset, int y_offset)
{
	CircleDrawing(radius, val, x_offset, y_offset);
}