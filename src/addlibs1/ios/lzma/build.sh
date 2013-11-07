rm -f *.a

xcodebuild -configuration Release -project lzma.xcodeproj -target lzma -arch i386 -sdk iphonesimulator OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphonesimulator/liblzma.a ../lib/liblzma-i386.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project lzma.xcodeproj -target lzma -arch armv7 -sdk iphoneos OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphoneos/liblzma.a ../lib/liblzma-armv7.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project lzma.xcodeproj -target lzma -arch armv7s -sdk iphoneos OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphoneos/liblzma.a ../lib/liblzma-armv7s.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project lzma.xcodeproj -target lzma -arch arm64 -sdk iphoneos OBJROOT=obj SYMROOT=sym IPHONEOS_DEPLOYMENT_TARGET=7.0.0 clean build
mv sym/Release-iphoneos/liblzma.a ../lib/liblzma-arm64.a
rm -rf obj
rm -rf sym

lipo -create -output ../lib/liblzma.a ../lib/liblzma-i386.a ../lib/liblzma-armv7.a ../lib/liblzma-armv7s.a ../lib/liblzma-arm64.a
rm -f *.a

cp lzma/C/LzmaLib.h ../include/LzmaLib.h
cp lzma/C/Types.h ../include/Types.h

