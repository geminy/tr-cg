#include "common.h"

#include <GL/freeglut.h>

#define RAMPSIZE 16
#define RAMP1STATE 32
#define RAMP2STATE 48

static float rotAngle = 0.;

/*
 * Initialize antialiasing for color-index mode,
 * including loading a green color ramp starting
 * at RAMP1STATE and a blue color ramp starting
 * at RAMP2STATE. The ramps must be a multiple of 16.
 */
void aaindex_init(void)
{
    int i;
    for (i = 0; i < RAMPSIZE; ++i) {
        GLfloat shade;
        shade = (GLfloat)i / (GLfloat)RAMPSIZE;
        glutSetColor(RAMP1STATE + (GLint)i, 0., shade, 0.);
        glutSetColor(RAMP2STATE + (GLint)i, 0., 0., shade);
    }
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth(1.5);

    glClearIndex((GLfloat)RAMP1STATE);
}

/* Draw 2 diagonal lines to form an X */
void aaindex_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glIndexi(RAMP1STATE);
    glPushMatrix();
    glRotatef(-rotAngle, 0.0, 0.0, 0.1);
    glBegin(GL_LINES);
        glVertex2f(-0.5, 0.5);
        glVertex2f(0.5, -0.5);
    glEnd();
    glPopMatrix();

    glIndexi(RAMP2STATE);
    glPushMatrix();
    glRotatef(-rotAngle, 0.0, 0.0, 0.1);
    glBegin(GL_LINES);
        glVertex2f(0.5, 0.5);
        glVertex2f(-0.5, -0.5);
    glEnd();
    glPopMatrix();

    glFlush();
}

void aaindex_reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) {
        gluOrtho2D(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w);
    }
    else {
        gluOrtho2D(-1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w, -1.0, 1.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void aaindex_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'r':
    case 'R':
        rotAngle += 20;
        if (rotAngle >= 360) {
            rotAngle = 0.;
        }
        glutPostRedisplay();
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

int aaindex_main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(200, 200);
    glutCreateWindow(argv[0]);
    aaindex_init();
    glutReshapeFunc(aaindex_reshape);
    glutKeyboardFunc(aaindex_keyboard);
    glutDisplayFunc(aaindex_display);
    glutMainLoop();

    return 0;
}



