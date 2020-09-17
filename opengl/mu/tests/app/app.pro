TARGET = mutesta
TEMPLATE = app

include(../../mu.pri)
DESTDIR = $$DESTDIR_BIN

LIBS += -lmu

SOURCES += \
    app_main.cpp
