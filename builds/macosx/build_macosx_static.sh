#!/bin/bash
#
#   Copyright 2012 - 2013 Kulykov Oleh
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#

rm -rf *.a

BUILD_DIR="/Volumes/Data/Work/recore/builds/macosx/"
OBJ_DIR=$BUILD_DIR"obj"
SYM_DIR=$BUILD_DIR"sym"

ARCH=("i386" "x86_64")

PROJECT=(
"../../src/addlibs1/macosx/art_lgpl/art_lgpl.xcodeproj"
"../../src/addlibs1/macosx/tinyxml2/tinyxml2.xcodeproj"
"../../src/addlibs1/macosx/jpeg/jpeg.xcodeproj"
"../../src/addlibs1/macosx/lzma/lzma.xcodeproj"
"../../src/addlibs1/macosx/png/png.xcodeproj"
"../../src/addlibs1/macosx/webp/webp.xcodeproj"
"recore.xcodeproj"
)

TARGETS=(
"art_lgpl"
"tinyxml2"
"jpeg"
"lzma"
"png"
"webp"
"recore"
)

for arch_index in 0 1
do
CUR_ARCH=${ARCH[arch_index]}
LIB_PATH=$BUILD_DIR"all-"$CUR_ARCH".a"
ALL_LIBS[arch_index]=$LIB_PATH

for proj_index in 0 1 2 3 4 5 6
do
CUR_PROJ=${PROJECT[proj_index]}
CUR_TARGET=${TARGETS[proj_index]}
COMND="xcodebuild -configuration Release -project "$CUR_PROJ" -target "$CUR_TARGET" -arch "$CUR_ARCH" OBJROOT="$OBJ_DIR" SYMROOT="$SYM_DIR" clean build"
echo $COMND
$COMND
cd $OBJ_DIR
find . -type f -iregex '.*\.o$' | xargs -I @ ar -q -v $LIB_PATH @
cd $BUILD_DIR
rm -rf $OBJ_DIR
rm -rf $SYM_DIR
done
done

FRAMEWORK_DIR=$BUILD_DIR"../../lib_bin/macosx/"
LIPO_LIB=$FRAMEWORK_DIR"all.a"
lipo -create ${ALL_LIBS[0]} ${ALL_LIBS[1]} -output $LIPO_LIB


cd $FRAMEWORK_DIR

rm -rf RECoreSDK.framework
mkdir -p RECoreSDK.framework/Versions/A/Headers

mkdir -p RECoreSDK.framework/Versions/A/Headers/recore
mkdir -p RECoreSDK.framework/Versions/A/Headers/regui
mkdir -p RECoreSDK.framework/Versions/A/Headers/remedia

cp ../../include/recore.h RECoreSDK.framework/Versions/A/Headers/recore.h
cp ../../include/regui.h RECoreSDK.framework/Versions/A/Headers/regui.h
cp ../../include/remedia.h RECoreSDK.framework/Versions/A/Headers/remedia.h

cp ../../include/recore/*.h RECoreSDK.framework/Versions/A/Headers/recore/
cp ../../include/regui/*.h RECoreSDK.framework/Versions/A/Headers/regui/
cp ../../include/remedia/*.h RECoreSDK.framework/Versions/A/Headers/remedia/

mkdir -p RECoreSDK.framework/Versions/A/Resources
cp all.a RECoreSDK
cp RECoreSDK RECoreSDK.framework/Versions/A

ln -s A RECoreSDK.framework/Versions/Current
ln -s Versions/Current/Headers RECoreSDK.framework/Headers
ln -s Versions/Current/Resources RECoreSDK.framework/Resources
ln -s Versions/Current/RECoreSDK RECoreSDK.framework/RECoreSDK

rm -f RECoreSDK
rm -f all.a

cd $BUILD_DIR
rm -rf *.a

