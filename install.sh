#!/bin/sh

echo Obtaining required packages...
apt-get -qq update
apt-get -qq install build-essential
apt-get -qq install libusb-1.0-0-dev
apt-get -qq install qt5-default
echo Copying source code files...
mkdir -p /usr/local/src/cp2130-com/icons
mkdir -p /usr/local/src/cp2130-com/misc
mkdir -p /usr/local/src/cp2130-com/translations
cp -f src/aboutdialog.cpp /usr/local/src/cp2130-com/.
cp -f src/aboutdialog.h /usr/local/src/cp2130-com/.
cp -f src/aboutdialog.ui /usr/local/src/cp2130-com/.
cp -f src/convert.cpp /usr/local/src/cp2130-com/.
cp -f src/convert.h /usr/local/src/cp2130-com/.
cp -f src/cp2130-com.pro /usr/local/src/cp2130-com/.
cp -f src/cp2130.cpp /usr/local/src/cp2130-com/.
cp -f src/cp2130.h /usr/local/src/cp2130-com/.
cp -f src/delaysdialog.cpp /usr/local/src/cp2130-com/.
cp -f src/delaysdialog.h /usr/local/src/cp2130-com/.
cp -f src/delaysdialog.ui /usr/local/src/cp2130-com/.
cp -f src/devicewindow.cpp /usr/local/src/cp2130-com/.
cp -f src/devicewindow.h /usr/local/src/cp2130-com/.
cp -f src/devicewindow.ui /usr/local/src/cp2130-com/.
cp -f src/GPL.txt /usr/local/src/cp2130-com/.
cp -f src/icons/active64.png /usr/local/src/cp2130-com/icons/.
cp -f src/icons/cp2130-com.png /usr/local/src/cp2130-com/icons/.
cp -f src/icons/greyed64.png /usr/local/src/cp2130-com/icons/.
cp -f src/icons/icon.svg /usr/local/src/cp2130-com/icons/.
cp -f src/icons/selected64.png /usr/local/src/cp2130-com/icons/.
cp -f src/informationdialog.cpp /usr/local/src/cp2130-com/.
cp -f src/informationdialog.h /usr/local/src/cp2130-com/.
cp -f src/informationdialog.ui /usr/local/src/cp2130-com/.
cp -f src/LGPL.txt /usr/local/src/cp2130-com/.
cp -f src/libusb-extra.c /usr/local/src/cp2130-com/.
cp -f src/libusb-extra.h /usr/local/src/cp2130-com/.
cp -f src/main.cpp /usr/local/src/cp2130-com/.
cp -f src/mainwindow.cpp /usr/local/src/cp2130-com/.
cp -f src/mainwindow.h /usr/local/src/cp2130-com/.
cp -f src/mainwindow.ui /usr/local/src/cp2130-com/.
cp -f src/misc/cp2130-com.desktop /usr/local/src/cp2130-com/misc/.
cp -f src/README.txt /usr/local/src/cp2130-com/.
cp -f src/resources.qrc /usr/local/src/cp2130-com/.
cp -f src/translations/cp2130-com_en.qm /usr/local/src/cp2130-com/translations/.
cp -f src/translations/cp2130-com_en.ts /usr/local/src/cp2130-com/translations/.
cp -f src/translations/cp2130-com_en_US.qm /usr/local/src/cp2130-com/translations/.
cp -f src/translations/cp2130-com_en_US.ts /usr/local/src/cp2130-com/translations/.
cp -f src/translations/cp2130-com_pt.qm /usr/local/src/cp2130-com/translations/.
cp -f src/translations/cp2130-com_pt.ts /usr/local/src/cp2130-com/translations/.
cp -f src/translations/cp2130-com_pt_PT.qm /usr/local/src/cp2130-com/translations/.
cp -f src/translations/cp2130-com_pt_PT.ts /usr/local/src/cp2130-com/translations/.
echo Building and installing application...
cd /usr/local/src/cp2130-com
qmake
make install clean
rm -f cp2130-com
echo Done!
