rm -f *.a

xcodebuild -configuration Release -project art_lgpl.xcodeproj -target art_lgpl -arch i386 OBJROOT=obj SYMROOT=sym clean build
mv sym/Release/libart_lgpl.a ../lib/libart_lgpl-i386.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project art_lgpl.xcodeproj -target art_lgpl -arch x86_64 OBJROOT=obj SYMROOT=sym clean build
mv sym/Release/libart_lgpl.a ../lib/libart_lgpl-x86_64.a
rm -rf obj
rm -rf sym

lipo -create -output ../lib/libart_lgpl.a ../lib/libart_lgpl-i386.a ../lib/libart_lgpl-x86_64.a
rm -f *.a

mkdir ../include/libart_lgpl

cp libart_lgpl/*.h ../include/libart_lgpl


