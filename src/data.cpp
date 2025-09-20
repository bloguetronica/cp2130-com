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
#include <QStringRef>
#include "data.h"

// Returns a fragment of data from the given index, with the specified size (cleaned up and optimized in version 1.5.1)
// If the given size, when added to the index, goes out of boundaries, the returned QVector will have a smaller than expected size
QVector<quint8> Data::fragment(size_t index, size_t size) const
{
    size_t vectorSize = static_cast<size_t>(vector.size());
    if (index < vectorSize) {  // This is essential to prevent integer underflow (see subtraction in the line below)
        size = size + index > vectorSize ? vectorSize - index : size;
    } else {
        size = 0;
    }
    QVector<quint8> dataFragment(size);  // The return variable is declared here and not at the beginning, so it can be declared using the conditioned size (which might not match the value passed via the corresponding argument)
    for (size_t i = 0; i < size; ++i) {
        dataFragment[static_cast<int>(i)] = vector.at(static_cast<int>(index + i));  // Since version 1.5.1, QVector::at() is used rather than QVector::operator[] to get values from the vector
    }
    return dataFragment;
}

// Converts to a string of hexadecimal numbers (optimized in version 1.5.1)
QString Data::toHexadecimal() const
{
    QString hexadecimal;
    for (int i = 0; i < vector.size(); ++i) {
        if (i > 0) {
            hexadecimal += " ";
        }
        hexadecimal += QString("%1").arg(vector.at(i), 2, 16, QChar('0'));  // Since version 1.5.1, QVector::at() is used in place of QVector::operator[] to get values from the vector
    }
    return hexadecimal;
}

// Obtains the data from a string of hexadecimal numbers
void Data::fromHexadecimal(const QString &hexadecimal)
{
    QString strippedHexadecimal(hexadecimal);
    strippedHexadecimal.remove(QChar(' ')).remove(QChar('\n'));  // Newline characters are also removed to prevent conversion errors (version 4.1 bug fix)
    int vectorSize = strippedHexadecimal.size() / 2;
    vector.resize(vectorSize);
    for (int i = 0; i < vectorSize; ++i) {
        vector[i] = static_cast<quint8>(QStringRef(&strippedHexadecimal, 2 * i, 2).toUInt(nullptr, 16));
    }
}
