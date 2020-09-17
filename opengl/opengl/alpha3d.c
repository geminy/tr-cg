#include "common.h"

#include <GL/freeglut.h>

#define MAXZ 8.0
#define MINZ -8.0
#define ZINC 0.4

static float solidZ = MAXZ;
static float transparentZ = MINZ;
static GLuint sphereList, cubeList;

void alpha3d_init(void)
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 0.15 };
    GLfloat mat_shininess[] = { 100.0 };
    GLfloat position[] = { 0.5, 0.5, 1.0, 0.0 };

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    sphereList = glGenLists(1);
    glNewList(sphereList, GL_COMPILE);
        glutSolidSphere(0.4, 16, 16);
    glEndList();

    cubeList = glGenLists(1);
    glNewList(cubeList, GL_COMPILE);
        glutSolidCube(0.6);
    glEndList();
}

void alpha3d_display(void)
{
    GLfloat mat_solid[] = { 0.75, 0.75, 0.0, 1.0 };
    GLfloat mat_zero[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_transparent[] = { 0.0, 0.8, 0.8, 0.6 };
    GLfloat mat_emission[] = { 0.0, 0.3, 0.3, 0.6 };

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
        glTranslatef(-0.15, -0.15, solidZ);
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_zero);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_solid);
        glCallList(sphereList);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(0.15, 0.15, transparentZ);
        glRotatef(15.0, 1.0, 1.0, 0.0);
        glRotatef(30.0, 0.0, 1.0, 0.0);
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_transparent);
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glCallList(cubeList);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
    glPopMatrix();

    glutSwapBuffers();
}

void alpha3d_reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h) {
        glOrtho(-1.5, 1.5, -1.5 * (GLfloat)h / (GLfloat)w, 1.5 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
    }
    else {
        glOrtho(-1.5 * (GLfloat)h / (GLfloat)w, 1.5 * (GLfloat)h / (GLfloat)w, -1.5, 1.5, -10.0, 10.0);
    }
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void alpha3d_animate(void)
{
    if (solidZ <= MINZ || transparentZ >= MAXZ) {
        glutIdleFunc(NULL);
    }
    else {
        solidZ -= ZINC;
        transparentZ += ZINC;
        glutPostRedisplay();
    }
}

void alpha3d_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'a':
    case 'A':
        solidZ = MAXZ;
        transparentZ = MINZ;
        glutIdleFunc(alpha3d_animate);
        break;
    case 'r':
    case 'R':
        solidZ = MAXZ;
        transparentZ = MINZ;
        glutPostRedisplay();
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

int alpha3d_main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow(argv[0]);
    alpha3d_init();
    glutReshapeFunc(alpha3d_reshape);
    glutKeyboardFunc(alpha3d_keyboard);
    glutDisplayFunc(alpha3d_display);
    glutMainLoop();

    return 0;
}

