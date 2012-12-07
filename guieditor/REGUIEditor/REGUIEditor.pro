#-------------------------------------------------
#
# Project created by QtCreator 2012-06-08T23:47:55
#
#-------------------------------------------------

QT += core \
    gui \
    opengl


TARGET = REGUIEditor
TEMPLATE = app

RESOURCES += \
    CustomFonts.qrc

SOURCES += \
    main.cpp\
    mainwindow.cpp \
    qstringpath.cpp \
    renderdialog.cpp \
    glrenderer.cpp \
    guiobjectstreewidget.cpp \
    guiobjectstreeitem.cpp \
    qreguiobjectshelper.cpp \
    treepropertymanager.cpp \
    reqtconverter.cpp \
    treepropertybrowser.cpp \
    qreobject.cpp \
    qreview.cpp \
    qrelabel.cpp \
    qretextureobject.cpp \
    qreguiapplication.cpp \
    qreviewcontroller.cpp \
    qrefontobject.cpp \
    qrebutton.cpp \
    qretextfield.cpp \
    qreparticleview.cpp \
    openviewcontrollerdialog.cpp \
    viewcontrollercreatecustomxmlserializableobject.cpp \
    sizedialog.cpp \
    qreproperty.cpp \
    xmloutputdialog.cpp \
    objectpropsdialog.cpp \
    qrepropertylistitem.cpp \
    qrepropertylistwidgetitem.cpp \
    qrepropertyradiobutton.cpp \
    qrepropertymimedata.cpp \
    saveviewcontrollerdialog.cpp \
    savevcinfo.cpp \
    qrepropertymanager.cpp \
    qrevccpphelper.cpp \
    aboutdialog.cpp

HEADERS += \
    mainwindow.h \
    qreview.h \
    qrebutton.h \
    qreviewcontroller.h \
    qrelabel.h \
    qreparticleview.h \
    qretextfield.h \
    qretextureobject.h \
    qrefontobject.h \
    qstringpath.h \
    renderdialog.h \
    glrenderer.h \
    guiobjectstreewidget.h \
    guiobjectstreeitem.h \
    qreguiobjectshelper.h \
    treepropertymanager.h \
    reqtconverter.h \
    qreguiapplication.h \
    treepropertybrowser.h \
    openviewcontrollerdialog.h \
    qreobject.h \
    qreview.h \
    qrelabel.h \
    qretextureobject.h \
    qreguiapplication.h \
    qreviewcontroller.h \
    qrefontobject.h \
    qrebutton.h \
    qretextfield.h \
    qreparticleview.h \
    openviewcontrollerdialog.h \
    viewcontrollercreatecustomxmlserializableobject.h \
    sizedialog.h \
    qreproperty.h \
    xmloutputdialog.h \
    objectpropsdialog.h \
    qrepropertylistitem.h \
    qrepropertylistwidgetitem.h \
    qrepropertyradiobutton.h \
    qrepropertymimedata.h \
    saveviewcontrollerdialog.h \
    savevcinfo.h \
    qrepropertymanager.h \
    qrevccpphelper.h \
    aboutdialog.h

FORMS += \
    mainwindow.ui \
    renderdialog.ui \
    openviewcontrollerdialog.ui \
    viewcontrollercreatecustomxmlserializableobject.ui \
    sizedialog.ui \
    xmloutputdialog.ui \
    objectpropsdialog.ui \
    qrepropertylistitem.ui \
    saveviewcontrollerdialog.ui \
    qrevccpphelper.ui \
    aboutdialog.ui

include(../commontk-QtPropertyBrowser-c0ff83e/src/qtpropertybrowser.pri)

DEFINES += \
    __RE_QT_GUI_EDITOR__

INCLUDEPATH += \
    ../../include \
    ../../src/addlibs/freetype/include \
    ../../src/addlibs/libtheora/include \
    ../../src/addlibs/libogg/include \
    ../../src/addlibs/libvorbis/include \
    ../../src/addlibs/zlib \
    ../../src/addlibs/pyro_particles/SDK/Include
#    ../../src/gui/particle

HEADERS += \
    ../../include/recore.h \
    ../../include/regui.h \
    ../../include/remedia.h \
    ../../include/reconfig/REConfigMacosx.h \
    ../../include/reconfig/REConfigLinux.h

macx {
SOURCES += ../../src/core/RELocale_macosx.mm
}


