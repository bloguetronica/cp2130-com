This directory contains the QT project required for compiling CP2130
Commander. A list of relevant files follows:
– aboutdialog.cpp;
– aboutdialog.h;
– aboutdialog.ui;
– common.cpp;
– common.h;
– cp2130-com.pro;
– cp2130.cpp;
– cp2130.h;
– data.cpp;
– data.h;
– delaysdialog.cpp;
– delaysdialog.h;
– delaysdialog.ui;
– devicewindow.cpp;
– devicewindow.h;
– devicewindow.ui;
– dividerdialog.cpp;
– dividerdialog.h;
– dividerdialog.ui;
– icons/active64.png;
– icons/buttons/clipboard-copy.png;
– icons/buttons/clipboard-copy.svg;
– icons/buttons/clipboard-paste.png;
– icons/buttons/clipboard-paste.svg;
– icons/cp2130-com.png;
– icons/greyed64.png;
– icons/icon.svg;
– icons/selected64.png;
– images/banner.png;
– images/banner.svg;
– informationdialog.cpp;
– informationdialog.h;
– informationdialog.ui;
– libusb-extra.c;
– libusb-extra.h;
– main.cpp;
– mainwindow.cpp;
– mainwindow.h;
– mainwindow.ui;
– misc/cp2130-com.desktop;
– pinfunctionsdialog.cpp;
– pinfunctionsdialog.h;
– pinfunctionsdialog.ui;
– resources.qrc;
– translations/cp2130-com_en.qm;
– translations/cp2130-com_en.ts;
– translations/cp2130-com_en_US.qm;
– translations/cp2130-com_en_US.ts;
– translations/cp2130-com_pt.qm;
– translations/cp2130-com_pt.ts;
– translations/cp2130-com_pt_PT.qm;
– translations/cp2130-com_pt_PT.ts.

In order to compile successfully, you must have the packages
"build-essential", "libusb-1.0-0-dev" and "qt5-default" (or "qtbase5-dev")
already installed. Given that, if you wish to simply compile, change your
working directory to the current one on a terminal window, and invoke "qmake",
followed by "make" or "make all". Notice that invoking "qmake" is necessary to
generate the Makefile, but only needs to be done once.

You can also install using make. To do so, after invoking "qmake", you should
simply run "sudo make install". If you wish to force a rebuild before the
installation, then you must invoke "sudo make clean install" instead.

It may be necessary to undo any previous operations. Invoking "make clean"
will delete all object code generated during earlier compilations. However,
the previously generated binary is preserved. It is important to note that it
is possible to undo previous installation operations as well, by invoking
"sudo make uninstall". Such approach is not recommended, though.

P.S.:
Notice that any make operation containing the actions "install" or "uninstall"
(e.g. "make install" or "make uninstall") requires root permissions, or in
other words, must be run with sudo.
