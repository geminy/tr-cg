#include <stdlib.h>

#include "glesUtil.h"

typedef struct
{
   GLuint programObject; // Handle to a program object.
} UserData4Triangle;

/**
 * @brief Create a shader object, load the shader source, and compile the shader.
 * @param type
 * @param shaderSrc
 */
GLuint LoadShader4Trangle(GLenum type, const char *shaderSrc)
{
   GLuint shader;
   GLint compiled;

   shader = glCreateShader(type);

   if (0 == shader) {
    return 0;
   }

   glShaderSource(shader, 1, &shaderSrc, NULL);

   glCompileShader(shader);

   glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

   if (!compiled) {
      GLint infoLength = 0;

      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);

      if (infoLength > 1) {
         char *info = malloc(sizeof(char) * infoLength);

         glGetShaderInfoLog(shader, infoLength, NULL, info);
         glesLogMessage("[Error] [Compiling Shader]\n%s\n", info);

         free(info);
      }

      glDeleteShader(shader);
      return 0;
   }

   return shader;
}

/**
 * @brief Initialize the shader and program object.
 * @param glesContext Applicaion context.
 */
int InitTriangle(GLESContext *glesContext)
{
   glesContext->userData = malloc(sizeof(UserData4Triangle));

   UserData4Triangle *userData = glesContext->userData;
   GLbyte vShaderStr[] =
      "attribute vec4 vPosition;  \n"
      "void main()                        \n"
      "{                                          \n"
      "   gl_Position = vPosition; \n"
      "}                                          \n";

   GLbyte fShaderStr[] =
      "precision mediump float;                       \n"
      "void main()                                               \n"
      "{                                                                 \n"
      "  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); \n"
      "}                                                                 \n";

   GLuint vertexShader;
   GLuint fragmentShader;
   GLuint programObject;
   GLint linked;

   vertexShader = LoadShader4Trangle(GL_VERTEX_SHADER, vShaderStr);
   fragmentShader = LoadShader4Trangle(GL_FRAGMENT_SHADER, fShaderStr);

   programObject = glCreateProgram();

   if (0 == programObject) {
      return 0;
   }

   glAttachShader(programObject, vertexShader);
   glAttachShader(programObject, fragmentShader);

   // Bind vPosition to attribute 0.
   glBindAttribLocation(programObject, 0, "vPosition");

   glLinkProgram(programObject);

   glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

   if (!linked) {
      GLint infoLength = 0;

      glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLength);

      if (infoLength > 1) {
         char *info = malloc(sizeof(char) * infoLength);

         glGetProgramInfoLog(programObject, infoLength, NULL, info);
         glesLogMessage("Error linking program:\n%s\n", info);

         free(info);
      }

      glDeleteProgram(programObject);
      return GL_FALSE;
   }

   userData->programObject = programObject;

   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   return GL_TRUE;
}

/**
 * @brief Draw a triangle using the shader pair created in InitTriangle().
 * @param glesContext Applicaion context.
 */
void DrawTriangle(GLESContext *glesContext)
{
   UserData4Triangle *userData = glesContext->userData;
   GLfloat vVertices[] =
   {
       0.0f,  0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f
   };

   glViewport(0, 0, glesContext->width, glesContext->height);

   glClear(GL_COLOR_BUFFER_BIT);

   glUseProgram(userData->programObject);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
   glEnableVertexAttribArray(0);

   glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(int argc, char *argv[])
{
   GLESContext glesContext;
   UserData4Triangle userData;

   glesInitContext(&glesContext);
   glesContext.userData = &userData;

   glesCreateWindow(&glesContext, "Hello Triangle", 320, 240, GLES_WINDOW_RGB);

   if (!InitTriangle(&glesContext)) {
      return 0;
   }

   glesRegisterDrawFunc(&glesContext, DrawTriangle);

   glesMainLoop(&glesContext);

   return 1;
}
