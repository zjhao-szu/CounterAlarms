INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_COUNTERCLOCKWISEALARMS CounterClockwiseAlarms)

FIND_PATH(
    COUNTERCLOCKWISEALARMS_INCLUDE_DIRS
    NAMES CounterClockwiseAlarms/api.h
    HINTS $ENV{COUNTERCLOCKWISEALARMS_DIR}/include
        ${PC_COUNTERCLOCKWISEALARMS_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    COUNTERCLOCKWISEALARMS_LIBRARIES
    NAMES gnuradio-CounterClockwiseAlarms
    HINTS $ENV{COUNTERCLOCKWISEALARMS_DIR}/lib
        ${PC_COUNTERCLOCKWISEALARMS_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
          )

include("${CMAKE_CURRENT_LIST_DIR}/CounterClockwiseAlarmsTarget.cmake")

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(COUNTERCLOCKWISEALARMS DEFAULT_MSG COUNTERCLOCKWISEALARMS_LIBRARIES COUNTERCLOCKWISEALARMS_INCLUDE_DIRS)
MARK_AS_ADVANCED(COUNTERCLOCKWISEALARMS_LIBRARIES COUNTERCLOCKWISEALARMS_INCLUDE_DIRS)
