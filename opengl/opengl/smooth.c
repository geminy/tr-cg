#include "common.h"

#include <GL/freeglut.h>

void smooth_init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
//    glShadeModel(GL_FLAT);
}

void smooth_triangle(void)
{
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(5.0, 5.0);
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(25.0, 5.0);
    glColor3f(0.0, 0.0, 1.0);
    glVertex2f(5.0, 25.0);
    glEnd();
}

void smooth_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    smooth_triangle();
    glFlush();
}

void smooth_reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) {
        gluOrtho2D(0.0, 30.0, 0.0, 30.0 * (GLfloat)h / (GLfloat)w);
    }
    else {
        gluOrtho2D(0.0, 30.0 * (GLfloat)h / (GLfloat)w, 0.0, 30.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

int smooth_main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);
    smooth_init();
    glutDisplayFunc(smooth_display);
    glutReshapeFunc(smooth_reshape);
    glutMainLoop();

    return 0;
}

