#include <windows.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "mesh.h"

#define M_PI 3.14159265358979323846

using namespace std;

//constants
const double SWIDTH = 600; //screen width set
const double SHEIGHT = 600; //screen height set

//globalse
string filename;
char MODE = 'a';                    //default mode
vector<glm::vec2> ctrlPts;    //to store control points 
vector<glm::vec2> ctrlPts_tangents;    //control point store
vector<glm::vec3> curvePts;      //curve points store
glm::vec2 coord_mouse;         //current mouse coordinates
int idPt = -1;           //Point to move when in the drag mode
int idTgtPt = -1;
void generateSurface();             //function to generate hermite surface


/* Identify the point that is supposed to be selected for operations
like deletion or drag. This is done by calculating the distance from the
mouse coordinates to each point and identifying the point closest to the click
event */
int identifyPoint(vector<glm::vec2> Points, glm::vec2 P)
{
    int result = -1;
    float min_dist = SHEIGHT + SWIDTH;
    for (int i = 0; i < Points.size(); i++)
    {
        glm::vec2 P2 = Points[i];
        float dist = glm::dot(P - P2, P - P2);
        if (dist < min_dist && dist < 100)
        {
            min_dist = dist;
            result = i;
        }
    }
    return result;
}


/* Handle mouse events for interaction with the control points
  LEFT CLICK = add control point  (if mode == 'a')
               move control point (if mode == 'm')

  RIGHT CLICK = delete control point nearest to the click
*/
void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    double x;
    double y;
    glfwGetCursorPos(window, &x, &y);
    y = SHEIGHT - y;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        coord_mouse = glm::vec2(x, y);                 // current mouse coordinates
        
        if(MODE=='a')                                       // if mode 'a' then add control points on click
        {
          ctrlPts.push_back(coord_mouse);    // populate the control points vector with the added control point
          ctrlPts_tangents.push_back(coord_mouse);
          MODE = 'b';
          
        }
        
        else if (MODE == 'm')
        {
          idPt = identifyPoint(ctrlPts,glm::vec2(x, y)); // identify which point to move by finding distance of all points from the mouse click position
          idTgtPt = identifyPoint(ctrlPts_tangents, glm::vec2(x, y));
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
      coord_mouse = glm::vec2(x, y);                  // current mouse position when the click is released
      if(MODE == 'm')
      {
        if(idPt!=-1)
        {
          // update the selected point
          ctrlPts.erase(ctrlPts.begin() + idPt);
          ctrlPts.insert(ctrlPts.begin() + idPt, coord_mouse);
        }

        if (idTgtPt != -1)
        {
 
            // update the selected point
            
            ctrlPts_tangents.erase(ctrlPts_tangents.begin() + idTgtPt);
            ctrlPts_tangents.insert(ctrlPts_tangents.begin() + idTgtPt, coord_mouse);
        }
      }

    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        coord_mouse = glm::vec2(x, y);
        if (MODE == 'd')
        {
            cout << "in delete mode" << endl;
            // delete event for control points mapped to right mouse click
            coord_mouse = glm::vec2(x, y);
            int i = identifyPoint(ctrlPts,glm::vec2(x, y));
            ctrlPts.erase(ctrlPts.begin() + i);
        }
        else if(MODE == 'b')
        {
            ctrlPts_tangents[ctrlPts_tangents.size() - 1] = coord_mouse;
            
            
        }
    }

    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    {
        coord_mouse = glm::vec2(x, y);
        if (MODE == 'b')
        {
            ctrlPts_tangents[ctrlPts_tangents.size() - 1] = coord_mouse;
            
           
            
        }
    }

}



