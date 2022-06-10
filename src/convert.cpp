/* CP2130 Commander - Version 2.0 for Debian Linux
   Copyright (c) 2022 Samuel Lourenço

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
#include <QStringRef>
#include "convert.h"

// Converts a given vector to a string of hexadecimal numbers
QString DataToHexadecimal(const QVector<quint8> &data)
{
    QString hexadecimal;
    for (quint8 value : data) {
        hexadecimal += QString("%1").arg(value, 2, 16, QChar('0'));
    }
    return hexadecimal;
}

// Converts a string of hexadecimal numbers to a corresponding vector
QVector<quint8> HexadecimalToData(const QString &hexadecimal)
{
    int strsize = hexadecimal.size() / 2;
    QVector<quint8> data(strsize);
    for (int i = 0; i < strsize; ++i) {
        data[i] = static_cast<quint8>(QStringRef(&hexadecimal, 2 * i, 2).toUInt(nullptr, 16));
    }
    return data;
}
