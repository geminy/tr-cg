#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include  <X11/Xlib.h>
#include  <X11/Xatom.h>
#include  <X11/Xutil.h>

#include "glesUtil.h"

// X11 related local variable.
static Display *X_Display = NULL;

/**
 * @brief Initialize the native X11 display and window for EGL.
 * @param glesContext Applicaion context.
 * @param title Name of title bar for window.
 * @return EGL_TRUE if succeeded, otherwise EGL_FALSE.
 */
EGLBoolean CreateWindow(GLESContext *glesContext, const char *title)
{
    Window rootWin;
    XSetWindowAttributes swa;
    XSetWindowAttributes  xattr;
    Atom wmState;
    XWMHints wmHints;
    XEvent xevent;
    Window window;

    // X11 native display initialization.
    X_Display = XOpenDisplay(NULL);
    if (NULL == X_Display) {
        return EGL_FALSE;
    }

    rootWin = DefaultRootWindow(X_Display);

    swa.event_mask  =  ExposureMask | PointerMotionMask | KeyPressMask;
    window = XCreateWindow(X_Display,
                           rootWin, 0, 0,
                           glesContext->width, glesContext->height, 0,
                           CopyFromParent, InputOutput,
                           CopyFromParent, CWEventMask,
                           &swa);

    xattr.override_redirect = EGL_FALSE;
    XChangeWindowAttributes(X_Display, window, CWOverrideRedirect, &xattr);

    wmHints.input = EGL_TRUE;
    wmHints.flags = InputHint;
    XSetWMHints(X_Display, window, &wmHints);

    // make the window visible on the screen.
    XMapWindow(X_Display, window);
    XStoreName(X_Display, window, title);

    // get identifiers for the provided atom name strings.
    wmState = XInternAtom(X_Display, "_NET_WM_STATE", EGL_FALSE);

    memset(&xevent, 0, sizeof(xevent));
    xevent.type = ClientMessage;
    xevent.xclient.window = window;
    xevent.xclient.message_type = wmState;
    xevent.xclient.format = 32;
    xevent.xclient.data.l[0] = 1;
    xevent.xclient.data.l[1] = EGL_FALSE;
    XSendEvent(X_Display,
                DefaultRootWindow(X_Display),
                EGL_FALSE,
                SubstructureNotifyMask,
                &xevent);

    glesContext->hWnd = (EGLNativeWindowType)window;
    return EGL_TRUE;
}

/**
 * @brief Create an EGL rendering context and all associated elements.
 * @param hWnd Window handle.
 * @param eglDisplay EGL display.
 * @param eglContext EGL context.
 * @param eglSurface EGL surface.
 * @param attribList Attribute list.
 * @return EGL_TRUE if succeeded, otherwise EGL_FALSE.
 */
