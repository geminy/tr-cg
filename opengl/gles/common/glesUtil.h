#ifndef GLESUTIL_H
#define GLESUTIL_H

/**
 * @file glesUtil.h
 * @brief A utility library for OpenGL ES 2.0.
 *
 * This library provides a basic common framework.
 */

#include <GLES2/gl2.h>
#include <EGL/egl.h>

#ifdef __cplusplus
# define GLES_BEGIN_DECLARATION \
    extern "C" {
# define GLES_END_DECLARATION \
    }
#else
# define GLES_BEGIN_DECLARATION
# define GLES_END_DECLARATION
#endif

GLES_BEGIN_DECLARATION

#ifdef GLES_EXPORT
# define GLES_API __attribute__((visibility("default")))
#else
# define GLES_API
#endif

#if 1
# define GLES_CALLBACK
#endif

// buffer
#define GLES_WINDOW_RGB                  (0)
#define GLES_WINDOW_ALPHA              (1)
#define GLES_WINDOW_DEPTH              (2)
#define GLES_WINDOW_STENCIL           (4)
#define GLES_WINDOW_MULTISAMPLE (8)

typedef struct
{
    GLfloat m[4][4]; // matrix
} GLESMatrix, *GLESMatrixp;

typedef struct _glescontext
{
   void                                  *userData; // user data
   GLint                                width; // window width
   GLint                                height; // window height
   EGLNativeWindowType hWnd; // window handle
   EGLDisplay                      eglDisplay; // egl display
   EGLContext                     eglContext; // egl context
   EGLSurface                      eglSurface; // egl surface

   void (GLES_CALLBACK *drawFunc)(struct _glescontext*); // draw callback
   void (GLES_CALLBACK *keyFunc)(struct _glescontext*, unsigned char key, int x, int y); // keyboard event callback
   void (GLES_CALLBACK *updateFunc)(struct _glescontext*, float deltaTime); // update callback
} GLESContext, *GLESContextp;

/**util********************/

/**
 * @brief Initialize GLES context.
 * @param glesContext Application context.
 * @note This must be called firstly before calling any other functions.
 */
void GLES_API glesInitContext(GLESContext *glesContext);

/**
 * @brief Create a window.
 * @param glesContext Application context.
 * @param title Name for title bar of window.
 * @param width Width of window in pixels.
 * @param height Height of window in pixels.
 * @param flags Bit field with GLES_WINDOW_XXX for buffer.
 * @return GL_TRUE if window created successfully, otherwise GL_FALSE.
 */
GLboolean GLES_API glesCreateWindow(GLESContext *glesContext, const char *title, GLint width, GLint height, GLuint flags);

/**
 * @brief Register a draw callback functioin.
 * @param glesContext Application context.
 * @param drawFunc Draw callback function for rendering.
 */
void GLES_API glesRegisterDrawFunc(GLESContext *glesContext, void (GLES_CALLBACK *drawFunc)(GLESContext*));

/**
 * @brief Register a keyboard event callback functioin.
 * @param glesContext Application context.
 * @param keyFunc Key callback function for keyboard event processing.
 */
void GLES_API glesRegisterKeyFunc(GLESContext *glesContext, void (GLES_CALLBACK *keyFunc)(GLESContext*, unsigned char, int, int));

/**
 * @brief Register an update callback functioin.
 * @param glesContext Application context.
 * @param updateFunc Update callback function for rendering.
 */
void GLES_API glesRegisterUpdateFunc(GLESContext *glesContext, void (GLES_CALLBACK *updateFunc)(GLESContext*, float));

/**
 * @brief Start the main loop for GLES application.
 * @param glesContext Application context.
 */
void GLES_API glesMainLoop(GLESContext *glesContext);

/**
 * @brief Output error message.
 * @param formatStr Format string for error log.
 */
void GLES_API glesLogMessage(const char *formatStr, ...);

/**
 * @brief Load a TGA image of 24-bit from a file.
 * @param fileName File name of image source.
 * @param width Width of image in pixels.
 * @param height Height of image in pixels.
 * @return Pointer to loaded image, NULL if failed.
 * @note Does not support loading of compressed TGAs nor TGAa with alpha channel.
 */
char* GLES_API glesLoadTGA(char *fileName, int *width, int *height);

/**shader********************/

/**
 * @brief Load a shader, and process compiling errors.
 * @param type Type of shader,  GL_VERTEX_SHADER or GL_FRAGMENT_SHADER.
 * @param shaderSrc Source string of shader.
 * @return A new shader object if succeed, otherwise 0.
 */
GLuint GLES_API glesLoadShader(GLenum type, const char *shaderSrc);

