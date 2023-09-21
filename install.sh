#!/bin/sh

# copy executable
cp froot $QPEDIR/bin
# everything in pics/ needs to go to $QPEDIR/pics/froot
cp -r pics $QPEDIR
# copy icon
cp Froot.png $QPEDIR/pics
# copy applnk
cp froot.desktop $QPEDIR/apps/Games
# copy help file
cp -r help $QPEDIR
