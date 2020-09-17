#include "common.h"

#include <GL/freeglut.h>

static int leftFirst = GL_TRUE;

/* Initialize alpha blending function. */
void alpha_init(void)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_FLAT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void alpha_drawLeftTriangle(void)
{
    // draw yellow triangle on LHS of screen
    glBegin(GL_TRIANGLES);
        glColor4f(1.0, 1.0, 0.0, 0.75);
        glVertex3f(0.1, 0.9, 0.0);
        glVertex3f(0.1, 0.1, 0.0);
        glVertex3f(0.7, 0.5, 0.0);
    glEnd();
}

void alpha_drawRightTriangle(void)
{
    // draw cyan triangle on RHS of screen
    glBegin(GL_TRIANGLES);
        glColor4f(0.0, 1.0, 1.0, 0.75);
        glVertex3f(0.9, 0.9, 0.0);
        glVertex3f(0.3, 0.5, 0.0);
        glVertex3f(0.9, 0.1, 0.0);
    glEnd();
}

void alpha_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    if (leftFirst) {
        alpha_drawLeftTriangle();
        alpha_drawRightTriangle();
    }
    else {
        alpha_drawRightTriangle();
        alpha_drawLeftTriangle();
    }
    glFlush();
}

void alpha_reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) {
        gluOrtho2D(0.0, 1.0, 0.0, 1.0 * (GLfloat)h / (GLfloat)w);
    }
    else {
        gluOrtho2D(0.0, 1.0 * (GLfloat)h / (GLfloat)w, 0.0, 1.0);
    }
}

void alpha_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 't':
    case 'T':
        leftFirst = !leftFirst;
        glutPostRedisplay();
        break;
    case 27: // Escape key
        exit(0);
        break;
    default:
        break;
    }
}

int alpha_main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(200, 200);
    glutCreateWindow(argv[0]);
    alpha_init();
    glutReshapeFunc(alpha_reshape);
    glutKeyboardFunc(alpha_keyboard);
    glutDisplayFunc(alpha_display);
    glutMainLoop();

    return 0;
}
