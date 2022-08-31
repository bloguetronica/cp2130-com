/* CP2130 Commander - Version 3.1 for Debian Linux
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


#ifndef PINFUNCTIONSDIALOG_H
#define PINFUNCTIONSDIALOG_H

// Includes
#include <QDialog>
#include <QString>

namespace Ui {
class PinFunctionsDialog;
}

class PinFunctionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PinFunctionsDialog(QWidget *parent = nullptr);
    ~PinFunctionsDialog();

    void setGPIO0ValueLabelText(quint8 gpio0);
    void setGPIO1ValueLabelText(quint8 gpio1);
    void setGPIO2ValueLabelText(quint8 gpio2);
    void setGPIO3ValueLabelText(quint8 gpio3);
    void setGPIO4ValueLabelText(quint8 gpio4);
    void setGPIO5ValueLabelText(quint8 gpio5);
    void setGPIO6ValueLabelText(quint8 gpio6);
    void setGPIO7ValueLabelText(quint8 gpio7);
    void setGPIO8ValueLabelText(quint8 gpio8);
    void setGPIO9ValueLabelText(quint8 gpio9);
    void setGPIO10ValueLabelText(quint8 gpio10);

private:
    Ui::PinFunctionsDialog *ui;

    QString genericGPIOValue(quint8 gpio);
};

#endif  // PINFUNCTIONSDIALOG_H