# RECore
SOURCES += \
    ../../src/core/RE2DAlgorithm.cpp \
    ../../src/core/REImageManager.cpp \
    ../../src/core/REResourcesStorage.cpp \
    ../../src/core/RE2DVector.cpp \
    ../../src/core/RELZMACompression.cpp \
    ../../src/core/RESQLiteDB.cpp \
    ../../src/core/RE3DVector.cpp \
    ../../src/core/RELocale.cpp \
    ../../src/core/RELog.cpp \
    ../../src/core/RESQLiteDBResultSet.cpp \
    ../../src/core/REAffineTransform.cpp \
    ../../src/core/REString.cpp \
    ../../src/core/REAngle.cpp \
    ../../src/core/REStringObject.cpp \
    ../../src/core/REApplication.cpp \
    ../../src/core/REMD5Generator.cpp \
    ../../src/core/REArray.cpp \
    ../../src/core/REMainLoopUpdatable.cpp \
    ../../src/core/REMainLoopsObjectsStoragePrivate.cpp \
    ../../src/core/RETetragon.cpp \
    ../../src/core/REAutoReleasePool.cpp \
    ../../src/core/REMath.cpp \
    ../../src/core/REThread.cpp \
    ../../src/core/REBase64.cpp \
    ../../src/core/REMatrix4.cpp \
    ../../src/core/RETime.cpp \
    ../../src/core/REBuffer.cpp \
    ../../src/core/REMutex.cpp \
    ../../src/core/RETimer.cpp \
    ../../src/core/REBufferObject.cpp \
    ../../src/core/RENULLObject.cpp \
    ../../src/core/REUserCallBacks.cpp \
    ../../src/core/RECPUFeatures.cpp \
    ../../src/core/RENotificationManager.cpp \
    ../../src/core/REXMLPropertyListReader.cpp \
    ../../src/core/REColor.cpp \
    ../../src/core/RECoreC.cpp \
    ../../src/core/RENumber.cpp \
    ../../src/core/REXMLPropertyListWriter.cpp \
    ../../src/core/REData.cpp \
    ../../src/core/RENumberObject.cpp \
    ../../src/core/REZLIBCompression.cpp \
    ../../src/core/REDate.cpp \
    ../../src/core/REObject.cpp \
    ../../src/core/REZipReader.cpp \
    ../../src/core/REFile.cpp \
    ../../src/core/REObjectsArray.cpp \
    ../../src/core/unzip/ioapi.c \
    ../../src/core/unzip/unzip.c \
    ../../src/core/REFileManager.cpp \
    ../../src/core/REObjectsDictionary.cpp \
    ../../src/core/yuv2rgb/REYUV420toRGB565.cpp \
    ../../src/core/yuv2rgb/REYUV422toRGB565.cpp \
    ../../src/core/yuv2rgb/REYUV444toRGB565.cpp \
    ../../src/core/yuv2rgb/REYUVtoRGB.cpp \
    ../../src/core/yuv2rgb/REYUV420toRGB888.cpp \
    ../../src/core/yuv2rgb/REYUV422toRGB888.cpp \
    ../../src/core/yuv2rgb/REYUV444toRGB888.cpp \
    ../../src/core/yuv2rgb/REYUV420toRGB8888.cpp \
    ../../src/core/yuv2rgb/REYUV422toRGB8888.cpp \
    ../../src/core/yuv2rgb/REYUV444toRGB8888.cpp \
    ../../src/core/REImage.cpp \
    ../../src/core/RERandomizer.cpp \
    ../../src/core/REImageBase.cpp \
    ../../src/core/RERect.cpp


