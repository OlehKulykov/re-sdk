QT -= core gui

TARGET = REGui
TEMPLATE = lib
#TEMPLATE = vclib

# static library
#CONFIG += staticlib

DEFINES += \

INCLUDEPATH += \
    ../../include \
    ../../src/addlibs/zlib \
    ../../src/addlibs/freetype/include \
    ../../src/addlibs/pyro_particles/SDK/Include
#    ../../src/gui/particle


HEADERS += \
    ../../include/recore.h \
    ../../include/reconfig/REConfigMacosx.h \
    ../../include/reconfig/REConfigWindows.h


SOURCES += \
    ../../src/gui/REAnimation.cpp \
    ../../src/gui/RERenderDeviceTextureObject.cpp \
    ../../src/gui/REAnimationBase.cpp \
    ../../src/gui/REAnimationController.cpp \
    ../../src/gui/REStackedViewController.cpp \
    ../../src/gui/REAnimationInfo.cpp \
    ../../src/gui/RESubViewsContainer.cpp \
    ../../src/gui/REButton.cpp \
    ../../src/gui/RETTFFontChar.cpp \
    ../../src/gui/RECamera.cpp \
    ../../src/gui/RETTFFontLoader.cpp \
    ../../src/gui/RECursor.cpp \
    ../../src/gui/RETextField.cpp \
    ../../src/gui/REEditableAnimationInfo.cpp \
    ../../src/gui/RETextInputRespondersManager.cpp \
    ../../src/gui/REFontBase.cpp \
    ../../src/gui/RETextureDirectX9.cpp \
    ../../src/gui/REFontObject.cpp \
    ../../src/gui/RETextureObject.cpp \
    ../../src/gui/REFontsCache.cpp \
    ../../src/gui/RETextureOpenGL.cpp \
    ../../src/gui/REFramedTextureObject.cpp \
    ../../src/gui/RETransformedView.cpp \
    ../../src/gui/REGUIApplication.cpp \
    ../../src/gui/REGUIObject.cpp \
    ../../src/gui/RETypedAnimation.cpp \
    ../../src/gui/RELabel.cpp \
    ../../src/gui/REView.cpp \
    ../../src/gui/REParticleView.cpp \
    ../../src/gui/REViewController.cpp \
    ../../src/gui/RERenderDevice.cpp \
    ../../src/gui/REXMLSerializableReader.cpp \
    ../../src/gui/RERenderDeviceDirectX9.cpp \
    ../../src/gui/RERenderDeviceOpenGL.cpp

# addlibs
SOURCES += \
    ../../src/addlibs/freetype_wrap1.c \
    ../../src/addlibs/freetype_wrap2.c \
    ../../src/addlibs/freetype_wrap3.c \
    ../../src/addlibs/freetype_wrap4.c \
    ../../src/addlibs/tinyxml_wrap1.cpp \
    ../../src/addlibs/pyroparticles_wrap1.cpp
#    ../../src/gui/particle/Base.cpp \
#    ../../src/gui/particle/BaseTypes.cpp \
#    ../../src/gui/particle/BitmapIO.cpp \
#    ../../src/gui/particle/Debug.cpp \
#    ../../src/gui/particle/FileManager.cpp \
#    ../../src/gui/particle/Geometry.cpp \
#    ../../src/gui/particle/Log.cpp \
#    ../../src/gui/particle/Particles.cpp \
#    ../../src/gui/particle/PyroOpenGL.cpp \
#    ../../src/gui/particle/Random.cpp \
#    ../../src/gui/particle/StringClass.cpp \
#    ../../src/gui/particle/Timer.cpp

HEADERS += \


win32 {
    DESTDIR = ../qt/win32

    DEFINES += __RE_OS_WINDOWS__
}


macx {

exists(/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.6.sdk/) {
    QMAKE_MAC_SDK += /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.6.sdk
}
    DESTDIR = ../qt/macx

    DEFINES += __RE_OS_MACOSX__

    CONFIG += x86
    CONFIG += x86_64

!staticlib {
    LIBS += -lz
    LIBS += -lsqlite3

    LIBS += -framework CoreFoundation
    LIBS += -framework Carbon
    LIBS += -framework Cocoa
    LIBS += -framework OpenGL

    LIBS += ../qt/macx/libRECore.dylib
    LIBS += -L../../src/addlibs/pyro_particles/SDK/Lib/MacOSX -lpyro
}

}
