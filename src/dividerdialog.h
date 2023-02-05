/* CP2130 Commander - Version 4.1 for Debian Linux
   Copyright (c) 2022-2023 Samuel Lourenço

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


#ifndef DIVIDERDIALOG_H
#define DIVIDERDIALOG_H

// Includes
#include <QDialog>
#include <QLocale>

namespace Ui {
class DividerDialog;
}

class DividerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DividerDialog(QWidget *parent = nullptr);
    ~DividerDialog();

    quint8 clockDividerSpinBoxValue();
    void setClockDividerSpinBoxValue(quint8 divider);

private slots:
    void on_spinBoxClockDivider_valueChanged(int i);

private:
    Ui::DividerDialog *ui;
    QLocale locale_ = QLocale::system();

    void setExpectedFrequencyValueLabelText(quint8 divider);
};

#endif  // DIVIDERDIALOG_H
