#-------------------------------------------------
#
# Project created by QtCreator 2014-01-20T11:00:02
#
#-------------------------------------------------

QT       -= core gui
VERSION = 1.1
TARGET = nas2d
TEMPLATE = lib

CONFIG += staticlib

DEFINES += TIXML_USE_STL \
           GL_GLEXT_PROTOTYPES
DESTDIR = $$OUT_PWD/lib/nas2d/bin/
INCLUDEPATH += "../../include" \
               "/usr/include/SDL2" \
               "/usr/include" \
               "../../include/tinyxml"

LIBS *= -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lphysfs -lGLU -lGL


SOURCES += \
    ../../src/XmlAttributeParser.cpp \
    ../../src/Trig.cpp \
    ../../src/Timer.cpp \
    ../../src/StateManager.cpp \
    ../../src/Random.cpp \
    ../../src/NAS2D.cpp \
    ../../src/MersenneTwister.cpp \
    ../../src/KeyTranslator.cpp \
    ../../src/Game.cpp \
    ../../src/FpsCounter.cpp \
    ../../src/Filesystem.cpp \
    ../../src/EventHandler.cpp \
    ../../src/Configuration.cpp \
    ../../src/Common.cpp \
    ../../src/Mixer/SDL_Mixer.cpp \
    ../../src/Renderer/ShaderManager.cpp \
    ../../src/Renderer/Renderer.cpp \
    ../../src/Renderer/Primitives.cpp \
    ../../src/Renderer/OGL_Renderer.cpp \
    ../../src/Resources/Sprite.cpp \
    ../../src/Resources/Sound.cpp \
    ../../src/Resources/Shader.cpp \
    ../../src/Resources/Resource.cpp \
    ../../src/Resources/Music.cpp \
    ../../src/Resources/Image.cpp \
    ../../src/Resources/Font.cpp \
    ../../src/tinyxml/tinyxmlparser.cpp \
    ../../src/tinyxml/tinyxmlerror.cpp \
    ../../src/tinyxml/tinyxml.cpp \
    ../../src/tinyxml/tinystr.cpp

HEADERS += \
    ../../include/NAS2D/XmlAttributeParser.h \
    ../../include/NAS2D/Utility.h \
    ../../include/NAS2D/Trig.h \
    ../../include/NAS2D/Timer.h \
    ../../include/NAS2D/StateManager.h \
    ../../include/NAS2D/State.h \
    ../../include/NAS2D/sigslot.h \
    ../../include/NAS2D/Signal.h \
    ../../include/NAS2D/Random.h \
    ../../include/NAS2D/NAS2D.h \
    ../../include/NAS2D/MersenneTwister.h \
    ../../include/NAS2D/KeyTranslator.h \
    ../../include/NAS2D/Game.h \
    ../../include/NAS2D/FpsCounter.h \
    ../../include/NAS2D/Filesystem.h \
    ../../include/NAS2D/File.h \
    ../../include/NAS2D/Exception.h \
    ../../include/NAS2D/EventHandler.h \
    ../../include/NAS2D/Documentation.h \
    ../../include/NAS2D/Delegate.h \
    ../../include/NAS2D/Configuration.h \
    ../../include/NAS2D/Common.h \
    ../../include/NAS2D/Event/MouseButtons.h \
    ../../include/NAS2D/Event/KeyMap.h \
    ../../include/NAS2D/Mixer/SDL_Mixer.h \
    ../../include/NAS2D/Mixer/Mixer.h \
    ../../include/NAS2D/Renderer/ShaderManager.h \
    ../../include/NAS2D/Renderer/Renderer.h \
    ../../include/NAS2D/Renderer/Primitives.h \
    ../../include/NAS2D/Renderer/OGL_Renderer.h \
    ../../include/NAS2D/Resources/Sprite.h \
    ../../include/NAS2D/Resources/Sound.h \
    ../../include/NAS2D/Resources/Shader.h \
    ../../include/NAS2D/Resources/Resource.h \
    ../../include/NAS2D/Resources/Music.h \
    ../../include/NAS2D/Resources/Image.h \
    ../../include/NAS2D/Resources/Font.h \
    ../../include/NAS2D/Resources/errorImage.h \
    ../../include/NAS2D/tinyxml/tinyxml.h \
    ../../include/NAS2D/tinyxml/tinystr.h

headers.path = $$OUT_PWD/lib/nas2d/include
headers.files = ../../include/*

INSTALLS += headers
