TEMPLATE = app
DESTDIR = ../../gles/out
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../common
HEADERS += \
    ../common/glesUtil.h
SOURCES += \
    ../common/glesUtil.c \
    ../common/glesTransform.c \
    ../common/glesShape.c \
    ../common/glesShader.c

SOURCES += texturewrap.c

LIBS += -lGLESv2 -lEGL -lm -lX11

include(../gles.pri)
qtcAddDeployment()
