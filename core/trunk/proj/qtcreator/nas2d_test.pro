#-------------------------------------------------
#
# Project created by QtCreator 2014-01-20T11:00:02
#
#-------------------------------------------------

QT       -= core gui
VERSION = 1.1
TARGET = nas2d_test
TEMPLATE = app

DEFINES += TIXML_USE_STL \
           GL_GLEXT_PROTOTYPES
DESTDIR = $$_PRO_FILE_PWD_/../build/nas2d/

headers.path = $$_PRO_FILE_PWD_/../build/nas2d/
headers.files = ../../include/*

INSTALLS += headers

HEADERS += \
    ../../../../nas2d-tests/NAS2D_Test/GameState.h \
    ../../../../nas2d-tests/NAS2D_Test/Player.h \
    ../../../../nas2d-tests/NAS2D_Test/Zombie.h

SOURCES += \
    ../../../../nas2d-tests/NAS2D_Test/GameState.cpp \
    ../../../../nas2d-tests/NAS2D_Test/NAS2D_Test.cpp \
    ../../../../nas2d-tests/NAS2D_Test/Player.cpp \
    ../../../../nas2d-tests/NAS2D_Test/Zombie.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../depend/win32/lib/ -lnas2d -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lphysfs -lGLee -lGL
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../depend/win32/lib/ -lnas2d -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lphysfs -lGLee -lGL
else:unix:!macx: LIBS += -L$$PWD/../../../../depend/win32/lib/ -lnas2

INCLUDEPATH += $$PWD/../../../../depend/win32/include/nas2d $$PWD/../../../../depend/win32/include
DEPENDPATH += $$PWD/../../../../depend/win32/include/nas2d $$PWD/../../../../depend/win32/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../depend/win32/lib/libnas2d.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../depend/win32/lib/libnas2d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../depend/win32/lib/nas2d.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../depend/win32/lib/nas2d.lib
else:unix:!macx: PRE_TARGETDEPS += $$PWD/../../../../depend/win32/lib/libnas2d.a
