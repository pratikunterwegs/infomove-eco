# files

# use cpp17
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

# warning levels
QMAKE_CXXFLAGS += -Wall

# allow debug and release
CONFIG += debug_and_release

# In release mode, turn on profiling
CONFIG(release, debug|release) {

  DEFINES += NDEBUG

  # gprof
  QMAKE_CXXFLAGS += -pg
  QMAKE_LFLAGS += -pg
}

SOURCES += \
    source.cpp

HEADERS += \
        params.hpp \
        agents.hpp \
        landscape_dynamics.hpp \
        utilities.hpp \
    	landscape.hpp \
    	simulations.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
