cp -r ./sfx ./opk/sfx
cp ./nKaruga.elf ./opk/nKaruga.elf
mksquashfs ./opk nKaruga.opk -all-root -noappend -no-exports -no-xattrs
