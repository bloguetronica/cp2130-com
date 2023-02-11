#!/bin/sh

echo Applying rules...
cat > /etc/udev/rules.d/70-slab-generic.rules << EOF
SUBSYSTEM=="usb", ATTRS{idVendor}=="10c4", MODE="0666"
SUBSYSTEM=="usb_device", ATTRS{idVendor}=="10c4", MODE="0666"
EOF
services=$(service --status-all)
if echo $services | grep -q eudev; then
    service eudev restart
elif echo $services | grep -q udev; then
    service udev restart
fi
echo Done!
