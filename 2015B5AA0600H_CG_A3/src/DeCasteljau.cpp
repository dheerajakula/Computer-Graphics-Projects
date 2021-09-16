//#include <math.h>
//#include <gl/glew.h>
//#include <GL/glu.h>
//#include <GL/glut.h>
//#include <vector>
//#include <utility>
//#include <iostream>
//#define NO_POINTS 100
//
////Global Points for x and y coordinates 
//int x[NO_POINTS];
//int y[NO_POINTS];
//int q = 0, k = 0;
//std::vector <std::pair <int, int>> r;
//
////For Drawing bezier between two calculated points
//double oldX, oldY, currentX, currentY;
//
////Display Function
//void display(void)
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//	glFlush();
//}
//
////Initiate values for display
//void init(void)
//{
//	glClearColor(0.529412, 0.807843, 0.921569, 0.0f);
//	glColor3f(0.545098, 0.270588, 0.0745098);
//	glPointSize(4.0);
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	//Set up orthographics matrix
//	gluOrtho2D(0, 1280, 0, 720);
//}
//
////Draw line between passed set of 2 points
//void drawLine(float l, float m, float j, float i)
//{
//	glBegin(GL_LINES);
//	glVertex2f(l, m);
//	glVertex2f(j, i);
//	glEnd();
//	glFlush();
//}
//
////Drawing a point
//void drawDot(int l, int m)
//{
//	glBegin(GL_POINTS);
//	glVertex2i(l, m);
//	glEnd();
//	glFlush();
//}
//
////De Casteljau's algorithm with recursion
//void deCas(std::vector <std::pair <int, int>> r, double t)
//{
//	if (r.size() == 1)
//	{
//		drawDot(r[0].first, 720 - r[0].second);
//		return;
//	}
//	else
//	{
//		std::vector <std::pair <int, int>> newpoints(r.size() - 1);
//		for (int i = 0; i < newpoints.size(); i++)
//		{
//			int x = (1 - t) * r[i].first + t * r[i + 1].first;
//			int y = (1 - t) * r[i].second + t * r[i + 1].second;
//			newpoints[i] = std::make_pair(x, y);
//		}
//		deCas(newpoints, t);
//	}
//}
//
////Draw points and curve using De Casteljau's 
//void placeDots(int button, int state, int i, int j)
//{
//	int m = 0;
//	bool iscoincide = false;
//	if (state == GLUT_DOWN)
//	{
//		if (button == GLUT_LEFT_BUTTON)
//		{
//			if (q <= 1) // to check no. of points array have, if it is only 1 or 2, code to draw just dots 
//			{
//				x[q] = (int)i;
//				y[k] = (int)j;
//				std::pair <int, int> p;
//				p = std::make_pair(x[q], y[k]);
//				r.push_back(p);
//				drawDot(x[q], 720 - y[k]);	//Drawing the points clicked by mouse
//				//For the first point in bezier
//				if (q == 0)oldX = x[q];
//				if (q == 0)oldY = y[q];
//				q++;
//				k++;
//			}
//			else //calculation 0f decasteljau and plotting starts from 3 points
//			{
//				int d = 0;
//
//				std::vector <std::pair <int, int>>::iterator it;
//				for (it = r.begin(); it != r.end(); ++it, d++)
//				{
//					if (i < r[d].first + 10 && i > r[d].first - 10 && j < r[d].second + 10 && j > r[d].second - 10)
//					{
//						m = d;
//						iscoincide = true;
//						break;
//					}
//				}
//				if (iscoincide == false)
//				{
//					x[q] = (int)i;
//					y[k] = (int)j;
//					std::pair <int, int> p;
//					p = std::make_pair(x[q], y[k]);
//					r.push_back(p);
//					std::cout << " " << r.size() << std::endl;
//					drawDot(x[q], 720 - y[k]);
//					glClearColor(0.529412, 0.807843, 0.921569, 0.0f);
//					for (double t = 0; t < 1.0; t = t + 0.001)
//						deCas(r, t);
//					q++;
//					k++;
//				}
//			}
//		}
//		else if (button == GLUT_RIGHT_BUTTON) // To delete a control point on mouse right click
//		{
//			int d = 0;
//			std::vector <std::pair <int, int>>::iterator it;
//			for (it = r.begin(); it != r.end(); ++it, d++)
//			{
//				if (i < r[d].first + 10 && i > r[d].first - 10 && j < r[d].second + 10 && j > r[d].second - 10)
//				{
//					r.erase(it);
//					break;
//				}
//			}
//			glClearColor(0.529412, 0.807843, 0.921569, 0.0f);
//			for (double t = 0; t < 1.0; t = t + 0.001)
//				deCas(r, t);
//			return;
//		}
//
//	}
//	if (state == GLUT_UP) // mouse drag to move control points
//	{
//
//		if (m != 0)
//		{
//			std::pair <int, int> b;
//			b = std::make_pair(i, j);
//			r[m] = b;
//			glClearColor(0.529412, 0.807843, 0.921569, 0.0f);
//			for (double t = 0; t < 1.0; t = t + 0.001)
//				deCas(r, t);
//		}
//		return;
//
//	}
//}
//
////Function to intialize all values
//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//	glutInitWindowSize(1280, 720);
//	glutInitWindowPosition(0, 0);
//	glutCreateWindow("De Casteljau's Algorithm");
//	glutMouseFunc(placeDots);
//	glutDisplayFunc(display);
//	init();
//	glutMainLoop();
//	return 0;
//}