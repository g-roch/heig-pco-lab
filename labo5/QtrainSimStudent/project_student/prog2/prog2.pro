#message("Building student project")
include(../../../QtrainSim/QtrainSim.pri)

CONFIG += c++17

LIBS += -lpcosynchro

HEADERS +=  \
    src/allsections.h \
    src/parcours.h \
    src/section.h \
    src/sharedsectioninterface.h \
    src/locomotive.h \
    src/launchable.h \
    src/locomotivebehavior.h \
    src/sharedsection.h

SOURCES +=  \
    src/allsections.cpp \
    src/locomotive.cpp \
    src/cppmain.cpp \
    src/locomotivebehavior.cpp \
    src/parcours.cpp \
    src/section.cpp
