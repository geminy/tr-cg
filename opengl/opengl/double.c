#include "common.h"

#include <GL/freeglut.h>

static GLfloat spin = 0.0;

void double_init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void double_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glRotatef(spin, 0.0, 0.0, 1.0);
    glColor3f(1.0, 1.0, 1.0);
    glRectf(-25.0, -25.0, 25.0, 25.0);
    glPopMatrix();
    glutSwapBuffers();
}

void double_spinDisplay(void)
{
    spin = spin + 2.0;
    if (spin > 360.0) {
        spin = spin - 360.0;
    }
    glutPostRedisplay();
}

void double_reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void double_mouse(int button, int state, int x, int y)
{
    switch (button) {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN) {
            glutIdleFunc(double_spinDisplay);
        }
        break;
    case GLUT_MIDDLE_BUTTON:
        if (state == GLUT_DOWN) {
            glutIdleFunc(NULL);
        }
        break;
    default:
        break;
    }
}

/*
 * Request double buffer diplay mode.
 * Register mouse input callback functions
 */
int double_main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(250, 250);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    double_init();
    glutDisplayFunc(double_display);
    glutReshapeFunc(double_reshape);
    glutMouseFunc(double_mouse);
    glutMainLoop();

    return 0;
}