/* Plot Individual Pixels on the window*/
void drawPixel(GLfloat x, GLfloat y, int pointSize, bool line=false)
{
    GLfloat pointVertex[] = {x, y};
    GLfloat rgb_vector[] = {1, 0, 0};
    if(line)
    {
      rgb_vector[1] = 1, rgb_vector[2] = 1;
    }
    glColorPointer(3, GL_FLOAT, 0, rgb_vector);

    glEnable(GL_POINT_SMOOTH);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glPointSize(pointSize);
    glVertexPointer(2, GL_FLOAT, 0, pointVertex);
    glDrawArrays(GL_POINTS, 0, 1);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisable(GL_POINT_SMOOTH);
}
//De Castlejau Linear Interpolation Function
glm::vec2 Lerp(glm::vec2 A, glm::vec2 B, float t)
{
    return (1-t)*A + t*B;
}

/* De Castlejau Algorithm Recursive Implementaion
  CurvePoints are generated recursively using linear interpolation
  and the curve point vector is populated
*/
glm::vec2 getCurvePoint(double t, vector<glm::vec2> controlPoints)
{
  if (controlPoints.size() == 1) return controlPoints[0];
  vector<glm::vec2> tmp;
  for (int i = 1; i < controlPoints.size(); i++) {
    tmp.push_back(Lerp(controlPoints[i - 1], controlPoints[i], t));
  }
  return getCurvePoint(t, tmp);
}

void drawTangentLines(vector<glm::vec2> controlPoints, vector<glm::vec2> controlPoints_tangents)
{
    glLineWidth(1);
    glColor3f(1.0, 0.0, 0.0);

    for (int i = 0; i < controlPoints.size(); i++)
    {

        glBegin(GL_LINES);
        glVertex3f(controlPoints[i].x, controlPoints[i].y, 0.0);
        glVertex3f(controlPoints_tangents[i].x, controlPoints_tangents[i].y, 0);
        glEnd();
    }
    // Axis of rotation
}

// t is a value that goes from 0 to 1 to interpolate in a C1 continuous way across uniformly sampled data points.
// when t is 0, this will return B.  When t is 1, this will return C.
float CubicHermite(float A, float B, float C, float D, float t)
{
    float a = -A / 2.0f + (3.0f * B) / 2.0f - (3.0f * C) / 2.0f + D / 2.0f;
    float b = A - (5.0f * B) / 2.0f + 2.0f * C - D / 2.0f;
    float c = -A / 2.0f + C / 2.0f;
    float d = B;

    return a * t * t * t + b * t * t + c * t + d;
}

/* Draw the Beizer Curve and the axis of rotation*/
void drawHermiteCurve(vector<glm::vec2> controlPoints)
{
    

    //recursion deapth of de castlejau algorith for linear interpolation
    int degree = controlPoints.size() - 1;

    if (degree < 0)
        return;

    // redraw all the control points
    for (int i = 0; i <= degree; i++)
    {
        drawPixel(controlPoints[i].x, controlPoints[i].y, 10);
    }

    for (int i = 0; i <= degree; i++)
    {
        drawPixel(ctrlPts_tangents[i].x, ctrlPts_tangents[i].y, 10);
    }
    curvePts.clear();

    // with t = 0.001 draw caculate the curvePoints and draw on the scene
    for(float t = 0; t < 1; t += 0.001)
    {
        glm::vec2 CP = getCurvePoint(t, controlPoints);
        //curvePoints.push_back(glm::vec3(CP.x, CP.y, 1));
        //drawPixel(CP.x, CP.y, 2, true);
    }
    for (int j = 0; j < controlPoints.size() - 1; j++)
    
    {
        for (float t = 0; t < 1; t += 0.001)
        
        {
            glm::vec2 point_temp = controlPoints[j];
            glm::vec2 point_temp_tangents = ctrlPts_tangents[j];
            glm::vec2 next_point_temp = controlPoints[j+1];
            glm::vec2 next_point_temp_tangents = ctrlPts_tangents[j+1];
            float x = CubicHermite(ctrlPts_tangents[j][0], controlPoints[j][0], controlPoints[j + 1][0], ctrlPts_tangents[j+1][0], t);
            float y = CubicHermite(ctrlPts_tangents[j][1], controlPoints[j][1], controlPoints[j + 1][1], ctrlPts_tangents[j + 1][1], t);
            curvePts.push_back(glm::vec3(x, y, 1));
            drawPixel(x, y, 2, true);
        }
    }
}

