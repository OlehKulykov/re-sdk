rm -f *.a

xcodebuild -configuration Release -project webp.xcodeproj -target webp -arch i386 OBJROOT=obj SYMROOT=sym clean build
mv sym/Release/libwebp.a ../lib/libwebp-i386.a
rm -rf obj
rm -rf sym

xcodebuild -configuration Release -project webp.xcodeproj -target webp -arch x86_64 OBJROOT=obj SYMROOT=sym clean build
mv sym/Release/libwebp.a ../lib/libwebp-x86_64.a
rm -rf obj
rm -rf sym

lipo -create -output ../lib/libwebp.a ../lib/libwebp-i386.a ../lib/libwebp-x86_64.a
rm -f *.a

mkdir ../include/webp

cp libwebp/src/webp/decode.h ../include/webp/decode.h
cp libwebp/src/webp/demux.h ../include/webp/demux.h
cp libwebp/src/webp/encode.h ../include/webp/encode.h
cp libwebp/src/webp/format_constants.h ../include/webp/format_constants.h
cp libwebp/src/webp/mux.h ../include/webp/mux.h
cp libwebp/src/webp/mux_types.h ../include/webp/mux_types.h
cp libwebp/src/webp/types.h ../include/webp/types.h



