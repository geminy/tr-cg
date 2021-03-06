#include <string.h>
#include <math.h>

#include "glesUtil.h"

#ifndef GLES_PI
# define GLES_PI (3.1415926535897932384626433832795f)
#endif

void GLES_API glesScale(GLESMatrix *result, GLfloat sx, GLfloat sy, GLfloat sz)
{
    result->m[0][0] *= sx;
    result->m[0][1] *= sx;
    result->m[0][2] *= sx;
    result->m[0][3] *= sx;

    result->m[1][0] *= sy;
    result->m[1][1] *= sy;
    result->m[1][2] *= sy;
    result->m[1][3] *= sy;

    result->m[2][0] *= sz;
    result->m[2][1] *= sz;
    result->m[2][2] *= sz;
    result->m[2][3] *= sz;
}

void GLES_API glesTranslate(GLESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz)
{
    result->m[3][0] += (result->m[0][0] * tx + result->m[1][0] * ty + result->m[2][0] * tz);
    result->m[3][1] += (result->m[0][1] * tx + result->m[1][1] * ty + result->m[2][1] * tz);
    result->m[3][2] += (result->m[0][2] * tx + result->m[1][2] * ty + result->m[2][2] * tz);
    result->m[3][3] += (result->m[0][3] * tx + result->m[1][3] * ty + result->m[2][3] * tz);
}

void GLES_API glesRotate(GLESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
   GLfloat sinAngle;
   GLfloat cosAngle;
   GLfloat mag = sqrtf(x * x + y * y + z * z);

   sinAngle = sinf(angle * GLES_PI / 180.0f);
   cosAngle = cosf(angle * GLES_PI / 180.0f);
   if (mag > 0.0f) {
      GLfloat xx, yy, zz;
      GLfloat xy, yz, zx;
      GLfloat xs, ys, zs;
      GLfloat oneMinusCos;
      GLESMatrix rotMat;

      x /= mag;
      y /= mag;
      z /= mag;

      xx = x * x;
      yy = y * y;
      zz = z * z;
      xy = x * y;
      yz = y * z;
      zx = z * x;
      xs = x * sinAngle;
      ys = y * sinAngle;
      zs = z * sinAngle;
      oneMinusCos = 1.0f - cosAngle;

      rotMat.m[0][0] = (oneMinusCos * xx) + cosAngle;
      rotMat.m[0][1] = (oneMinusCos * xy) - zs;
      rotMat.m[0][2] = (oneMinusCos * zx) + ys;
      rotMat.m[0][3] = 0.0F;

      rotMat.m[1][0] = (oneMinusCos * xy) + zs;
      rotMat.m[1][1] = (oneMinusCos * yy) + cosAngle;
      rotMat.m[1][2] = (oneMinusCos * yz) - xs;
      rotMat.m[1][3] = 0.0F;

      rotMat.m[2][0] = (oneMinusCos * zx) - ys;
      rotMat.m[2][1] = (oneMinusCos * yz) + xs;
      rotMat.m[2][2] = (oneMinusCos * zz) + cosAngle;
      rotMat.m[2][3] = 0.0F;

      rotMat.m[3][0] = 0.0F;
      rotMat.m[3][1] = 0.0F;
      rotMat.m[3][2] = 0.0F;
      rotMat.m[3][3] = 1.0F;

      glesMatrixMultiply(result, &rotMat, result);
   }
}

void GLES_API glesFrustum(GLESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float deltaX = right - left;
    float deltaY = top - bottom;
    float deltaZ = farZ - nearZ;
    GLESMatrix frust;

    if ((nearZ <= 0.0f) || (farZ <= 0.0f) ||
         (deltaX <= 0.0f) || (deltaY <= 0.0f) || (deltaZ <= 0.0f)) {
         return;
    }

    frust.m[0][0] = 2.0f * nearZ / deltaX;
    frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = 0.0f;

    frust.m[1][1] = 2.0f * nearZ / deltaY;
    frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = 0.0f;

    frust.m[2][0] = (right + left) / deltaX;
    frust.m[2][1] = (top + bottom) / deltaY;
    frust.m[2][2] = -(nearZ + farZ) / deltaZ;
    frust.m[2][3] = -1.0f;

    frust.m[3][2] = -2.0f * nearZ * farZ / deltaZ;
    frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = 0.0f;

    glesMatrixMultiply(result, &frust, result);
}


void GLES_API glesPerspective(GLESMatrix *result, float fovy, float aspect, float nearZ, float farZ)
{
   GLfloat frustumW;
   GLfloat frustumH;

   frustumH = tanf(fovy / 360.0f * GLES_PI ) * nearZ;
   frustumW = frustumH * aspect;

   glesFrustum(result, -frustumW, frustumW, -frustumH, frustumH, nearZ, farZ);
}

void GLES_API glesOrtho(GLESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ)
{
    float deltaX = right - left;
    float deltaY = top - bottom;
    float deltaZ = farZ - nearZ;
    GLESMatrix ortho;

    if ((deltaX == 0.0f) || (deltaY == 0.0f) || (deltaZ == 0.0f)) {
        return;
    }

    glesMatrixLoadIdentity(&ortho);
    ortho.m[0][0] = 2.0f / deltaX;
    ortho.m[3][0] = -(right + left) / deltaX;
    ortho.m[1][1] = 2.0f / deltaY;
    ortho.m[3][1] = -(top + bottom) / deltaY;
    ortho.m[2][2] = -2.0f / deltaZ;
    ortho.m[3][2] = -(nearZ + farZ) / deltaZ;

    glesMatrixMultiply(result, &ortho, result);
}


void GLES_API glesMatrixMultiply(GLESMatrix *result, GLESMatrix *srcA, GLESMatrix *srcB)
{
    GLESMatrix tmp;
    int i;

    for (i=0; i<4; i++) {
        tmp.m[i][0] =	(srcA->m[i][0] * srcB->m[0][0])
                + (srcA->m[i][1] * srcB->m[1][0])
                + (srcA->m[i][2] * srcB->m[2][0])
                + (srcA->m[i][3] * srcB->m[3][0]);

        tmp.m[i][1] =	(srcA->m[i][0] * srcB->m[0][1])
                + (srcA->m[i][1] * srcB->m[1][1])
                + (srcA->m[i][2] * srcB->m[2][1])
                + (srcA->m[i][3] * srcB->m[3][1]);

        tmp.m[i][2] =	(srcA->m[i][0] * srcB->m[0][2])
                + (srcA->m[i][1] * srcB->m[1][2])
                + (srcA->m[i][2] * srcB->m[2][2])
                + (srcA->m[i][3] * srcB->m[3][2]);

        tmp.m[i][3] =	(srcA->m[i][0] * srcB->m[0][3])
                + (srcA->m[i][1] * srcB->m[1][3])
                + (srcA->m[i][2] * srcB->m[2][3])
                + (srcA->m[i][3] * srcB->m[3][3]);
    }
    memcpy(result, &tmp, sizeof(GLESMatrix));
}


void GLES_API glesMatrixLoadIdentity(GLESMatrix *result)
{
    memset(result, 0x0, sizeof(GLESMatrix));
    result->m[0][0] = 1.0f;
    result->m[1][1] = 1.0f;
    result->m[2][2] = 1.0f;
    result->m[3][3] = 1.0f;
}
