#include <stdlib.h>

#include "glesUtil.h"

/**
 * @file texture2d.c
 * @brief A quad with a 2D texture image.
 */

typedef struct
{
    GLuint programObject;
    GLint  positionLoc;
    GLint  texCoordLoc;
    GLint samplerLoc;
    GLuint textureId;
} UserData;

// Create a simple 2x2 texture image with four different colors.
GLuint CreateSimpleTexture2D( )
{
    // Texture object handle.
    GLuint textureId;

    // 2x2 Image, 3 bytes per pixel (R, G, B).
    GLubyte pixels[4 * 3] =
    {
        255, 0, 0,     // Red
        0, 255, 0,     // Green
        0, 0, 255,      // Blue
        255, 255, 0   // Yellow
    };

    // Use tightly packed data.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Generate a texture object.
    glGenTextures(1, &textureId);

    // Bind the texture object.
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Load the texture.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    // Set the filtering mode.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return textureId;
}


// Initialize the shader and program object.
int Init(GLESContext *glesContext)
{
    glesContext->userData = malloc(sizeof(UserData));
    UserData *userData = glesContext->userData;
    GLbyte vShaderStr[] =
        "attribute vec4 a_position;     \n"
        "attribute vec2 a_texCoord;   \n"
        "varying vec2 v_texCoord;      \n"
        "void main()                             \n"
        "{                                               \n"
        "   gl_Position = a_position;     \n"
        "   v_texCoord = a_texCoord;  \n"
        "}                                                \n";

    GLbyte fShaderStr[] =
        "precision mediump float;                                             \n"
        "varying vec2 v_texCoord;                                              \n"
        "uniform sampler2D s_texture;                                     \n"
        "void main()                                                                     \n"
        "{                                                                                       \n"
        "  gl_FragColor = texture2D( s_texture, v_texCoord );  \n"
        "}                                                                                       \n";

    // Load the shaders and get a linked program object.
    userData->programObject = glesLoadProgram(vShaderStr, fShaderStr);

    // Get the attribute locations.
    userData->positionLoc = glGetAttribLocation(userData->programObject, "a_position");
    userData->texCoordLoc = glGetAttribLocation(userData->programObject, "a_texCoord");

    // Get the sampler location.
    userData->samplerLoc = glGetUniformLocation(userData->programObject, "s_texture");

    // Load the texture
    userData->textureId = CreateSimpleTexture2D();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    return GL_TRUE;
}

// Draw a triangle using the shader pair created in Init().
void Draw(GLESContext *glesContext )
{
    UserData *userData = glesContext->userData;
    GLfloat vVertices[] =
    {
        -0.5f, 0.5f, 0.0f,   // Position 0
        0.0f, 0.0f,             // TexCoord 0
        -0.5f, -0.5f, 0.0f,  // Position 1
        0.0f, 1.0f,             // TexCoord 1
        0.5f, -0.5f, 0.0f,   // Position 2
        1.0f, 1.0f,             // TexCoord 2
        0.5f, 0.5f, 0.0f,    // Position 3
        1.0f, 0.0f              // TexCoord 3
    };
    GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

    // Set the viewport.
    glViewport(0, 0, glesContext->width, glesContext->height);

    // Clear the color buffer.
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the program object.
    glUseProgram(userData->programObject);

    // Load the vertex position.
    glVertexAttribPointer(userData->positionLoc, 3, GL_FLOAT,
                          GL_FALSE, 5 * sizeof(GLfloat), vVertices);
    // Load the texture coordinate.
    glVertexAttribPointer(userData->texCoordLoc, 2, GL_FLOAT,
                          GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3]);

    glEnableVertexAttribArray(userData->positionLoc);
    glEnableVertexAttribArray(userData->texCoordLoc);

    // Bind the texture.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, userData->textureId);

    // Set the sampler texture unit to 0.
    glUniform1i(userData->samplerLoc, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

// Cleanup.
void ShutDown(GLESContext *glesContext)
{
   UserData *userData = glesContext->userData;

   // Delete texture object.
   glDeleteTextures(1, &userData->textureId);

   // Delete program object.
   glDeleteProgram(userData->programObject);

   free(glesContext->userData);
}

int main(int argc, char *argv[])
{
   GLESContext glesContext;
   UserData userData;

   glesInitContext(&glesContext);
   glesContext.userData = &userData;

   glesCreateWindow(&glesContext, "Simple Texture 2D", 320, 240, GLES_WINDOW_RGB);

   if (!Init(&glesContext)) {
        return 0;
   }

   glesRegisterDrawFunc(&glesContext, Draw);

   glesMainLoop(&glesContext);

   ShutDown(&glesContext);

   return 1;
}
