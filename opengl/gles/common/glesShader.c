#include <stdlib.h>

#include "glesUtil.h"

GLuint GLES_API glesLoadShader(GLenum type, const char *shaderSrc)
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
         glesLogMessage( "[Error] [Compiling Shader]\n%s\n", info);

         free(info);
      }

      glDeleteShader(shader);
      return 0;
   }

   return shader;
}

GLuint GLES_API glesLoadProgram(const char *vertShaderSrc, const char *fragShaderSrc)
{
   GLuint vertexShader;
   GLuint fragmentShader;
   GLuint programObject;
   GLint linked;

   vertexShader = glesLoadShader(GL_VERTEX_SHADER, vertShaderSrc);
   if (0 == vertexShader) {
      return 0;
   }

   fragmentShader = glesLoadShader(GL_FRAGMENT_SHADER, fragShaderSrc);
   if (0 == fragmentShader) {
      glDeleteShader(vertexShader);
      return 0;
   }

   programObject = glCreateProgram();

   if (0 == programObject) {
      return 0;
   }

   glAttachShader(programObject, vertexShader);
   glAttachShader(programObject, fragmentShader);

   glLinkProgram(programObject);

   glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

   if (!linked) {
      GLint infoLength = 0;

      glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLength);

      if (infoLength > 1) {
         char *info = malloc(sizeof(char) * infoLength);

         glGetShaderInfoLog(programObject, infoLength, NULL, info);
         glesLogMessage( "[Error] [Linking Program]\n%s\n", info);

         free(info);
      }

      glDeleteProgram(programObject);
      return 0;
   }

   // Free up no longer needed shader resources.
   glDeleteShader(vertexShader);
   glDeleteShader(fragmentShader);

   return programObject;
}
