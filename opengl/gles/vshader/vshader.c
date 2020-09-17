#include <stdlib.h>

#include "glesUtil.h"

/**
 * @file vshader.c
 * @brief A rotating cube in perspective.
 *
 * Using a vertex shader to transform the object.
 */

typedef struct
{
    GLuint          programObject; // Handle to program object
    GLint            positionLoc; // Attribute locations
    GLint            mvpLoc; // Uniform locations
    GLfloat         *vertices; // For vertex
    GLuint          *indices; // For vertex
    int                 numIndices; // For vertex
    GLfloat         angle; // Rotation angle
    GLESMatrix  mvpMatrix; // MVP matrix
} UserData;

// Initialize the shader and program object.
int Initialize(GLESContext *glesContext)
{
    UserData *userData = malloc(sizeof(UserData));
    glesContext->userData = userData;

    GLbyte vShaderStr[] =
        "uniform mat4 u_mvpMatrix;                        \n"
        "attribute vec4 a_position;                             \n"
        "void main()                                                     \n"
        "{                                                                       \n"
        "   gl_Position = u_mvpMatrix * a_position;  \n"
        "}                                                                       \n";

    GLbyte fShaderStr[] =
        "precision mediump float;                          \n"
        "void main()                                                  \n"
        "{                                                                    \n"
        "  gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );  \n"
        "}                                                                    \n";

    // Load shaders and get a linked program object.
    userData->programObject = glesLoadProgram(vShaderStr, fShaderStr);

    // Get the attribute locations.
    userData->positionLoc = glGetAttribLocation(userData->programObject, "a_position");

    // Get the uniform locations.
    userData->mvpLoc = glGetUniformLocation(userData->programObject, "u_mvpMatrix");

    // Generate the vertex data.
    userData->numIndices = glesGenCube(1.0,
                                       &userData->vertices, NULL, NULL, &userData->indices);

    // Starting rotation angle for the cube.
    userData->angle = 45.0f;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    return GL_TRUE;
}

// Update MVP matrix based on time.
void Update(GLESContext *glesContext, float deltaTime)
{
   UserData *userData = (UserData*)glesContext->userData;
   GLESMatrix perspective;
   GLESMatrix modelView;
   float aspect;

   // Compute a rotation angle based on time to rotate the cube.
   userData->angle += (deltaTime * 40.0f);
   if(userData->angle >= 360.0f) {
      userData->angle -= 360.0f;
   }

   // Compute the window aspect ratio.
   aspect = (GLfloat)glesContext->width / (GLfloat)glesContext->height;

   // Generate a perspective matrix with a 60 degree FOV.
   glesMatrixLoadIdentity(&perspective);
   glesPerspective(&perspective, 60.0f, aspect, 1.0f, 20.0f);

   // Generate a model view matrix to rotate/translate the cube.
   glesMatrixLoadIdentity(&modelView);

   // Translate away from the viewer.
   glesTranslate(&modelView, 0.0, 0.0, -2.0);

   // Rotate the cube.
   glesRotate(&modelView, userData->angle, 1.0, 0.0, 1.0);

   // Compute the final MVP by multiplying the
   // modeVleiw and perspective matrices together.
   glesMatrixMultiply(&userData->mvpMatrix, &modelView, &perspective);
}

// Draw a triangle using the shader pair created in Initialize().
void Draw(GLESContext *glesContext)
{
   UserData *userData = glesContext->userData;

   // Set the viewport.
   glViewport(0, 0, glesContext->width, glesContext->height);

   // Clear the color buffer.
   glClear(GL_COLOR_BUFFER_BIT);

   // Use the program object.
   glUseProgram(userData->programObject);

   // Load the vertex position.
   glVertexAttribPointer(userData->positionLoc,
                         3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), userData->vertices);

   glEnableVertexAttribArray(userData->positionLoc);

   // Load the MVP matrix.
   glUniformMatrix4fv(userData->mvpLoc, 1, GL_FALSE, (GLfloat*)&userData->mvpMatrix.m[0][0]);

   // Draw the cube.
   glDrawElements(GL_TRIANGLES, userData->numIndices, GL_UNSIGNED_INT, userData->indices);
}

// Cleanup.
void ShutDown(GLESContext *glesContext)
{
   UserData *userData = glesContext->userData;

   if (NULL != userData->vertices) {
      free(userData->vertices);
   }

   if (NULL != userData->indices) {
      free(userData->indices);
   }

   // Delete program object.
   glDeleteProgram(userData->programObject);

   free(userData);
}

int main(int argc, char *argv[])
{
   GLESContext glesContext;
   UserData userData;

   glesInitContext(&glesContext);
   glesContext.userData = &userData;

   glesCreateWindow(&glesContext, "Simple Texture 2D", 320, 240, GLES_WINDOW_RGB);

   if (!Initialize(&glesContext)) {
      return 0;
    }
   glesRegisterDrawFunc(&glesContext, Draw);
   glesRegisterUpdateFunc(&glesContext, Update);

   glesMainLoop(&glesContext);

   ShutDown(&glesContext);
   return 1;
}
