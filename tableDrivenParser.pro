#-------------------------------------------------
#
# Project created by QtCreator 2012-01-20T05:21:24
#
#-------------------------------------------------

QT       += core
QT       += xml

QT       -= gui

TARGET = tableManagedParser
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    Lexer.cpp \
    GrammarSymbol.cpp \
    GrammarLexer.cpp \
    CodeLexer.cpp \
    TableManagedParser.cpp

HEADERS += \
    Lexer.h \
    GrammarSymbol.h \
    GrammarLexer.h \
    CodeLexer.h \
    TableManagedParser.h

CONFIG(release, release|debug){
    DEFINES += QT_NO_DEBUG_OUTPUT
}
