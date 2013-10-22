#-------------------------------------------------
#
# Project created by QtCreator 2013-07-05T00:30:28
#
#-------------------------------------------------

QT -= core gui

TARGET = recore
TEMPLATE = lib
#CONFIG += staticlib
CONFIG += lib

DEFINES += \

INCLUDEPATH += \
    ../../include \
    ../../src/addlibs/zlib \
    ../../src/addlibs/freetype/include

SOURCES += \
    ../../src/core/REZLIBCompression.cpp \
    ../../src/core/REZipReader.cpp \
    ../../src/core/REXMLPropertyListWriter.cpp \
    ../../src/core/REXMLPropertyListReader.cpp \
    ../../src/core/REWideString.cpp \
    ../../src/core/REUserCallBacks.cpp \
    ../../src/core/RETypedPtr.cpp \
    ../../src/core/RETypedArray.cpp \
    ../../src/core/RETimer.cpp \
    ../../src/core/RETime.cpp \
    ../../src/core/REThread.cpp \
    ../../src/core/RETetragon.cpp \
    ../../src/core/REStringUtilsPrivate.cpp \
    ../../src/core/REStringObject.cpp \
    ../../src/core/REStringBase.cpp \
    ../../src/core/REString.cpp \
    ../../src/core/RESQLiteDBResultSet.cpp \
    ../../src/core/RESQLiteDB.cpp \
    ../../src/core/REResourcesStorage.cpp \
    ../../src/core/RERect.cpp \
    ../../src/core/RERandomizer.cpp \
    ../../src/core/REObject.cpp \
    ../../src/core/RENumberObject.cpp \
    ../../src/core/RENumber.cpp \
    ../../src/core/RENULLObject.cpp \
    ../../src/core/RENULL.cpp \
    ../../src/core/RENotificationManager.cpp \
    ../../src/core/REMutex.cpp \
    ../../src/core/REMutableString.cpp \
    ../../src/core/REMD5Generator.cpp \
    ../../src/core/REMatrix4.cpp \
    ../../src/core/REMath.cpp \
    ../../src/core/REMainLoopUpdatable.cpp \
    ../../src/core/REMainLoopsObjectsStoragePrivate.cpp \
    ../../src/core/RELZMACompression.cpp \
    ../../src/core/RELog.cpp \
    ../../src/core/RELocale.cpp \
    ../../src/core/REImageManager.cpp \
    ../../src/core/REImageBase.cpp \
    ../../src/core/REImage.cpp \
    ../../src/core/REFileManager.cpp \
    ../../src/core/REFile.cpp \
    ../../src/core/REDictionaryObject.cpp \
    ../../src/core/REDictionary.cpp \
    ../../src/core/REDate.cpp \
    ../../src/core/REData.cpp \
    ../../src/core/RECPUFeatures.cpp \
    ../../src/core/RECoreC.cpp \
    ../../src/core/REColor.cpp \
    ../../src/core/REBufferObject.cpp \
    ../../src/core/REBuffer.cpp \
    ../../src/core/REBase64.cpp \
    ../../src/core/REAutoReleasePoolPrivate.cpp \
    ../../src/core/REAutoReleasePool.cpp \
    ../../src/core/REArrayObject.cpp \
    ../../src/core/REArray.cpp \
    ../../src/core/REApplication.cpp \
    ../../src/core/REAngle.cpp \
    ../../src/core/REAffineTransform.cpp \
    ../../src/core/RE3DVector.cpp \
    ../../src/core/RE2DVector.cpp \
    ../../src/core/RE2DAlgorithm.cpp \
    ../../src/core/db/REDBResultSet.cpp \
    ../../src/core/db/REDB.cpp \
    ../../src/core/json/REDictionaryJSONCallbacks.cpp \
    ../../src/core/json/OKJSONParser.c \
    ../../src/core/unzip/unzip.c \
    ../../src/core/unzip/ioapi.c \
    ../../src/core/yuv2rgb/REYUVtoRGB.cpp \
    ../../src/core/yuv2rgb/REYUV444toRGB8888.cpp \
    ../../src/core/yuv2rgb/REYUV444toRGB888.cpp \
    ../../src/core/yuv2rgb/REYUV444toRGB565.cpp \
    ../../src/core/yuv2rgb/REYUV422toRGB8888.cpp \
    ../../src/core/yuv2rgb/REYUV422toRGB888.cpp \
    ../../src/core/yuv2rgb/REYUV422toRGB565.cpp \
    ../../src/core/yuv2rgb/REYUV420toRGB8888.cpp \
    ../../src/core/yuv2rgb/REYUV420toRGB888.cpp \
    ../../src/core/yuv2rgb/REYUV420toRGB565.cpp \
    ../../src/addlibs/zlib_wrap15.c \
    ../../src/addlibs/zlib_wrap14.c \
    ../../src/addlibs/zlib_wrap13.c \
    ../../src/addlibs/zlib_wrap12.c \
    ../../src/addlibs/zlib_wrap11.c \
    ../../src/addlibs/zlib_wrap10.c \
    ../../src/addlibs/zlib_wrap9.c \
    ../../src/addlibs/zlib_wrap8.c \
    ../../src/addlibs/zlib_wrap7.c \
    ../../src/addlibs/zlib_wrap6.c \
    ../../src/addlibs/zlib_wrap5.c \
    ../../src/addlibs/zlib_wrap4.c \
    ../../src/addlibs/zlib_wrap3.c \
    ../../src/addlibs/zlib_wrap2.c \
    ../../src/addlibs/zlib_wrap1.c \
    ../../src/addlibs/tinyxml_wrap1.cpp \
    ../../src/addlibs/sqlite_wrap1.c \
    ../../src/addlibs/png_wrap1.c \
    ../../src/addlibs/openjpeg_wrap3.c \
    ../../src/addlibs/openjpeg_wrap2.c \
    ../../src/addlibs/openjpeg_wrap1.c \
    ../../src/addlibs/lzma_wrap1.c \
    ../../src/addlibs/jpeg_wrap60.c \
    ../../src/addlibs/jpeg_wrap59.c \
    ../../src/addlibs/jpeg_wrap58.c \
    ../../src/addlibs/jpeg_wrap57.c \
    ../../src/addlibs/jpeg_wrap56.c \
    ../../src/addlibs/jpeg_wrap55.c \
    ../../src/addlibs/jpeg_wrap54.c \
    ../../src/addlibs/jpeg_wrap53.c \
    ../../src/addlibs/jpeg_wrap52.c \
    ../../src/addlibs/jpeg_wrap51.c \
    ../../src/addlibs/jpeg_wrap50.c \
    ../../src/addlibs/jpeg_wrap49.c \
    ../../src/addlibs/jpeg_wrap48.c \
    ../../src/addlibs/jpeg_wrap47.c \
    ../../src/addlibs/jpeg_wrap46.c \
    ../../src/addlibs/jpeg_wrap45.c \
    ../../src/addlibs/jpeg_wrap44.c \
    ../../src/addlibs/jpeg_wrap43.c \
    ../../src/addlibs/jpeg_wrap42.c \
    ../../src/addlibs/jpeg_wrap41.c \
    ../../src/addlibs/jpeg_wrap40.c \
    ../../src/addlibs/jpeg_wrap39.c \
    ../../src/addlibs/jpeg_wrap38.c \
    ../../src/addlibs/jpeg_wrap37.c \
    ../../src/addlibs/jpeg_wrap36.c \
    ../../src/addlibs/jpeg_wrap35.c \
    ../../src/addlibs/jpeg_wrap34.c \
    ../../src/addlibs/jpeg_wrap33.c \
    ../../src/addlibs/jpeg_wrap32.c \
    ../../src/addlibs/jpeg_wrap31.c \
    ../../src/addlibs/jpeg_wrap30.c \
    ../../src/addlibs/jpeg_wrap29.c \
    ../../src/addlibs/jpeg_wrap28.c \
    ../../src/addlibs/jpeg_wrap27.c \
    ../../src/addlibs/jpeg_wrap26.c \
    ../../src/addlibs/jpeg_wrap25.c \
    ../../src/addlibs/jpeg_wrap24.c \
    ../../src/addlibs/jpeg_wrap23.c \
    ../../src/addlibs/jpeg_wrap22.c \
    ../../src/addlibs/jpeg_wrap21.c \
    ../../src/addlibs/jpeg_wrap20.c \
    ../../src/addlibs/jpeg_wrap19.c \
    ../../src/addlibs/jpeg_wrap18.c \
    ../../src/addlibs/jpeg_wrap17.c \
    ../../src/addlibs/jpeg_wrap16.c \
    ../../src/addlibs/jpeg_wrap15.c \
    ../../src/addlibs/jpeg_wrap14.c \
    ../../src/addlibs/jpeg_wrap13.c \
    ../../src/addlibs/jpeg_wrap12.c \
    ../../src/addlibs/jpeg_wrap11.c \
    ../../src/addlibs/jpeg_wrap10.c \
    ../../src/addlibs/jpeg_wrap9.c \
    ../../src/addlibs/jpeg_wrap8.c \
    ../../src/addlibs/jpeg_wrap7.c \
    ../../src/addlibs/jpeg_wrap6.c \
    ../../src/addlibs/jpeg_wrap5.c \
    ../../src/addlibs/jpeg_wrap4.c \
    ../../src/addlibs/jpeg_wrap3.c \
    ../../src/addlibs/jpeg_wrap2.c \
    ../../src/addlibs/jpeg_wrap1.c \
    ../../src/addlibs/webp_wrap4.c \
    ../../src/addlibs/webp_wrap3.c \
    ../../src/addlibs/webp_wrap2.c \
    ../../src/addlibs/webp_wrap1.c

