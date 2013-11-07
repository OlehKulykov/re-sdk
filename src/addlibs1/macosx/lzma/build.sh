rm -f *.a

xcodebuild -configuration Release -project lzma.xcodeproj -target lzma -arch i386 OBJROOT=obj SYMROOT=sym clean build
mv sym/Release/liblzma.a ../lib/liblzma-i386.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project lzma.xcodeproj -target lzma -arch x86_64 OBJROOT=obj SYMROOT=sym clean build
mv sym/Release/liblzma.a ../lib/liblzma-x86_64.a
rm -rf obj
rm -rf sym

lipo -create -output ../lib/liblzma.a ../lib/liblzma-i386.a ../lib/liblzma-x86_64.a
rm -f *.a

cp lzma/C/LzmaLib.h ../include/LzmaLib.h
cp lzma/C/Types.h ../include/Types.h



