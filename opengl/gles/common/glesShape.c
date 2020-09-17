#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "glesUtil.h"

#ifndef GLES_PI
# define GLES_PI (3.14159265f)
#endif

int GLES_API glesGenSphere(int numSlices,
                           float radius,
                           GLfloat **vertices,
                           GLfloat **normals,
                           GLfloat **texCoords,
                           GLuint **indices)
{
   int i;
   int j;
   float angleStep = (2.0f * GLES_PI) / ((float)numSlices);
   int numParallels = numSlices / 2;
   int numVertices = (numParallels + 1) * (numSlices + 1);
   int numIndices = numParallels * numSlices * 6;

   // Allocate memory for buffers.
   if (NULL != vertices) {
       *vertices = malloc(sizeof(GLfloat) * 3 * numVertices);
   }

   if (NULL != normals) {
      *normals = malloc(sizeof(GLfloat) * 3 * numVertices);
   }

   if (NULL != texCoords) {
      *texCoords = malloc(sizeof(GLfloat) * 2 * numVertices);
   }

   if (NULL != indices) {
      *indices = malloc(sizeof(GLuint) * numIndices);
   }

   for (i = 0; i < numParallels + 1; i++) {
      for (j = 0; j < numSlices + 1; j++) {
         int vertex = (i * (numSlices + 1) + j) * 3;

         if (vertices) {
            (*vertices)[vertex + 0] = radius * sinf(angleStep * (float)i) * sinf(angleStep * (float)j);
            (*vertices)[vertex + 1] = radius * cosf(angleStep * (float)i);
            (*vertices)[vertex + 2] = radius * sinf(angleStep * (float)i) * cosf(angleStep * (float)j);
         }

         if (normals) {
            (*normals)[vertex + 0] = (*vertices)[vertex + 0] / radius;
            (*normals)[vertex + 1] = (*vertices)[vertex + 1] / radius;
            (*normals)[vertex + 2] = (*vertices)[vertex + 2] / radius;
         }

         if (texCoords) {
            int texIndex = (i * (numSlices + 1) + j) * 2;
            (*texCoords)[texIndex + 0] = (float)j / (float)numSlices;
            (*texCoords)[texIndex + 1] = (1.0f - (float)i) / (float)(numParallels - 1);
         }
      }
   }

   // Generate the indices.
   if (NULL != indices) {
      GLuint *indexBuf = (*indices);
      for (i = 0; i < numParallels; i++) {
         for (j = 0; j < numSlices; j++) {
            *indexBuf++ = i * (numSlices + 1) + j;
            *indexBuf++ = (i + 1) * (numSlices + 1) + j;
            *indexBuf++ = (i + 1) * (numSlices + 1) + (j + 1);

            *indexBuf++ = i * (numSlices + 1) + j;
            *indexBuf++ = (i + 1) * (numSlices + 1) + (j + 1);
            *indexBuf++ = i * (numSlices + 1) + (j + 1);
         }
      }
   }

   return numIndices;
}

int GLES_API glesGenCube(float scale,
                         GLfloat **vertices,
                         GLfloat **normals,
                         GLfloat **texCoords,
                         GLuint **indices)
{
   int i;
   int numVertices = 24;
   int numIndices = 36;

   GLfloat cubeVertices[] =
   {
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f, -0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      -0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f,  0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f, 0.5f,
      -0.5f,  0.5f, 0.5f,
      0.5f,  0.5f, 0.5f,
      0.5f, -0.5f, 0.5f,
      -0.5f, -0.5f, -0.5f,
      -0.5f, -0.5f,  0.5f,
      -0.5f,  0.5f,  0.5f,
      -0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,
   };

   GLfloat cubeNormals[] =
   {
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, -1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, -1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      0.0f, 0.0f, 1.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      -1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
      1.0f, 0.0f, 0.0f,
   };

   GLfloat cubeTex[] =
   {
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      1.0f, 0.0f,
      1.0f, 1.0f,
      0.0f, 1.0f,
      0.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
      0.0f, 0.0f,
      0.0f, 1.0f,
      1.0f, 1.0f,
      1.0f, 0.0f,
   };

   // Allocate memory for buffers.
   if (NULL != vertices) {
      *vertices = malloc(sizeof(GLfloat) * 3 * numVertices);
      memcpy(*vertices, cubeVertices, sizeof( cubeVertices));
      for (i = 0; i < numVertices * 3; i++) {
         (*vertices)[i] *= scale;
      }
   }

   if (NULL != normals) {
      *normals = malloc(sizeof(GLfloat) * 3 * numVertices);
      memcpy(*normals, cubeNormals, sizeof(cubeNormals));
   }

   if (NULL != texCoords) {
      *texCoords = malloc(sizeof(GLfloat) * 2 * numVertices);
      memcpy(*texCoords, cubeTex, sizeof(cubeTex)) ;
   }

   // Generate the indices.
   if (NULL != indices) {
      GLuint cubeIndices[] =
      {
         0, 2, 1,
         0, 3, 2,
         4, 5, 6,
         4, 6, 7,
         8, 9, 10,
         8, 10, 11,
         12, 15, 14,
         12, 14, 13,
         16, 17, 18,
         16, 18, 19,
         20, 23, 22,
         20, 22, 21
      };

      *indices = malloc(sizeof(GLuint) * numIndices);
      memcpy(*indices, cubeIndices, sizeof(cubeIndices));
   }

   return numIndices;
}