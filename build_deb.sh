cp -r ./sfx ./nkaruga-debian/opt/nkaruga
cp ./nKaruga.elf ./nkaruga-debian/opt/nkaruga/nKaruga.elf
dpkg-deb --build nkaruga-debian