# REGUI
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
    ../../src/addlibs/lzma_wrap1.c \
    ../../src/addlibs/ogg_wrap1.c \
    ../../src/addlibs/openjpeg_wrap1.c \
    ../../src/addlibs/openjpeg_wrap2.c \
    ../../src/addlibs/openjpeg_wrap3.c \
    ../../src/addlibs/png_wrap1.c \
    ../../src/addlibs/sqlite_wrap1.c \
    ../../src/addlibs/tinyxml_wrap1.cpp \
    ../../src/addlibs/webp_wrap1.c \
    ../../src/addlibs/webp_wrap2.c \
    ../../src/addlibs/webp_wrap3.c \
    ../../src/addlibs/webp_wrap4.c \
    ../../src/addlibs/zlib_wrap1.c \
    ../../src/addlibs/zlib_wrap2.c \
    ../../src/addlibs/zlib_wrap3.c \
    ../../src/addlibs/zlib_wrap4.c \
    ../../src/addlibs/zlib_wrap5.c \
    ../../src/addlibs/zlib_wrap6.c \
    ../../src/addlibs/zlib_wrap7.c \
    ../../src/addlibs/zlib_wrap8.c \
    ../../src/addlibs/zlib_wrap9.c \
    ../../src/addlibs/zlib_wrap10.c \
    ../../src/addlibs/zlib_wrap11.c \
    ../../src/addlibs/zlib_wrap12.c \
    ../../src/addlibs/zlib_wrap13.c \
    ../../src/addlibs/zlib_wrap14.c \
    ../../src/addlibs/zlib_wrap15.c \
    ../../src/addlibs/jpeg_wrap1.c \
    ../../src/addlibs/jpeg_wrap10.c \
    ../../src/addlibs/jpeg_wrap11.c \
    ../../src/addlibs/jpeg_wrap12.c \
    ../../src/addlibs/jpeg_wrap13.c \
    ../../src/addlibs/jpeg_wrap14.c \
    ../../src/addlibs/jpeg_wrap15.c \
    ../../src/addlibs/jpeg_wrap16.c \
    ../../src/addlibs/jpeg_wrap17.c \
    ../../src/addlibs/jpeg_wrap18.c \
    ../../src/addlibs/jpeg_wrap19.c \
    ../../src/addlibs/jpeg_wrap2.c \
    ../../src/addlibs/jpeg_wrap20.c \
    ../../src/addlibs/jpeg_wrap21.c \
    ../../src/addlibs/jpeg_wrap22.c \
    ../../src/addlibs/jpeg_wrap23.c \
    ../../src/addlibs/jpeg_wrap24.c \
    ../../src/addlibs/jpeg_wrap25.c \
    ../../src/addlibs/jpeg_wrap26.c \
    ../../src/addlibs/jpeg_wrap27.c \
    ../../src/addlibs/jpeg_wrap28.c \
    ../../src/addlibs/jpeg_wrap29.c \
    ../../src/addlibs/jpeg_wrap3.c \
    ../../src/addlibs/jpeg_wrap30.c \
    ../../src/addlibs/jpeg_wrap31.c \
    ../../src/addlibs/jpeg_wrap32.c \
    ../../src/addlibs/jpeg_wrap33.c \
    ../../src/addlibs/jpeg_wrap34.c \
    ../../src/addlibs/jpeg_wrap35.c \
    ../../src/addlibs/jpeg_wrap36.c \
    ../../src/addlibs/jpeg_wrap37.c \
    ../../src/addlibs/jpeg_wrap38.c \
    ../../src/addlibs/jpeg_wrap39.c \
    ../../src/addlibs/jpeg_wrap4.c \
    ../../src/addlibs/jpeg_wrap40.c \
    ../../src/addlibs/jpeg_wrap41.c \
    ../../src/addlibs/jpeg_wrap42.c \
    ../../src/addlibs/jpeg_wrap43.c \
    ../../src/addlibs/jpeg_wrap44.c \
    ../../src/addlibs/jpeg_wrap45.c \
    ../../src/addlibs/jpeg_wrap46.c \
    ../../src/addlibs/jpeg_wrap47.c \
    ../../src/addlibs/jpeg_wrap48.c \
    ../../src/addlibs/jpeg_wrap49.c \
    ../../src/addlibs/jpeg_wrap5.c \
    ../../src/addlibs/jpeg_wrap50.c \
    ../../src/addlibs/jpeg_wrap51.c \
    ../../src/addlibs/jpeg_wrap52.c \
    ../../src/addlibs/jpeg_wrap53.c \
    ../../src/addlibs/jpeg_wrap54.c \
    ../../src/addlibs/jpeg_wrap55.c \
    ../../src/addlibs/jpeg_wrap56.c \
    ../../src/addlibs/jpeg_wrap57.c \
    ../../src/addlibs/jpeg_wrap58.c \
    ../../src/addlibs/jpeg_wrap59.c \
    ../../src/addlibs/jpeg_wrap6.c \
    ../../src/addlibs/jpeg_wrap60.c \
    ../../src/addlibs/jpeg_wrap7.c \
    ../../src/addlibs/jpeg_wrap8.c \
    ../../src/addlibs/jpeg_wrap9.c \
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


macx {

exists(/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.6.sdk/) {
    QMAKE_MAC_SDK += /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.6.sdk
}

    DEFINES += __RE_OS_MACOSX__

    LIBS += -lz
    LIBS += -lsqlite3

    LIBS += -framework CoreFoundation
    LIBS += -framework Carbon
    LIBS += -framework Cocoa

    LIBS += -L../../src/addlibs/pyro_particles/SDK/Lib/MacOSX -lpyro
}

win32 {

INCLUDEPATH += ../../src/addlibs/openal_soft/include

DEFINES += __RE_OS_WINDOWS__

DEFINES += WIN32_LEAN_AND_MEAN

DEFINES -= ENGINE_UNIX

DEFINES += ENGINE_WIN32
DEFINES += ENGINE_NO_REFLECTION
DEFINES += ENGINE_NO_TESS

LIBS += -lwinmm
LIBS += -L../../src/addlibs/pyro_particles/SDK/Lib/Win32 -lpyro

release {
message(Release)
CONFIG += static
static {
message(Static)
CONFIG += static
DEFINES += STATIC
}
}
}

linux-* {

message(Linux)

DEFINES += __RE_OS_LINUX__

LIBS += -L../../src/addlibs/pyro_particles/SDK/Lib/Linux/i386 -lpyro
LIBS += -lGLU

CONFIG -= x86_64
CONFIG += x86

release {
message(Release)
CONFIG += static
static {
message(Static)
CONFIG += static
DEFINES += STATIC
}
}

}

CONFIG += no_warn
CONFIG -= warn_on


