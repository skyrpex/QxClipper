QT = core gui
TARGET = QxClipper
TEMPLATE = lib
DESTDIR = $$PWD/bin
DEFINES += QXCLIPPER_LIBRARY

SOURCES += \
  $$PWD/src/qxclipper.cpp \
  $$PWD/src/clipper/clipper.cpp

HEADERS += \
  $$PWD/include/QxClipper \
  $$PWD/include/qxclipper.h \
  $$PWD/include/qxclipper_global.h \
  $$PWD/src/clipper/clipper.hpp

INCLUDEPATH += $$PWD/include
