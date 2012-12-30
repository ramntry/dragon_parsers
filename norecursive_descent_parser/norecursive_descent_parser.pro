TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    ParseLib.cpp \
    Lexer.cpp \
    FirstExample.cpp \
    VerySimpleExample.cpp

HEADERS += \
    LexerInterface.h \
    Lexer.h \
    FirstExample.h \
    ParseLib.h \
    VerySimpleExample.h

