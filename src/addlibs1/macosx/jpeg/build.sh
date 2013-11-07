rm -f *.a

xcodebuild -configuration Release -project jpeg.xcodeproj -target jpeg -arch i386 OBJROOT=obj SYMROOT=sym clean build
mv sym/Release/libjpeg.a ../lib/libjpeg-i386.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project jpeg.xcodeproj -target jpeg -arch x86_64 OBJROOT=obj SYMROOT=sym clean build
mv sym/Release/libjpeg.a ../lib/libjpeg-x86_64.a
rm -rf obj
rm -rf sym

lipo -create -output ../lib/libjpeg.a ../lib/libjpeg-i386.a ../lib/libjpeg-x86_64.a
rm -f *.a

cp jpeg/jconfig.h ../include/jconfig.h
cp jpeg/jpeglib.h ../include/jpeglib.h
cp jpeg/jmorecfg.h ../include/jmorecfg.h


