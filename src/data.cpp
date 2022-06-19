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
#include "data.h"

// Converts to a string of hexadecimal numbers
QString Data::toHexadecimal() const
{
    QString hexadecimal;
    for (int i = 0; i < vector.size(); ++i) {
        if (i > 0) {
            hexadecimal += " ";
        }
        hexadecimal += QString("%1").arg(vector[i], 2, 16, QChar('0'));
    }
    return hexadecimal;
}

// Obtains the data from a string of hexadecimal numbers
void Data::fromHexadecimal(const QString &hexadecimal)
{
    QString strippedHexadecimal(hexadecimal);
    strippedHexadecimal.remove(QChar(' '));
    int vecSize = strippedHexadecimal.size() / 2;
    vector.resize(vecSize);
    for (int i = 0; i < vecSize; ++i) {
        vector[i] = static_cast<quint8>(QStringRef(&strippedHexadecimal, 2 * i, 2).toUInt(nullptr, 16));
    }
}
