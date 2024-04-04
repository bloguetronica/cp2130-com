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


// Includes
#include "dividerdialog.h"
#include "ui_dividerdialog.h"

// Definitions
const float MCLK = 24000;  // 24 MHz clock

DividerDialog::DividerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DividerDialog)
{
    ui->setupUi(this);
    setExpectedFrequencyValueLabelText(0);  // This is required in order to show the value of the expected frequency when the dialog is opened
}

DividerDialog::~DividerDialog()
{
    delete ui;
}

void DividerDialog::on_spinBoxClockDivider_valueChanged(int i)
{
    setExpectedFrequencyValueLabelText(i);
}

// Returns the value of "spinBoxClockDivider"
quint8 DividerDialog::clockDividerSpinBoxValue()
{
    return static_cast<quint8>(ui->spinBoxClockDivider->value());
}

// Sets the value of "spinBoxClockDivider"
void DividerDialog::setClockDividerSpinBoxValue(quint8 divider)
{
    ui->spinBoxClockDivider->setValue(divider);
}

// Sets the text of "labelExpectedFrequencyValue" based on a given divider value
void DividerDialog::setExpectedFrequencyValueLabelText(quint8 divider)
{
    float frequency = MCLK / (divider == 0 ? 256 : divider);  // Frequency in kHz
    if (frequency < 1000) {
        ui->labelExpectedFrequencyValue->setText(QString("%1 kHz").arg(locale_.toString(frequency, 'f', 2)));
    } else {
        ui->labelExpectedFrequencyValue->setText(QString("%1 MHz").arg(locale_.toString(frequency / 1000, 'f', 2)));
    }
}
