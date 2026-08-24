# musicplay.pro is the qmake project file.
# Qt Creator reads this file to find source, header, and UI files.

# Qt modules used by this QWidget project.
QT += core gui widgets multimedia

# Output executable name.
TARGET = musicplay

# Build an application, not a library.
TEMPLATE = app

# Use the C++11 standard.
CONFIG += c++11

# C++ source files.
SOURCES += \
    main.cpp \
    widget.cpp

# C++ header files.
HEADERS += \
    widget.h

# Qt Designer UI files.
FORMS += \
    widget.ui

RESOURCES += \
    im.qrc