/**
 * @brief Load a vertex and fragment shader, create a program object and link program, and process compiling errors.
 * @param vertShaderSrc Source codes of vertex shader.
 * @param fragShaderSrc Source codes of fragment shader.
 * @return A new program object linked with the vertex and fragment shader, or 0 if failed.
 */
GLuint GLES_API glesLoadProgram(const char *vertShaderSrc, const char *fragShaderSrc);

/**shape********************/

/**
 * @brief Generate a sphere and index list for a GL_TRIANGLE_STRIP.
 * @param numSlices The number of sphere's slices.
 * @param radius The radius of sphere.
 * @param vertices 3fv positions.
 * @param normals 3fv normals.
 * @param texCoords 2fv texture coordinates.
 * @param indices v for triangle strip.
 * @return The number of indices required to render the buffers as a GL_TRIANGLE_STRIP.
 */
int GLES_API glesGenSphere(int numSlices, float radius, GLfloat **vertices, GLfloat **normals, GLfloat **texCoords, GLuint **indices);

/**
 * @brief Generate a cube and index list for a GL_TRIANGLE_STRIP.
 * @param scale The size of cube,.
 * @param vertices 3fv positions.
 * @param normals 3fv normals.
 * @param texCoords 2fv texture coordinates.
 * @param indices v for triangle strip.
 * @return The number of indices required to render the buffers as a GL_TRIANGLE_STRIP.
 */
int GLES_API glesGenCube (float scale, GLfloat **vertices, GLfloat **normals, GLfloat **texCoords, GLuint **indices);

/**transform********************/

/**
 * @brief Multiply matrix by result with a scaling matrix.
 * @param result Input matrix and scaled matrix is stored in result.
 * @param sx Scale factor along the x axes.
 * @param sy Scale factor along the y axes.
 * @param sz Sacle factor along the z axes.
 */
void GLES_API glesScale(GLESMatrix *result, GLfloat sx, GLfloat sy, GLfloat sz);

/**
 * @brief Multiply matrix by result with a translation matrix.
 * @param result Input matrix and translated matrix is stored in result.
 * @param tx Scale factor along the x axes.
 * @param ty Scale factor along the y axes.
 * @param tz Sacle factor along the z axes.
 */
void GLES_API glesTranslate(GLESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz);

/**
 * @brief Multiply matrix by result with a rotation matrix.
 * @param result Input matrix and rotated matrix is stored in result.
 * @param angle Rotation angle by degree.
 * @param tx Scale factor along the x axes.
 * @param ty Scale factor along the y axes.
 * @param tz Sacle factor along the z axes.
 */
void GLES_API glesRotate(GLESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

/**
 * @brief Multiply matrix by result with a perspective matrix.
 * @param result Input matrix and new matrix is stored in result.
 * @param left Coordinate for the left vertical clipping plane.
 * @param right Coordinate for the right vertical clipping plane.
 * @param bottom Coordinate for the bottom horizontal clipping plane.
 * @param top Coordinate for the top horizontal clipping plane.
 * @param nearZ Distance to the near depth clipping plane.
 * @param farZ Distance to the far depth clipping plane.
 * @note nearZ and farZ must be positive.
 */
void GLES_API glesFrustum(GLESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ);

/**
 * @brief Multiply matrix by result with a perspective matrix.
 * @param result Input matrix and new matrix is stored in result.
 * @param fovy Field of view y angle in degrees.
 * @param aspect Aspect ratio of screen.
 * @param nearZ Near plane distance.
 * @param farZ Far plane distance.
 */
void GLES_API glesPerspective(GLESMatrix *result, float fovy, float aspect, float nearZ, float farZ);

/**
 * @brief Multiply matrix by result with a perspective matrix.
 * @param result Input matrix and new matrix is stored in result.
 * @param left Coordinate for the left vertical clipping plane.
 * @param right Coordinate for the right vertical clipping plane.
 * @param bottom Coordinate for the bottom horizontal clipping plane.
 * @param top Coordinate for the top horizontal clipping plane.
 * @param nearZ Distance to the near depth clipping plane.
 * @param farZ Distance to the far depth clipping plane.
 * @note nearZ and farZ are negative if plane is behind the viewer.
 */
void GLES_API glesOrtho(GLESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ);

/**
 * @brief result = srcA * srcB
 * @param result Store the multiplied matrix.
 * @param srcA Input matrix to be multiplied.
 * @param srcB Input matrix to be multiplied.
 */
void GLES_API glesMatrixMultiply(GLESMatrix *result, GLESMatrix *srcA, GLESMatrix *srcB);

/**
 * @brief Get the identity matrix.
 * @param result Store the identity matrix.
 */
void GLES_API glesMatrixLoadIdentity(GLESMatrix *result);

GLES_END_DECLARATION

#endif // GLESUTIL_H
