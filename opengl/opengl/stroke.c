#include "common.h"

#include <GL/freeglut.h>

#include <string.h>

#define PT 1
#define STROKE 2
#define END 3

typedef struct charpoint
{
    GLfloat x, y;
    int type;
} CP;

CP Adata[] = {
    {0, 0, PT}, {0, 9, PT}, {1, 0, PT}, {4, 10, PT},
    {5, 9, PT}, {5, 0, STROKE}, {0, 5, PT}, {5, 5, END}
};

CP Edata[] = {
    {5, 0, PT}, {0, 0, PT}, {0, 10, PT}, {5, 10, STROKE},
    {0, 5, PT}, {4, 5, END}
};

CP Pdata[] = {
    {0, 0, PT}, {0, 10, PT}, {4, 10, PT}, {5, 9, PT}, {5, 6, PT},
    {4, 5, PT}, {0, 5, END}
};

CP Rdata[] = {
    {0, 0, PT}, {0, 10, PT}, {4, 10, PT}, {5, 9, PT}, {5, 6, PT},
    {4, 5, PT}, {0, 5, STROKE}, {3, 5, PT}, {5, 0, END}
};

CP Sdata[] = {
    {0, 1, PT}, {1, 0, PT}, {4, 0, PT}, {5, 1, PT}, {5, 4, PT},
    {4, 5, PT}, {1, 5, PT}, {0, 6, PT}, {0, 9, PT}, {1, 10, PT},
    {4, 10, PT}, {5, 9, END}
};

// interprets the instructions from the array
// for that letter and renders the letter with line segments.
static void stroke_drawLetter(CP *l)
{
    glBegin(GL_LINE_STRIP);
    while (1) {
        switch (l->type) {
        case PT:
            glVertex2fv(&l->x);
            break;
        case STROKE:
            glVertex2fv(&l->x);
            glEnd();
            glBegin(GL_LINE_STRIP);
            break;
        case END:
            glVertex2fv(&l->x);
            glEnd();
            glTranslatef(8.0, 0.0, 0.0);
            return;
        }
        l++;
    }
}

// Create a display list for each of 6 characters.
static void stroke_init(void)
{
    GLuint base;

    glShadeModel(GL_FLAT);

    base = glGenLists(128);
    glListBase(base);
    glNewList(base + 'A ', GL_COMBINE);
    stroke_drawLetter(Adata);
    glEndList();
    glNewList(base + 'E ', GL_COMBINE);
    stroke_drawLetter(Edata);
    glEndList();
    glNewList(base + 'P ', GL_COMBINE);
    stroke_drawLetter(Pdata);
    glEndList();
    glNewList(base + 'R ', GL_COMBINE);
    stroke_drawLetter(Rdata);
    glEndList();
    glNewList(base + 'S', GL_COMBINE);
    stroke_drawLetter(Sdata);
    glEndList();
    glNewList(base + ' ', GL_COMBINE);
    glTranslatef(8.0, 0.0, 0.0);
    glEndList();
}

char *test1 = "A SPARE SERAPE APPEARS AS ";
char *test2 = "APES PREPARE PARE PEPPERS ";

static void stroke_printStrokedString(char *s)
{
    GLsizei len = strlen(s);
    glCallLists(len, GL_BYTE, (GLbyte*)s);
}

static void stroke_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glScalef(2.0, 2.0, 2.0);
    glTranslatef(10.0, 30.0, 0.0);
    stroke_printStrokedString(test1);
    glPopMatrix();
    glPushMatrix();
    glScalef(2.0, 2.0, 2.0);
    glTranslatef(10.0, 13.0, 0.0);
    stroke_printStrokedString(test2);
    glPopMatrix();
    glFlush();
}

static void stroke_reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}

void stroke_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case ' ':
        glutPostRedisplay();
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

int stroke_main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(440, 120);
    glutCreateWindow(argv[0]);
    stroke_init();
    glutReshapeFunc(stroke_reshape);
    glutKeyboardFunc(stroke_keyboard);
    glutDisplayFunc(stroke_display);
    glutMainLoop();

    return 0;
}






