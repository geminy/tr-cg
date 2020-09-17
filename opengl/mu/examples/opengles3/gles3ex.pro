TARGET = gles3ex
TEMPLATE = app

CONFIG -= qt
CONFIG -= app_bundle
CONFIG += console
CONFIG += c++11

INCLUDEPATH += $$PWD/External/Include
HEADERS += \
    External/Include/EGL/egl.h \
    External/Include/EGL/eglext.h \
    External/Include/EGL/eglplatform.h \
    External/Include/GLES2/gl2.h \
    External/Include/GLES2/gl2ext.h \
    External/Include/GLES2/gl2platform.h \
    External/Include/GLES3/gl3.h \
    External/Include/GLES3/gl3platform.h \
    External/Include/KHR/khrplatform.h

INCLUDEPATH += $$PWD/Common/Include
HEADERS += \
    Common/Include/esUtil.h

SOURCES += \
    Common/Source/esShader.c \
    Common/Source/esShapes.c \
    Common/Source/esTransform.c \
    Common/Source/esUtil.c \

SOURCES += \
    Common/Source/LinuxX11/esUtil_X11.c

#SOURCES += Hello_Triangle/Hello_Triangle.c
#SOURCES += Example_6_3/Example_6_3.c
#SOURCES += Example_6_6/Example_6_6.c
#SOURCES += MapBuffers/MapBuffers.c
#SOURCES += VertexArrayObjects/VertexArrayObjects.c
#SOURCES += VertexBufferObjects/VertexBufferObjects.c
#SOURCES += Instancing/Instancing.c
#SOURCES += Simple_VertexShader/Simple_VertexShader.c
#SOURCES += MipMap2D/MipMap2D.c
#SOURCES += Simple_Texture2D/Simple_Texture2D.c
#SOURCES += Simple_TextureCubemap/Simple_TextureCubemap.c
#SOURCES += TextureWrap/TextureWrap.c
#SOURCES += FragmentShader/MultiTexture/MultiTexture.c
#SOURCES += MRTs/MRTs.c
SOURCES += High/Noise3D/Noise3D.c
#SOURCES += High/ParticleSystem/ParticleSystem.c
#SOURCES += High/ParticleSystemTransformFeedback/ParticleSystemTransformFeedback.c High/ParticleSystemTransformFeedback/Noise3D.c
#SOURCES += High/Shadows/Shadows.c
#SOURCES += High/TerrainRendering/TerrainRendering.c

LIBS += -lX11 -lEGL -lGLESv2
