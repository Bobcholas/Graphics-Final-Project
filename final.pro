QT += core gui opengl
CONFIG += c++11
TARGET = final
unix:!macx:LIBS += -lGLU
TEMPLATE = app

# If you add your own folders, add them to INCLUDEPATH and DEPENDPATH, e.g.
# INCLUDEPATH += folder1 folder2
# DEPENDPATH += folder1 folder2

SOURCES += main.cpp \
    mainwindow.cpp \
    view.cpp \
    particlesystem/cs123_lib/resourceloader.cpp \
    glew-1.10.0/src/glew.c \
    openglshape.cpp \
    ParticleManager.cpp

HEADERS += mainwindow.h \
    view.h \
    particlesystem/cs123_lib/resourceloader.h \
    openglshape.h \
    ParticleManager.h

FORMS += mainwindow.ui

RESOURCES += \
    particlesystem/images/images.qrc \
    particlesystem/shaders/shaders.qrc

OTHER_FILES += \
    particlesystem/shaders/texture.frag \
    particlesystem/shaders/texture.vert
