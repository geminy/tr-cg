HEADERS += \
    $$PWD/global/Global.h

HEADERS += \
    $$PWD/io/AbstractFileEngine.h \
    $$PWD/io/File.h \
    $$PWD/io/FileDevice.h \
    $$PWD/io/FileInfo.h \
    $$PWD/io/FSFileEngine.h \
    $$PWD/io/IODevice.h \
    $$PWD/io/IODevicePrivate.h \
    $$PWD/io/Log.h \
    $$PWD/io/Process.h

SOURCES += \
    $$PWD/io/AbstractFileEngine.cpp \
    $$PWD/io/File.cpp \
    $$PWD/io/FileDevice.cpp \
    $$PWD/io/FileInfo.cpp \
    $$PWD/io/FSFileEngine.cpp \
    $$PWD/io/IODevice.cpp \
    $$PWD/io/Log.cpp \
    $$PWD/io/Process.cpp

HEADERS += \
    $$PWD/kernel/AbstractEventDispatcher.h \
    $$PWD/kernel/Application.h \
    $$PWD/kernel/ApplicationPrivate.h \
    $$PWD/kernel/CoreEvent.h \
    $$PWD/kernel/EventDispatcher_Linux.h \
    $$PWD/kernel/EventLoop.h \
    $$PWD/kernel/Object.h \
    $$PWD/kernel/ObjectPrivate.h \
    $$PWD/kernel/Singleton.h

SOURCES += \
    $$PWD/kernel/Application.cpp \
    $$PWD/kernel/EventDispatcher_Linux.cpp \
    $$PWD/kernel/Object.cpp

HEADERS += \
    $$PWD/thread/Mutex.h \
    $$PWD/thread/Thread.h \
    $$PWD/thread/ThreadPool.h \
    $$PWD/thread/ThreadPrivate.h

SOURCES += \
    $$PWD/thread/Mutex.cpp \
    $$PWD/thread/Thread.cpp

HEADERS += \
    $$PWD/tool/CommandLineOption.h \
    $$PWD/tool/CommandLineParser.h \
    $$PWD/tool/DateTime.h \
    $$PWD/tool/Locale.h \
    $$PWD/tool/RingBuffer.h

SOURCES += \
    $$PWD/tool/CommandLineOption.cpp \
    $$PWD/tool/CommandLineParser.cpp \
    $$PWD/tool/DateTime.cpp \
    $$PWD/tool/Locale.cpp \
    $$PWD/tool/RingBuffer.cpp
