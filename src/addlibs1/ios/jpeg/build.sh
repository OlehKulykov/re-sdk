rm -f *.a

xcodebuild -configuration Release -project jpeg.xcodeproj -target jpeg -arch i386 -sdk iphonesimulator OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphonesimulator/libjpeg.a ../lib/libjpeg-i386.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project jpeg.xcodeproj -target jpeg -arch armv7 -sdk iphoneos OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphoneos/libjpeg.a ../lib/libjpeg-armv7.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project jpeg.xcodeproj -target jpeg -arch armv7s -sdk iphoneos OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphoneos/libjpeg.a ../lib/libjpeg-armv7s.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project jpeg.xcodeproj -target jpeg -arch arm64 -sdk iphoneos OBJROOT=obj SYMROOT=sym IPHONEOS_DEPLOYMENT_TARGET=7.0.0 clean build
mv sym/Release-iphoneos/libjpeg.a ../lib/libjpeg-arm64.a
rm -rf obj
rm -rf sym

lipo -create -output ../lib/libjpeg.a ../lib/libjpeg-i386.a ../lib/libjpeg-armv7.a ../lib/libjpeg-armv7s.a ../lib/libjpeg-arm64.a
rm -f *.a

cp jpeg/jconfig.h ../include/jconfig.h
cp jpeg/jpeglib.h ../include/jpeglib.h
cp jpeg/jmorecfg.h ../include/jmorecfg.h

