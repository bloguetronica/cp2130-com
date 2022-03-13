/* CP2130 Commander - Version 1.0 for Debian Linux
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
#include "delaysdialog.h"
#include "ui_delaysdialog.h"

DelaysDialog::DelaysDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DelaysDialog)
{
    ui->setupUi(this);
}

DelaysDialog::~DelaysDialog()
{
    delete ui;
}

quint16 DelaysDialog::interByteDelaySpinBoxValue() const
{
    return static_cast<quint16>(ui->spinBoxInterByteDelay->value());
}

quint16 DelaysDialog::postAssertDelaySpinBoxValue() const
{
    return static_cast<quint16>(ui->spinBoxPostAssertDelay->value());
}

quint16 DelaysDialog::preDeassertDelaySpinBoxValue() const
{
    return static_cast<quint16>(ui->spinBoxPreDeassertDelay->value());
}

void DelaysDialog::setInterByteDelaySpinBoxValue(quint16 itbytdly)
{
    ui->spinBoxInterByteDelay->setValue(itbytdly);
}

void DelaysDialog::setPostAssertDelaySpinBoxValue(quint16 pstastdly)
{
    ui->spinBoxPostAssertDelay->setValue(pstastdly);
}

void DelaysDialog::setPreDeassertDelaySpinBoxValue(quint16 prdastdly)
{
    ui->spinBoxPreDeassertDelay->setValue(prdastdly);
}
