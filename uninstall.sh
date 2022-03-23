#!/bin/sh

echo Removing application...
rm -f /usr/local/share/applications/cp2130-com.desktop
rmdir --ignore-fail-on-non-empty /usr/local/share/applications
rm -f /usr/local/share/icons/hicolor/128x128/apps/cp2130-com.png
rmdir --ignore-fail-on-non-empty /usr/local/share/icons/hicolor/128x128/apps
rmdir --ignore-fail-on-non-empty /usr/local/share/icons/hicolor/128x128
rmdir --ignore-fail-on-non-empty /usr/local/share/icons/hicolor
rmdir --ignore-fail-on-non-empty /usr/local/share/icons
rm -f /usr/local/bin/cp2130-com
echo Removing source code files...
rm -rf /usr/local/src/cp2130-com
echo Done!
