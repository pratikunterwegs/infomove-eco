CONFIG += c++17 console

# allow debug and release
CONFIG += debug_and_release

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += -std=c++17

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    source.cpp

HEADERS += \
        params.hpp \
        agents.hpp \
        landscape_dynamics.hpp \
        utilities.hpp \
    	landscape.hpp \
    	simulations.hpp

LIBS += -L/usr/local/lib -lgsl -lgslcblas -lm


