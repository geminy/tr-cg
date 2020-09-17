TEMPLATE = subdirs

SUBDIRS += src
src.file = src/src.pro

SUBDIRS += tests
tests.file = tests/tests.pro
tests.depends += src

include (include/include.pri)

OTHER_FILES += README
