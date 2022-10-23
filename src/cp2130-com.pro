QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Added to provide backwards compatibility (C++11 support)
greaterThan(QT_MAJOR_VERSION, 4) {
    CONFIG += c++11
} else {
    QMAKE_CXXFLAGS += -std=c++11
}

TARGET = cp2130-com
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutdialog.cpp \
    data.cpp \
    delaysdialog.cpp \
    devicewindow.cpp \
    cp2130.cpp \
    dividerdialog.cpp \
    global.cpp \
    informationdialog.cpp \
    libusb-extra.c \
    main.cpp \
    mainwindow.cpp \
    pinfunctionsdialog.cpp

HEADERS += \
    aboutdialog.h \
    data.h \
    delaysdialog.h \
    devicewindow.h \
    cp2130.h \
    dividerdialog.h \
    global.h \
    informationdialog.h \
    libusb-extra.h \
    mainwindow.h \
    pinfunctionsdialog.h

FORMS += \
    aboutdialog.ui \
    delaysdialog.ui \
    devicewindow.ui \
    dividerdialog.ui \
    informationdialog.ui \
    mainwindow.ui \
    pinfunctionsdialog.ui

TRANSLATIONS += \
    translations/cp2130-com_en.ts \
    translations/cp2130-com_en_US.ts \
    translations/cp2130-com_pt.ts \
    translations/cp2130-com_pt_PT.ts

LIBS += -lusb-1.0

RESOURCES += \
    resources.qrc

# Added installation option
unix {
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }
    target.path = $$PREFIX/bin
    icon.files += icons/cp2130-com.png
    icon.path = $$PREFIX/share/icons/hicolor/128x128/apps
    shortcut.files = misc/cp2130-com.desktop
    shortcut.path = $$PREFIX/share/applications
    INSTALLS += icon
    INSTALLS += shortcut
}

!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    icons/cp2130-com.png \
    misc/cp2130-com.desktop
