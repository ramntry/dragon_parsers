TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    ParseLib.cpp \
    Lexer.cpp \
    FirstExample.cpp

HEADERS += \
    LexerInterface.h \
    Lexer.h \
    FirstExample.h \
    ParseLib.h

