rm -f *.a

xcodebuild -configuration Release -project tinyxml2.xcodeproj -target tinyxml2 -arch i386 OBJROOT=obj SYMROOT=sym clean build
mv sym/Release/libtinyxml2.a ../lib/libtinyxml2-i386.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project tinyxml2.xcodeproj -target tinyxml2 -arch x86_64 OBJROOT=obj SYMROOT=sym clean build
mv sym/Release/libtinyxml2.a ../lib/libtinyxml2-x86_64.a
rm -rf obj
rm -rf sym

lipo -create -output ../lib/libtinyxml2.a ../lib/libtinyxml2-i386.a ../lib/libtinyxml2-x86_64.a
rm -f *.a

cp tinyxml2/tinyxml2.h ../include/tinyxml2.h

