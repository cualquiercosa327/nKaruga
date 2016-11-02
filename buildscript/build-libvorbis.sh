export VERSION=1.3.5
export DIRECTORY_TOINSTALL="$PWD/libs"
NBPROC=$(getconf _NPROCESSORS_ONLN)

mkdir ./libs
if [ -d "./SDL2-$VERSION.tar.gz" ]; then
wget http://downloads.xiph.org/releases/vorbis/libvorbis-$VERSION.tar.gz
fi
tar -xf libvorbis-$VERSION.tar.gz
cd ./libvorbis-$VERSION
./configure --prefix=$DIRECTORY_TOINSTALL --enable-static --disable-shared
make clean
make -j$NBPROC
make install