void demoPrimitiveDrawing()
{
    drawHermiteCurve(ctrlPts);
    drawTangentLines(ctrlPts, ctrlPts_tangents);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ENTER))
    {
        cout << "enter pressed";
        generateSurface();
    }

    if (glfwGetKey(window, GLFW_KEY_KP_ADD))
    {
        MODE = 'a';
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE))
    {
        MODE = 'm';
        idPt = -1;
    }
    else if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT))
    {
        MODE = 'd';
        idPt = -1;
    }
}

/* Generate the Beizer Surface by rotating sampled points about the axis of rotation*/
void generateSurface()
{
    int degree = ctrlPts.size() - 1;
    if (degree > 0)
    {
        int samples = 10;                           // samples per rotation or the number of points to considered per rotation
        float step = 2*M_PI/(float)samples;           // theta increment steps

        vector<vector <glm::vec3> > p;
        float angle;

        // generate points on the surface of the curve by rotaing curvePoints
        for (int i = 0; i < curvePts.size(); i += (1.0 * curvePts.size() / samples)) {
          vector<glm::vec3> v;
          for (double theta = M_PI/2.0; theta > -M_PI / 2.0; theta -= (M_PI/samples)){
            glm::vec3 point = curvePts[i];
            v.push_back(glm::vec3((point.x-SWIDTH/2) * cos(theta), point.y, (point.x-SWIDTH/2) * sin(theta)));
          }
          for (double theta = -M_PI/2.0; theta < M_PI / 2.0; theta += (M_PI/samples)){
            glm::vec3 point = curvePts[i];
            v.push_back(glm::vec3(-1 * (point.x-SWIDTH/2) * cos(theta), point.y, (point.x-SWIDTH/2) * sin(theta)));
          }
          p.push_back(v);
        }

        // Mesh Instance
        Mesh surface = Mesh();
        int i, j;
        // Form Quads by connecting the vertices created by rotation of
        // curve points
        for (i = 1; i < p.size(); i++) {
          for (int j = 1; j < p[i].size(); j++) {
            surface.addQuad(p[i-1][j-1], p[i-1][j], p[i][j], p[i][j-1]);
          }surface.addQuad(p[i-1][p[i].size()-1], p[i-1][0], p[i][0], p[i][p[i].size()-1]);
        }

        int len = (surface.getVertices()).size();

        // write the Hermite surface to .OFF file
        surface.writeToOFF(filename);
    }
}


int main()
{
    filename = "MyMesh";
    GLFWwindow *window;
    // Initialize the library
    if (!glfwInit()) {
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(SWIDTH, SHEIGHT, "Special Assignment", NULL, NULL);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context
    glfwMakeContextCurrent(window);

    //Mouse callback function
    glfwSetMouseButtonCallback(window, mouse_callback);

    glViewport(0.0f, 0.0f, SWIDTH, SHEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glClearColor(0.529412, 0.807843, 0.921569, 0.0f);
    glColor3f(0.545098, 0.270588, 0.0745098);
    glOrtho(0, SWIDTH, 0, SHEIGHT, 0, 1); // essentially set coordinate system
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    cout << "AFTER INSERTING THE FIRST POINT RIGHT CLICK TO ADJUST THE TANGENT AND AGAIN RIGHT CLICK TO FINALIZE" << endl;
    cout << "PRESS THE + BUTTON TO ADD A POINT "<< endl;
    cout << "PRESS THE SPACE BAR BUTTON TO MOVE A POINT " << endl;
    cout << "PRESS THE - BUTTON AND THEN RIGHT CLICK TO REMOVE A POINT " << endl;
    cout << "PRESS THE ENTER BUTTON TO GENERATE THE SURFACE OF REVOLUTION " << endl;
    

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        
        glClear(GL_COLOR_BUFFER_BIT);
        {
            demoPrimitiveDrawing();
            glfwSwapBuffers(window);
        }
        glfwPollEvents();
    }
    
    cout << curvePts.size() << endl;
    glfwTerminate();
    return 0;
}
