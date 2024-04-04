/* CP2130 Commander - Version 5.0 for Debian Linux
   Copyright (c) 2022-2024 Samuel Lourenço

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


#ifndef DATA_H
#define DATA_H

// Includes
#include <QString>
#include <QVector>

struct Data
{
    QVector<quint8> vector;

    QVector<quint8> fragment(size_t index, size_t size) const;
    QString toHexadecimal() const;

    void fromHexadecimal(const QString &hexadecimal);
};

#endif  // DATA_H
