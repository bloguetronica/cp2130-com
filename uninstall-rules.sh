#!/bin/sh

echo Removing rules...
rm -f /etc/udev/rules.d/70-slab-generic.rules
service udev restart
echo Done!
