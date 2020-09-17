TARGET = mutestg
TEMPLATE = app

include(../../mu.pri)
DESTDIR = $$DESTDIR_BIN

LIBS += -lmu

SOURCES += \
    guiapp_main.cpp
