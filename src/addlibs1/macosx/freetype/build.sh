rm -f *.a

xcodebuild -configuration Release -project freetype.xcodeproj -target freetype -arch i386 OBJROOT=obj SYMROOT=sym clean build
mv sym/Release/libfreetype.a ../lib/libfreetype-i386.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project freetype.xcodeproj -target freetype -arch x86_64 OBJROOT=obj SYMROOT=sym clean build
mv sym/Release/libfreetype.a ../lib/libfreetype-x86_64.a
rm -rf obj
rm -rf sym

lipo -create -output ../lib/libfreetype.a ../lib/libfreetype-i386.a ../lib/libfreetype-x86_64.a
rm -f *.a

mkdir ../include/freetype
cp -r freetype/include/freetype ../include/freetype
cp freetype/include/ft2build.h ../include/freetype/ft2build.h


