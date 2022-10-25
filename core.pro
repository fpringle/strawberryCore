# Copyright (c) 2022, Frederick Pringle
# All rights reserved.
# 
# This source code is licensed under the BSD-style license found in the
# LICENSE file in the root directory of this source tree.

TEMPLATE = app
TARGET = build/strawberryCLI
OBJECTS_DIR = obj
MOC_DIR = obj

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += include

HEADERS += include/action.h \
           include/board.h \
           include/eval.h \
           include/hash.h \
           include/init.h \
           include/move.h \
           include/parse.h \
           include/play.h \
           include/search.h \
           include/twiddle.h \
           include/typedefs.h
SOURCES += src/action.cpp \
           src/board.cpp \
           src/check.cpp \
           src/eval.cpp \
           src/hash.cpp \
           src/init.cpp \
           src/main.cpp \
           src/move.cpp \
           src/play.cpp \
           src/search.cpp

QT -= core gui
