QT += core gui widgets openglwidgets
CONFIG += c++17
TARGET = HydModeler

SOURCES += main.cpp \
           glwidget.cpp \
           mainwindow.cpp \
           model.cpp \
           tree_model.cpp

HEADERS += mainwindow.h \
           glwidget.h \
           model.h \
           tree_model.h

FORMS += mainwindow.ui

RESOURCES += \
    hydmodeler.qrc

win32: LIBS += -lopengl32
unix: LIBS += -lGL

DISTFILES += \
    shaders/fragment.glsl \
    shaders/vertex.glsl
