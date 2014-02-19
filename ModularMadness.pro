TEMPLATE = app
CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    engine.cpp \
    commandparser.cpp \
    module.cpp \
    modules/echomodule.cpp \
    modules/reversemodule.cpp \
    modules/noopmodule.cpp \
    modules/delaymodule.cpp \
    modules/inputmodule.cpp \
    modules/outputmodule.cpp

HEADERS += \
    commandparser.h \
    engine.h \
    module.h \
    modules/echomodule.h \
    modules/reversemodule.h \
    modules/noopmodule.h \
    modules/delaymodule.h \
    modules/inputmodule.h \
    modules/outputmodule.h

OTHER_FILES += README.md
