QT += core gui opengl
CONFIG += c++11
TARGET = final
unix:!macx:LIBS += -lGLU
macx {
    QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
    QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
    CONFIG += c++11
    QMAKE_MACOSX_DEPLOYMENT_TARGET=10.9
}
TEMPLATE = app

# If you add your own folders, add them to INCLUDEPATH and DEPENDPATH, e.g.
INCLUDEPATH += glm glew-1.10.0/include
# DEPENDPATH += folder1 folder2

SOURCES += main.cpp \
    mainwindow.cpp \
    view.cpp \
    particlesystem/cs123_lib/resourceloader.cpp \
    glew-1.10.0/src/glew.c \
    openglshape.cpp \
    ParticleManager.cpp \
    shapes/Shape.cpp \
    shapes/ShapeCone.cpp \
    shapes/ShapeCube.cpp \
    shapes/ShapeCylinder.cpp \
    shapes/ShapeSphere.cpp \
    shapes/Statue.cpp \
    shapes/FinalProjDataStructs.cpp

HEADERS += mainwindow.h \
    view.h \
    particlesystem/cs123_lib/resourceloader.h \
    glew-1.10.0/include/GL/glew.h \
    openglshape.h \
    ParticleManager.h \
    shapes/Shape.h \
    shapes/ShapeCone.h \
    shapes/ShapeCube.h \
    shapes/ShapeCylinder.h \
    shapes/ShapeSphere.h \
    shapes/Statue.h \
    shapes/CS123SceneData.h

FORMS += mainwindow.ui

RESOURCES += \
    particlesystem/images/images.qrc \
    particlesystem/shaders/shaders.qrc

QMAKE_CXXFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON += -Waddress -Warray-bounds -Wc++0x-compat -Wchar-subscripts -Wformat\
                          -Wmain -Wparentheses -Wreorder -Wreturn-type \ #no -Wmissing-braces
                          -Wsequence-point -Wsign-compare -Wstrict-overflow=1 -Wswitch \
                          -Wtrigraphs -Wuninitialized -Wunused-label -Wunused-variable \
                          -Wvolatile-register-var -Wno-extra

macx {
    QMAKE_CXXFLAGS_WARN_ON -= -Warray-bounds -Wc++0x-compat
}

DISTFILES += \
    shaders/shader.frag \
    shaders/shader.vert
OTHER_FILES += \
    particlesystem/shaders/texture.frag \
    particlesystem/shaders/texture.vert \
    particlesystem/shaders/textureVanilla.vert \
    particlesystem/shaders/texture1.frag
