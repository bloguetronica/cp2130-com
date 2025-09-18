/* CP2130 Commander - Version 1.5.1 for Debian Linux
   Copyright (c) 2022-2025 Samuel Lourenço

   This program is free software: you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation, either version 3 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along
   with this program.  If not, see <https://www.gnu.org/licenses/>.


   Please feel free to contact me via e-mail: samuel.fmlourenco@gmail.com */


// Includes
#include <QApplication>
#include <QTranslator>
#include "common.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    if (!translator.load("cp2130-com_" + systemLocale.name(), ":/translations/translations")) {  // It the locale translation does not exist or cannot be loaded (modified in version 1.5.1)
        translator.load("cp2130-com_en_US", ":/translations/translations");  // Fall back to the en-US translation
    }
    a.installTranslator(&translator);
    MainWindow w;
    w.show();
    return a.exec();
}
