TARGET = mu
TEMPLATE = lib

include($$PWD/../mu.pri)
DESTDIR = $$DESTDIR_LIB

include (core/core.pri)
include (external/external.pri)
include (gui/gui.pri)
