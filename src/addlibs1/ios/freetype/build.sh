rm -f *.a

xcodebuild -configuration Release -project freetype.xcodeproj -target freetype -arch i386 -sdk iphonesimulator OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphonesimulator/libfreetype.a ../lib/libfreetype-i386.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project freetype.xcodeproj -target freetype -arch armv7 -sdk iphoneos OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphoneos/libfreetype.a ../lib/libfreetype-armv7.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project freetype.xcodeproj -target freetype -arch armv7s -sdk iphoneos OBJROOT=obj SYMROOT=sym clean build
mv sym/Release-iphoneos/libfreetype.a ../lib/libfreetype-armv7s.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project freetype.xcodeproj -target freetype -arch arm64 -sdk iphoneos OBJROOT=obj SYMROOT=sym IPHONEOS_DEPLOYMENT_TARGET=7.0.0 clean build
mv sym/Release-iphoneos/libfreetype.a ../lib/libfreetype-arm64.a
rm -rf obj
rm -rf sym

lipo -create -output ../lib/libfreetype.a ../lib/libfreetype-i386.a ../lib/libfreetype-armv7.a ../lib/libfreetype-armv7s.a ../lib/libfreetype-arm64.a
rm -f *.a

mkdir ../include/freetype
cp -r freetype/include/freetype ../include/freetype
cp freetype/include/ft2build.h ../include/freetype/ft2build.h

