rm -f *.a

xcodebuild -configuration Release -project art_lgpl.xcodeproj -target art_lgpl -arch i386 -sdk iphonesimulator OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphonesimulator/libart_lgpl.a ../lib/libart_lgpl-i386.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project art_lgpl.xcodeproj -target art_lgpl -arch armv7 -sdk iphoneos OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphoneos/libart_lgpl.a ../lib/libart_lgpl-armv7.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project art_lgpl.xcodeproj -target art_lgpl -arch armv7s -sdk iphoneos OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphoneos/libart_lgpl.a ../lib/libart_lgpl-armv7s.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project art_lgpl.xcodeproj -target art_lgpl -arch arm64 -sdk iphoneos OBJROOT=obj SYMROOT=sym IPHONEOS_DEPLOYMENT_TARGET=7.0.0 clean build
mv sym/Release-iphoneos/libart_lgpl.a ../lib/libart_lgpl-arm64.a
rm -rf obj
rm -rf sym

lipo -create -output ../lib/libart_lgpl.a ../lib/libart_lgpl-i386.a ../lib/libart_lgpl-armv7.a ../lib/libart_lgpl-armv7s.a ../lib/libart_lgpl-arm64.a
rm -f *.a

mkdir ../include/libart_lgpl

cp libart_lgpl/*.h ../include/libart_lgpl


