rm -f *.a

xcodebuild -configuration Release -project png.xcodeproj -target png -arch i386 OBJROOT=obj SYMROOT=sym clean build
mv sym/Release/libpng.a ../lib/libpng-i386.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project png.xcodeproj -target png -arch x86_64 OBJROOT=obj SYMROOT=sym clean build
mv sym/Release/libpng.a ../lib/libpng-x86_64.a
rm -rf obj
rm -rf sym

lipo -create -output ../lib/libpng.a ../lib/libpng-i386.a ../lib/libpng-x86_64.a
rm -f *.a

cp libpng/png.h ../include/png.h
cp libpng/pngconf.h ../include/pngconf.h
cp libpng/pnglibconf.h ../include/pnglibconf.h


