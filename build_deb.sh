mkdir ./nkaruga-debian/opt/nkaruga
mkdir ./nkaruga-debian/opt
cp -r ./sfx ./nkaruga-debian/opt/nkaruga
cp -r ./gfx ./nkaruga-debian/opt/nkaruga
cp ./nKaruga.elf ./nkaruga-debian/opt/nkaruga/nKaruga.elf
dpkg-deb --build nkaruga-debian
