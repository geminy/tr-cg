TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    main.c \
    hello.c \
    double.c \
    lines.c \
    polys.c \
    cube.c \
    clip.c \
    planet.c \
    robot.c \
    unproject.c \
    smooth.c \
    light.c \
    movelight.c \
    colormat.c \
    alpha.c \
    alpha3d.c \
    aargb.c \
    aaindex.c \
    fog.c \
    fogindex.c \
    torus.c \
    stroke.c \
    texgen.c \
    stencil.c \
    quadric.c \
    bezcurve.c \
    surface.c \
    select.c

LIBS += -lglut -lGL -lGLU

HEADERS += \
    common.h
