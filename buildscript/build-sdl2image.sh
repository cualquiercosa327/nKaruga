export VERSION=2.0.1
export DIRECTORY_TOINSTALL="$PWD/libs"
NBPROC=$(getconf _NPROCESSORS_ONLN)

mkdir ./libs
if [ -d "./SDL2-$VERSION.tar.gz" ]; then
wget https://www.libsdl.org/projects/SDL_image/release/SDL2_image-$VERSION.tar.gz
fi
tar -xf SDL2_image-$VERSION.tar.gz
cd ./SDL2_image-$VERSION
./configure --prefix=$DIRECTORY_TOINSTALL --enable-static --disable-shared --disable-png-shared --disable-webp --disable-webp-shared --disable-tif --disable-jpeg --disable-tif-shared --disable-jpeg-shared
make clean
make -j$NBPROC
make install
