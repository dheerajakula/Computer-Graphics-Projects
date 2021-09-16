#include <algorithm>
#include <GL/glut.h>
#include <iostream>
#include "Utility.h"
#include "tree.h"


using namespace std;
int ww = 800, wh = 800;
int xi, yi, xf, yf;
bool firstClick = true;

using namespace std;

bool drawLine = false;
tree* t;
void mouse(int btn, int state, int x, int y)
{
    if (btn == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if (firstClick) {
            cout << x <<" "<< y << endl;
            xi = x;
            yi = (wh - y);
            firstClick = false;
            drawLine = false;
        }
        else {
            cout << x << " " << y << endl;
            xf = x;
            yf = (wh - y);
            firstClick = true;
            drawLine = true;
        }
    }
    glutPostRedisplay();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: // Press escape to exit.
        exit(0);
        break;
    default:
        break;
    }
}

// Drawing (display) routine.
void drawScene(void)
{
    glClearColor(0.4, 0.7, 0.5, 1.0); // Set foreground color
    glColor3f(0.2, 0.3, 0.3); // Clear screen to background color.
    glClear(GL_COLOR_BUFFER_BIT);   //Flush created objects to the screen, i.e., force rendering.
    if (drawLine)
    {
        glColor3f(0.0, 0.0, 0.0); //Set pixel to black  
        glBegin(GL_POINTS);
        plotLine(xi, yi, xf, yf);
        /*plotCircle(20, 1, 300,200);
        plotCircle(10, 1, 315, 140);*/
        glEnd();
    }
    glFlush();
}

void draw_tree(void)

{
    glClearColor(0.4, 0.7, 0.5, 1.0); // Set foreground color
    glColor3f(0.2, 0.3, 0.3); // Clear screen to background color.
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0); 
    glPointSize(2.0);
    glBegin(GL_POINTS);
    t->Tidier();
    glEnd();
    
    
    glFlush();
}

// OpenGL window reshape routine.
void setup()
{
    glViewport(0, 0, ww, wh); // Set viewport size to be entire OpenGL window.
    glMatrixMode(GL_PROJECTION); // Set matrix mode to projection.
    glLoadIdentity(); // Clear current projection matrix to identity.
    gluOrtho2D(0.0, (GLdouble)ww, 0.0, (GLdouble)wh); // Specify the orthographic (or perpendicular) projection, i.e., define the viewing box.
    glMatrixMode(GL_MODELVIEW); // Set matrix mode to modelview.
}

// Main routine: defines window properties, creates window, registers callback routines and begins processing.
int main(int argc, char** argv)
{
    t = new tree(32);
    glutInit(&argc, argv); // Initialize GLUT.
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Set display mode as single-buffered and RGB color.
    glutInitWindowSize(ww, wh); // Set OpenGL window size.
    glutInitWindowPosition(0, 0); // Set position of OpenGL window upper-left corner.
    glutCreateWindow("Assignment1"); // Create OpenGL window with title.
    //glutDisplayFunc(draw_tree); // Register display routine.
    glutDisplayFunc(draw_tree); // Register display routine.
    setup(); // Register reshape routine.
    glutKeyboardFunc(keyInput); // Register keyboard routine.
    glutMouseFunc(mouse); // Begin processing.
    glutMainLoop();

    return 0;
}