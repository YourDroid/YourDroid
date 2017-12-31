#-----------------------------------------------------------------
# Project converted from qmake file using q2c
# https://github.com/benapetr/q2c at сб дек. 30 21:54:09 2017
#-----------------------------------------------------------------
cmake_minimum_required (VERSION 2.6)
project(yourdroid)
option(QT5BUILD "Build using Qt5 libs" TRUE)
# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)
# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core)
find_package(Qt5 COMPONENTS Widgets Core REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Core_INCLUDES})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
#qt5_use_modules(yourdroid core gui widgets)
set(yourdroid_SOURCES "data.cpp" "log.cpp" "main.cpp" "window.cpp" "install.cpp" "cmd.cpp" "console.cpp" "bkisofs/bkAdd.c" "bkisofs/bkCache.c" "bkisofs/bkDelete.c" "bkisofs/bkError.c" "bkisofs/bkExtract.c" "bkisofs/bkGet.c" "bkisofs/bkIoWrappers.c" "bkisofs/bkLink.c" "bkisofs/bkMangle.c" "bkisofs/bkMisc.c" "bkisofs/bkPath.c" "bkisofs/bkRead.c" "bkisofs/bkRead7x.c" "bkisofs/bkSet.c" "bkisofs/bkSort.c" "bkisofs/bkTime.c" "bkisofs/bkWrite.c" "bkisofs/bkWrite7x.c")
set(yourdroid_HEADERS "data.h" "log.h" "window.h" "enum.h" "install.h" "version.h" "cmd.h" "console.h" "bkisofs/bk.h" "bkisofs/bkAdd.h" "bkisofs/bkCache.h" "bkisofs/bkDelete.h" "bkisofs/bkError.h" "bkisofs/bkExtract.h" "bkisofs/bkGet.h" "bkisofs/bkInternal.h" "bkisofs/bkIoWrappers.h" "bkisofs/bkLink.h" "bkisofs/bkMangle.h" "bkisofs/bkMisc.h" "bkisofs/bkPath.h" "bkisofs/bkRead.h" "bkisofs/bkRead7x.h" "bkisofs/bkSet.h" "bkisofs/bkSort.h" "bkisofs/bkTime.h" "bkisofs/bkWrite.h" "bkisofs/bkWrite7x.h")
add_executable(yourdroid ${yourdroid_SOURCES} ${yourdroid_HEADERS} ${Qt})
