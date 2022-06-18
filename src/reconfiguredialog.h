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


#ifndef RECONFIGUREDIALOG_H
#define RECONFIGUREDIALOG_H

// Includes
#include <QDialog>

namespace Ui {
class ReconfigureDialog;
}

class ReconfigureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReconfigureDialog(QWidget *parent = nullptr);
    ~ReconfigureDialog();

    void setupGPIO0ComboBox(int index);
    void setupGPIO1ComboBox(int index);
    void setupGPIO2ComboBox(int index);
    void setupGPIO3ComboBox(int index);
    void setupGPIO4ComboBox(int index);
    void setupGPIO5ComboBox(int index);
    void setupGPIO6ComboBox(int index);
    void setupGPIO7ComboBox(int index);
    void setupGPIO8ComboBox(int index);
    void setupGPIO9ComboBox(int index);
    void setupGPIO10ComboBox(int index);

private:
    Ui::ReconfigureDialog *ui;
};

#endif // RECONFIGUREDIALOG_H
