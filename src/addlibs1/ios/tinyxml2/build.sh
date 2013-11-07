rm -f *.a

xcodebuild -configuration Release -project tinyxml2.xcodeproj -target tinyxml2 -arch i386 -sdk iphonesimulator OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphonesimulator/libtinyxml2.a ../lib/libtinyxml2-i386.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project tinyxml2.xcodeproj -target tinyxml2 -arch armv7 -sdk iphoneos OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphoneos/libtinyxml2.a ../lib/libtinyxml2-armv7.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project tinyxml2.xcodeproj -target tinyxml2 -arch armv7s -sdk iphoneos OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphoneos/libtinyxml2.a ../lib/libtinyxml2-armv7s.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project tinyxml2.xcodeproj -target tinyxml2 -arch arm64 -sdk iphoneos OBJROOT=obj SYMROOT=sym IPHONEOS_DEPLOYMENT_TARGET=7.0.0 clean build
mv sym/Release-iphoneos/libtinyxml2.a ../lib/libtinyxml2-arm64.a
rm -rf obj
rm -rf sym

lipo -create -output ../lib/libtinyxml2.a ../lib/libtinyxml2-i386.a ../lib/libtinyxml2-armv7.a ../lib/libtinyxml2-armv7s.a ../lib/libtinyxml2-arm64.a
rm -f *.a

cp tinyxml2/tinyxml2.h ../include/tinyxml2.h

