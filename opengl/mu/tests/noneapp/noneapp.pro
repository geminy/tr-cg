TARGET = mutest
TEMPLATE = app

include(../../mu.pri)
DESTDIR = $$DESTDIR_BIN

LIBS += -lmu

SOURCES += \
    noneapp_main.cpp
