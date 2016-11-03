export VERSION=1.3.2
export DIRECTORY_TOINSTALL="$PWD/libs"
NBPROC=$(getconf _NPROCESSORS_ONLN)

mkdir ./libs
if [ -d "./SDL2-$VERSION.tar.gz" ]; then
wget http://downloads.xiph.org/releases/ogg/libogg-$VERSION.tar.gz
fi
tar -xf libogg-$VERSION.tar.gz
cd ./libogg-$VERSION
./configure --prefix=$DIRECTORY_TOINSTALL --enable-static --disable-shared
make clean
make -j$NBPROC
make install