EGLBoolean CreateEGLContext(EGLNativeWindowType hWnd,
                            EGLDisplay *eglDisplay,
                            EGLContext* eglContext,
                            EGLSurface* eglSurface,
                            EGLint attribList[])
{
   EGLint numConfig;
   EGLint majorVer;
   EGLint minorVer;
   EGLDisplay display;
   EGLContext context;
   EGLSurface surface;
   EGLConfig cfg;
   EGLint contextAttribList[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

   display = eglGetDisplay((EGLNativeDisplayType)X_Display);
   if (EGL_NO_DISPLAY == display) {
      return EGL_FALSE;
   }

   if (!eglInitialize(display, &majorVer, &minorVer)) {
      return EGL_FALSE;
   }

   if (!eglGetConfigs(display, NULL, 0, &numConfig)) {
      return EGL_FALSE;
   }

   if (!eglChooseConfig(display, attribList, &cfg, 1, &numConfig)) {
      return EGL_FALSE;
   }

   surface = eglCreateWindowSurface(display, cfg, (EGLNativeWindowType)hWnd, NULL);
   if (EGL_NO_SURFACE == surface) {
      return EGL_FALSE;
   }

   context = eglCreateContext(display, cfg, EGL_NO_CONTEXT, contextAttribList);
   if (EGL_NO_CONTEXT == context) {
      return EGL_FALSE;
   }

   if (!eglMakeCurrent(display, surface, surface, context)) {
      return EGL_FALSE;
   }

   *eglDisplay = display;
   *eglSurface = surface;
   *eglContext = context;
   return EGL_TRUE;
}

/**
 * @brief Read from X11 event loop and interrupt program if there is a keypress, or window close action.
 *
 * Pump all messages from X server. Keypresses are directed to keyfunc (if defined).
 *
 * @param glesContext Application context.
 */
GLboolean userInterrupt(GLESContext *glesContext)
{
    XEvent xevent;
    KeySym keysym;
    GLboolean userInt = GL_FALSE;
    char text;

    while (XPending(X_Display)) {
        XNextEvent(X_Display, &xevent);
        if (KeyPress == xevent.type) {
            if (XLookupString(&xevent.xkey, &text,1,&keysym, 0) ==1) {
                if (glesContext->keyFunc != NULL) {
                    glesContext->keyFunc(glesContext, text, 0, 0);
                }
            }
        }
        if (DestroyNotify == xevent.type) {
            userInt = GL_TRUE;
        }
    }
    return userInt;
}

/**********************/

void GLES_API glesInitContext(GLESContext *glesContext)
{
   if (NULL != glesContext) {
      memset(glesContext, 0, sizeof(GLESContext));
   }
}

GLboolean GLES_API glesCreateWindow(GLESContext *glesContext, const char *title, GLint width, GLint height, GLuint flags)
{
   EGLint attributeList[] =
   {
       EGL_RED_SIZE,               5,
       EGL_GREEN_SIZE,          6,
       EGL_BLUE_SIZE,             5,
       EGL_ALPHA_SIZE,           (flags & GLES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
       EGL_DEPTH_SIZE,           (flags & GLES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
       EGL_STENCIL_SIZE,        (flags & GLES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
       EGL_SAMPLE_BUFFERS, (flags & GLES_WINDOW_MULTISAMPLE) ? 1 : 0,
       EGL_NONE
   };

   if (NULL == glesContext) {
      return GL_FALSE;
   }

   glesContext->width = width;
   glesContext->height = height;

   if (!CreateWindow(glesContext, title)) {
      return GL_FALSE;
   }

   if (!CreateEGLContext (glesContext->hWnd,
                          &glesContext->eglDisplay,
                          &glesContext->eglContext,
                          &glesContext->eglSurface,
                          attributeList)) {
      return GL_FALSE;
   }

   return GL_TRUE;
}

void GLES_API glesRegisterDrawFunc(GLESContext *glesContext, void (GLES_CALLBACK *drawFunc)(GLESContext*))
{
   glesContext->drawFunc = drawFunc;
}

void GLES_API glesRegisterKeyFunc(GLESContext *glesContext, void (GLES_CALLBACK *keyFunc)(GLESContext*, unsigned char, int, int))
{
   glesContext->keyFunc = keyFunc;
}

void GLES_API glesRegisterUpdateFunc(GLESContext *glesContext, void (GLES_CALLBACK *updateFunc)(GLESContext*, float))
{
   glesContext->updateFunc = updateFunc;
}

void GLES_API glesMainLoop(GLESContext *glesContext)
{
    struct timeval tv1, tv2;
    struct timezone tz;
    float deltaTime;
    float totalTime = 0.0f;
    unsigned int frame = 0;

    gettimeofday (&tv1, &tz);

    while(userInterrupt(glesContext) == GL_FALSE) {
        gettimeofday(&tv2, &tz);
        deltaTime = (float)(tv2.tv_sec - tv1.tv_sec + (tv2.tv_usec - tv1.tv_usec) * 1e-6); // Seconds-Microseconds
        tv1 = tv2;

        if (NULL != glesContext->updateFunc) {
            glesContext->updateFunc(glesContext, deltaTime);
        }
        if (NULL != glesContext->drawFunc) {
            glesContext->drawFunc(glesContext);
        }

        eglSwapBuffers(glesContext->eglDisplay, glesContext->eglSurface);

        totalTime += deltaTime;
        frame++;
        if (totalTime >  2.0f) {
            printf("%4d frames rendered in %1.4f seconds -> FPS=%3.4f\n", frame, totalTime, frame / totalTime);
            totalTime -= 2.0f;
            frame = 0;
        }
    }
}

void GLES_API glesLogMessage(const char *formatStr, ...)
{
    va_list args;
    char buffer[BUFSIZ]; // 8192 from stdio

    va_start(args, formatStr);
    vsprintf(buffer, formatStr, args);

    printf("%s", buffer);

    va_end(args);
}

char* GLES_API glesLoadTGA(char *fileName, int *width, int *height)
{
    char *buf = NULL;
    FILE *file;
    unsigned char tgaHeader[12];
    unsigned char attribute[6];
    unsigned int imgSize;

    file = fopen(fileName, "rb");
    if (NULL == file) {
        return NULL;
    }

    if(fread(&tgaHeader, sizeof(tgaHeader), 1, file) == 0) {
        fclose(file);
        return NULL;
    }

    if(fread(attribute, sizeof(attribute), 1, file) == 0) {
        fclose(file);
        return NULL;
    }

    *width = attribute[1] * 256 + attribute[0];
    *height = attribute[3] * 256 + attribute[2];
    imgSize = attribute[4] / 8 * *width * *height;
    buf = malloc(imgSize);
    if (NULL == buf) {
        fclose(file);
        return NULL;
    }

    if(fread(buf, 1, imgSize, file) != imgSize) {
        free(buf);
        fclose(file);
        return NULL;
    }
    fclose(file);
    return buf;
}
