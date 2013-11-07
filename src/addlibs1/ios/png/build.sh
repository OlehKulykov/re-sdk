rm -f *.a

xcodebuild -configuration Release -project png.xcodeproj -target png -arch i386 -sdk iphonesimulator OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphonesimulator/libpng.a ../lib/libpng-i386.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project png.xcodeproj -target png -arch armv7 -sdk iphoneos OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphoneos/libpng.a ../lib/libpng-armv7.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project png.xcodeproj -target png -arch armv7s -sdk iphoneos OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphoneos/libpng.a ../lib/libpng-armv7s.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project png.xcodeproj -target png -arch arm64 -sdk iphoneos OBJROOT=obj SYMROOT=sym IPHONEOS_DEPLOYMENT_TARGET=7.0.0 clean build
mv sym/Release-iphoneos/libpng.a ../lib/libpng-arm64.a
rm -rf obj
rm -rf sym

lipo -create -output ../lib/libpng.a ../lib/libpng-i386.a ../lib/libpng-armv7.a ../lib/libpng-armv7s.a ../lib/libpng-arm64.a
rm -f *.a

cp libpng/png.h ../include/png.h
cp libpng/pngconf.h ../include/pngconf.h
cp libpng/pnglibconf.h ../include/pnglibconf.h


