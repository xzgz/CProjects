#include <glut.h>
#include "Matrices.h"
#include "Vectors.h"

// GLUT CALLBACK functions
void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);
void keyboardCB(unsigned char key, int x, int y);
void mouseCB(int button, int stat, int x, int y);
void mouseMotionCB(int x, int y);
void initGL();
void initLights();
void drawAxis(float size = 0.6f);

const int   SCREEN_WIDTH    = 600;
const int   SCREEN_HEIGHT   = 600;
const float CAMERA_DISTANCE = 1.5f;
const int   TEXT_WIDTH      = 8;
const int   TEXT_HEIGHT     = 13;
const float DEG2RAD         = 3.141593f / 180;

// global variables
int screenWidth;
int screenHeight;
bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraDistance;
Matrix4 matrixView;
Matrix4 matrixModel;
Matrix4 matrixModelView;
Matrix4 matrixProjection;

int main(int argc, char **argv)
{
    // init global vars
    screenWidth = SCREEN_WIDTH;
    screenHeight = SCREEN_HEIGHT;
    mouseLeftDown = mouseRightDown = false;
    mouseX = mouseY = 0;
    cameraAngleX = cameraAngleY = 0;
    cameraDistance = CAMERA_DISTANCE;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );                 // display mode
    glutInitWindowSize(screenWidth, screenHeight);                             // window size
    glutInitWindowPosition(100, 100);                                          // window location
    glutCreateWindow(argv[0]);                                                 // param is the title of window

    glEnable(GL_DEPTH_TEST);
    glutReshapeFunc(reshapeCB);
    glutDisplayFunc(displayCB);
    glutTimerFunc(33, timerCB, 33);                                           // redraw only every given millisec

    glutKeyboardFunc(keyboardCB);
    glutMouseFunc(mouseCB);
    glutMotionFunc(mouseMotionCB);

    glutMainLoop();

    return 0;
}

void displayCB()
{
    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // transform camera
    matrixView.identity();
    matrixView.rotate(cameraAngleX, 1, 0, 0);     //cameraAngleX
//    glMultMatrixf(matrixView.get());
	glMultMatrixf(matrixView.get());
    matrixView.identity();
    matrixView.rotate(cameraAngleY, 0, 1, 0);     //cameraAngleY
//    glMultMatrixf(matrixView.get());
	glMultMatrixf(matrixView.get());
    cameraAngleX = 0;
    cameraAngleY = 0;

    glColor3f(1.0f, 0.3f, 1.0f);
    glutWireTeapot(0.6f);
    drawAxis();

    glutSwapBuffers();
}

void reshapeCB(int w, int h)
{
    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(80.0f, (float)(w)/h, 0.5f, 10.0f); // FOV, AspectRatio, NearClip, FarClip
//    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -5.0f, 5.0f);

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    matrixView.identity();
    matrixView.rotate(45, 0, 1, 0);
    matrixView.translate(0, 0, -cameraDistance);

    glPushMatrix();
    glMultMatrixf(matrixView.get());
//    glMultTransposeMatrixf(matrixView.get());

    matrixView.identity();
    matrixView.rotate(45, 0, 0, 1);

    glPushMatrix();
	glMultMatrixf(matrixView.get());

    matrixView.identity();
    matrixView.rotate(45, 1, 0, 0);

    glPushMatrix();
	glMultMatrixf(matrixView.get());

//    glPopMatrix();
//    glPopMatrix();
//    glPopMatrix();

//    matrixView.identity();
//    matrixView.translate(0, 0, -cameraDistance);
}

void timerCB(int millisec)
{
    glutTimerFunc(millisec, timerCB, millisec);
    glutPostRedisplay();
}

void keyboardCB(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'x':
        cameraAngleX = 3;
        break;
    case 'X':
        cameraAngleX = -3;
        break;
    case 'y':
        cameraAngleY = 3;
        break;
    case 'Y':
        cameraAngleY = -3;
        break;
    case 27:              // ESCAPE
        exit(0);
        break;
    default:
        break;
    }
}

void mouseCB(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = y;

    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseLeftDown = true;
        }
        else if(state == GLUT_UP)
            mouseLeftDown = false;
    }
    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseRightDown = true;
        }
        else if(state == GLUT_UP)
            mouseRightDown = false;
    }
}

void mouseMotionCB(int x, int y)
{
    if(mouseLeftDown)
    {
        cameraAngleX = (x - mouseX);
        cameraAngleY = (y - mouseY);
        mouseX = x;
        mouseY = y;
    }
    if(mouseRightDown)
    {
        matrixView.translate(0, 0, cameraDistance);
        cameraDistance += (y - mouseY) * 0.01f;
        mouseY = y;
        matrixView.translate(0, 0, -cameraDistance);
    }
}

void drawAxis(float size)
{
    glDepthFunc(GL_ALWAYS);     // to avoid visual artifacts with grid lines

    // draw axis
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(size, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, size, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, size);
    glEnd();
    glLineWidth(1);

    // draw arrows(actually big square dots)
    glPointSize(5);
    glBegin(GL_POINTS);
        glColor3f(1, 0, 0);
        glVertex3f(size, 0, 0);
        glColor3f(0, 1, 0);
        glVertex3f(0, size, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0, 0, size);
    glEnd();
    glPointSize(1);

    // restore default settings
    glDepthFunc(GL_LEQUAL);
}


