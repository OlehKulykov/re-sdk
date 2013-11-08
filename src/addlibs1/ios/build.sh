rm -rf lib
rm -rf include

mkdir lib
mkdir include

cd art_lgpl
./build.sh
cd ..

cd jpeg
./build.sh
cd ..

cd lzma
./build.sh
cd ..

cd png
./build.sh
cd ..

cd tinyxml2
./build.sh
cd ..

cd webp
./build.sh
cd ..

cd freetype
./build.sh
cd ..


