TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    Symbol.cpp \
    Nonterminal.cpp \
    Terminal.cpp \
    Grammar.cpp

HEADERS += \
    Symbol.h \
    Nonterminal.h \
    Terminal.h \
    Grammar.h