HEADERS += \
    ../../include/recore.h \
    ../../include/recore/REZLIBCompression.h \
    ../../include/recore/REZipReader.h \
    ../../include/recore/REYUVtoRGB.h \
    ../../include/recore/REXMLPropertyListWriter.h \
    ../../include/recore/REXMLPropertyListReader.h \
    ../../include/recore/REWideString.h \
    ../../include/recore/REVector3.h \
    ../../include/recore/REVector2.h \
    ../../include/recore/REUserCallBacks.h \
    ../../include/recore/RETypes.h \
    ../../include/recore/RETypedPtr.h \
    ../../include/recore/RETypedArray.h \
    ../../include/recore/RETimer.h \
    ../../include/recore/RETime.h \
    ../../include/recore/REThread.h \
    ../../include/recore/RETetragon.h \
    ../../include/recore/REStringObject.h \
    ../../include/recore/REStringBase.h \
    ../../include/recore/REString.h \
    ../../include/recore/RESize.h \
    ../../include/recore/REResourcesStorage.h \
    ../../include/recore/RERect.h \
    ../../include/recore/RERange.h \
    ../../include/recore/RERandomizer.h \
    ../../include/recore/REQuaternion.h \
    ../../include/recore/REPtr.h \
    ../../include/recore/REProperty.h \
    ../../include/recore/REPoint3.h \
    ../../include/recore/REPoint2.h \
    ../../include/recore/REObjectProperty.h \
    ../../include/recore/REObject.h \
    ../../include/recore/RENumberObject.h \
    ../../include/recore/RENumber.h \
    ../../include/recore/RENULLObject.h \
    ../../include/recore/RENULL.h \
    ../../include/recore/RENotificationManager.h \
    ../../include/recore/REMutex.h \
    ../../include/recore/REMutableString.h \
    ../../include/recore/REMem.h \
    ../../include/recore/REMD5Generator.h \
    ../../include/recore/REMatrix4.h \
    ../../include/recore/REMath.h \
    ../../include/recore/REMainLoopUpdatable.h \
    ../../include/recore/RELZMACompression.h \
    ../../include/recore/RELog.h \
    ../../include/recore/RELocale.h \
    ../../include/recore/REList.h \
    ../../include/recore/REInterpolation.h \
    ../../include/recore/REImageManager.h \
    ../../include/recore/REImageBase.h \
    ../../include/recore/REImage.h \
    ../../include/recore/REFloat16.h \
    ../../include/recore/REFileManager.h \
    ../../include/recore/REFile.h \
    ../../include/recore/REEdgeInsets.h \
    ../../include/recore/REDictionaryObject.h \
    ../../include/recore/REDictionary.h \
    ../../include/recore/REDBResultSet.h \
    ../../include/recore/REDB.h \
    ../../include/recore/REDate.h \
    ../../include/recore/REData.h \
    ../../include/recore/RECPUFeatures.h \
    ../../include/recore/RECoreSettings.h \
    ../../include/recore/RECorePlatformSelector.h \
    ../../include/recore/RECoreOSSelector.h \
    ../../include/recore/RECoreIncludes.h \
    ../../include/recore/RECoreC.h \
    ../../include/recore/RECommonHeader.h \
    ../../include/recore/REColor.h \
    ../../include/recore/REClassMethod.h \
    ../../include/recore/REBufferObject.h \
    ../../include/recore/REBuffer.h \
    ../../include/recore/REBase64.h \
    ../../include/recore/REAutoReleasePool.h \
    ../../include/recore/REArrayObject.h \
    ../../include/recore/REArray.h \
    ../../include/recore/REApplication.h \
    ../../include/recore/REAngle.h \
    ../../include/recore/REAffineTransform.h \
    ../../include/recore/RE2DAlgorithm.h \
    ../../include/recore/IREFileReadable.h \
    ../../include/recore/IREAffineTransformed.h \
    ../../include/recore/private/REStringUtilsPrivate.h \
    ../../include/recore/private/RESQLiteDBResultSet.h \
    ../../include/recore/private/RESQLiteDB.h \
    ../../include/recore/private/REMainThreadClassMethodWaitedPrivate.h \
    ../../include/recore/private/REMainThreadClassMethodPrivate.h \
    ../../include/recore/private/REMainLoopsObjectsStoragePrivate.h \
    ../../include/recore/private/REDictionaryJSONCallbacks.h \
    ../../include/recore/private/REDetachableThreadPrivate.h \
    ../../include/recore/private/REAutoReleasePoolPrivate.h \
    ../../src/core/json/OKJSONParser.h \
    ../../src/core/unzip/unzip.h \
    ../../src/core/unzip/ioapi.h \
    ../../src/core/unzip/crypt.h \
    ../../src/addlibs/zlib.h \
    ../../src/addlibs/zlib_config.h \
    ../../src/addlibs/tinyxml.h \
    ../../src/addlibs/tinyxml_config.h \
    ../../src/addlibs/sqlite.h \
    ../../src/addlibs/sqlite_config.h \
    ../../src/addlibs/png.h \
    ../../src/addlibs/png_config.h \
    ../../src/addlibs/openjpeg.h \
    ../../src/addlibs/openjpeg_config.h \
    ../../src/addlibs/lzma.h \
    ../../src/addlibs/lzma_config.h \
    ../../src/addlibs/jpeg.h \
    ../../src/addlibs/jpeg_config.h \
    ../../src/addlibs/webp.h \
    ../../src/addlibs/webp_config.h

win32 {
    DESTDIR = ../qt/win32

    DEFINES += __RE_OS_WINDOWS__
}


macx {

exists(/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.6.sdk/) {
    QMAKE_MAC_SDK += /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.6.sdk
}

    OBJECTIVE_SOURCES += ../../src/core/RELocale_macosx.mm
    DESTDIR = ../qt/macx

    DEFINES += __RE_OS_MACOSX__

    CONFIG += x86
    CONFIG += x86_64

!staticlib {
#    LIBS += -lz
LIBS += -lsqlite3

LIBS += -framework Foundation
LIBS += -framework CoreFoundation
LIBS += -framework Carbon
LIBS += -framework Cocoa
#    LIBS += -framework OpenGL

#    LIBS += -L../../src/addlibs/pyro_particles/SDK/Lib/MacOSX -lpyro
}

}

