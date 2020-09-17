#include "common.h"

#include <GL/freeglut.h>

#define NUMCOLORS 32
#define RAMPSTART 16

/*
 * Initialzie color map and fog.
 * Set screen clear color to end of color ramp.
 */
static void fogindex_init(void)
{
    int i;

    glEnable(GL_DEPTH_TEST);

    for (i = 0; i < NUMCOLORS; ++i) {
        GLfloat shade;
        shade = (GLfloat)(NUMCOLORS - i) / (GLfloat)NUMCOLORS;
        glutSetColor(RAMPSTART + i, shade, shade, shade);
    }
    glEnable(GL_FOG);

    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogi(GL_FOG_INDEX, NUMCOLORS);
    glFogf(GL_FOG_START, 1.0);
    glFogf(GL_FOG_END, 6.0);
    glHint(GL_FOG_HINT, GL_NICEST);
    glClearIndex((GLfloat)(NUMCOLORS + RAMPSTART -1));
}

static void fogindex_renderSphere(GLfloat x, GLfloat y, GLfloat z)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glutSolidSphere(0.4, 16, 16);
    glPopMatrix();
}

/* draws 5 sphere at different z positions. */
static void fogindex_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glIndexi(RAMPSTART);

    fogindex_renderSphere(-2., -0.5, -1.0);
    fogindex_renderSphere(-1., -0.5, -2.0);
    fogindex_renderSphere(0., -0.5, -3.0);
    fogindex_renderSphere(1., -0.5, -4.0);
    fogindex_renderSphere(2., -0.5, -5.0);

    glFlush();
}

static void fogindex_reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) {
        glOrtho(-2.5, 2.5, -2.5 * (GLfloat)h / (GLfloat)w, 2.5 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
    }
    else {
        glOrtho(-2.5 * (GLfloat)h / (GLfloat)w, 2.5 * (GLfloat)h / (GLfloat)w, -2.5, 2.5, -10.0, 10.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void fogindex_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

int fogindex_main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_INDEX | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow(argv[0]);
    fogindex_init();
    glutReshapeFunc(fogindex_reshape);
    glutKeyboardFunc(fogindex_keyboard);
    glutDisplayFunc(fogindex_display);
    glutMainLoop();

    return 0;
}




