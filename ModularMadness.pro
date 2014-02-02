TEMPLATE = app
CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    module.cpp \
    engine.cpp \
    echomodule.cpp \
    reversemodule.cpp \
    noopmodule.cpp \
    delaymodule.cpp \
    inputmodule.cpp \
    outputmodule.cpp

HEADERS += \
    module.h \
    engine.h \
    echomodule.h \
    reversemodule.h \
    noopmodule.h \
    delaymodule.h \
    inputmodule.h \
    outputmodule.h

