CONFIG -= qt
CONFIG -= app_bundle
CONFIG += console
CONFIG += c++11

INCLUDEPATH += $$PWD/include

DESTDIR_LIB = $$PWD/mu.out/lib
DESTDIR_BIN = $$PWD/mu.out/bin
OBJECTS_DIR = $$PWD/mu.out/intermediates

LIBS += -L$$DESTDIR_LIB
