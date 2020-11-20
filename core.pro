######################################################################
# Automatically generated by qmake (3.1) Fri Nov 20 23:04:33 2020
######################################################################

TEMPLATE = app
TARGET = build/strawberryCLI
OBJECTS_DIR = obj
MOC_DIR = obj

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += action.h \
           board.h \
           eval.h \
           hash.h \
           init.h \
           move.h \
           play.h \
           search.h \
           twiddle.h \
           typedefs.h
SOURCES += action.cpp \
           board.cpp \
           check.cpp \
           eval.cpp \
           hash.cpp \
           init.cpp \
           main.cpp \
           move.cpp \
           play.cpp \
           search.cpp \
           twiddle.cpp

QT -= core gui
