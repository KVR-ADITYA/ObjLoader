#include<bits/stdc++.h>

#include<windows.h>

#include<GL/glut.h>

#include "ObjectLoader.h"

using namespace std;

string title = "The Scene";

int windowWidth = 640,
    windowHeight = 480;

float fieldOfVision           = 60.0f,
      cameraDistance          =  5.0f;

float backgroundColor[3]      = { 0, 0, 0 };                // Color: Black

int timeForUpdatingFrame      = 25;

const int KEY_ESCAPE          = 27;

bool isWireframeModeEnabled   = false;                      // Default: false

/* Lighting Settings */
// Set material properties for lighting
bool isLightingEnabled        = true,
     isSpecularLightEnabled   = false;

float shininess               = 50.0f;                      // Shininess: 0 to 128;

GLfloat colorWhite[]          = { 1.00, 1.00, 1.00, 1.0 };  // Color: White
GLfloat colorDarkGray[]       = { 0.10, 0.10, 0.10, 1.0 };  // Color: Dark gray
GLfloat colorLightGray[]      = { 0.75, 0.75, 0.75, 1.0 };  // Color: Light gray

GLfloat lightPosition[]       = { 0.0, 1.0, 1.0, 0.0 };

/* Model Settings */
// -----------------------------------------------------
// *** Important *** : Change the file name if necessary
// -----------------------------------------------------
string filename = "./data/torus.obj";
ObjModelLoader *monkeyModel = NULL;
// ------------------------------------------------

float rotateAngle = 0.0f;


// void zoom(double factor)
// {
//     zoom_k *= factor;
// }

// void pan(double dx, double dy)
// {
//     pan_x += dx / zoom_k;
//     pan_y += dy / zoom_k;
// }

// void zoom_at(double x, double y, double factor)
// {
//     pan(-x, -y);
//     zoom(factor);
//     pan(x, y);
// }

// void mouse(int b, int s, int x, int y)
// {
//     if (s != GLUT_DOWN)
//         return;

//     y = win_h - 1 - y;

//     if (b == GLUT_LEFT_BUTTON)
//         zoom_at(x, y, scale);
//     else if (b == GLUT_RIGHT_BUTTON)
//         zoom_at(x, y, 1/scale);

//     glutPostRedisplay();
// }


void DrawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);                             // Switch to the drawing perspective
    glLoadIdentity();                                       // Reset the drawing perspective
    glTranslatef(0.0f, 0.0f, -cameraDistance);              // Move forward 5 units

    // Adding light
    if (isLightingEnabled)
    {
        // Set lighting intensity and color
        glLightfv(GL_LIGHT0, GL_AMBIENT,  colorDarkGray );
        glLightfv(GL_LIGHT0, GL_DIFFUSE,  colorLightGray);

        if (isSpecularLightEnabled)
        {
            glLightfv(GL_LIGHT0, GL_SPECULAR, colorWhite);

            // Setting material properties
            glMaterialfv(GL_FRONT, GL_SPECULAR, colorWhite);
            glMaterialf(GL_FRONT, GL_SHININESS, shininess); // Shininess: 0 to 128
        }

        // Set the light position
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    }

    if (isWireframeModeEnabled)
    {
        // Turn on wireframe mode
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);
    }
    else
    {
        // Turn off wireframe mode
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
    }

    glPushMatrix();

    // Drawing a cube
    glColor3f(0.60, 0.60, 0.60);                            // Color: Gray
    glRotatef(rotateAngle, false, true, false);

    // ------------------------------------------------
    monkeyModel->Draw();
    // ------------------------------------------------

    glPopMatrix();

    glutSwapBuffers();
}


void Update(int value)
{
    rotateAngle += 0.5f;

    if ((rotateAngle > 360) || (rotateAngle < -360))
    {
        rotateAngle = 0.0f;
    }

    glutPostRedisplay();                                    // Tell GLUT that the display has changed

    // Tell GLUT to call update again in 25 milliseconds
    glutTimerFunc(timeForUpdatingFrame, Update, 0);
}


// Called when a key is pressed
void HandleKeypress(unsigned char key, int x, int y) {
    switch (key)
    {
        // Escape key
        case KEY_ESCAPE:
            exit(EXIT_SUCCESS);
            break;

        // Toggle Wireframe mode
        case 'w':
            isWireframeModeEnabled = !isWireframeModeEnabled;
            isLightingEnabled      = !isLightingEnabled;
            break;
    }
}



// Called when the window is resized
void HandleResize(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fieldOfVision, (double) width / (double) height, 1.0, 200.0);
}

int main(int argc, char* argv[])
{
    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);

    //Create the window
    glutCreateWindow(title.c_str());

    // Initialize
    glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], 1.0f);
    glClearDepth(1.0f);                                 // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);                            // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);                             // Set the type of depth-test
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);                            // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

    // Lighting set up
    if (isLightingEnabled)
    {
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
    }

    // ------------------------------------------------
    monkeyModel = new ObjModelLoader(filename);
    // ------------------------------------------------

    // Set handler functions
    glutDisplayFunc(DrawScene);
    glutReshapeFunc(HandleResize);
    glutKeyboardFunc(HandleKeypress);
    glutTimerFunc(timeForUpdatingFrame, Update, 0);     // Add a timer

    glutMainLoop();
    return EXIT_SUCCESS;
}
