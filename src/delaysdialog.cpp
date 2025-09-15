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

// Returns the state of "checkBoxCSToggle"
bool DelaysDialog::csToggleCheckBoxIsChecked()
{
    return ui->checkBoxCSToggle->isChecked();
}

// Returns the state of "checkBoxInterByteDelay"
bool DelaysDialog::interByteDelayCheckBoxIsChecked()
{
    return ui->checkBoxInterByteDelay->isChecked();
}

// Returns the value of "spinBoxInterByteDelay"
quint16 DelaysDialog::interByteDelaySpinBoxValue()
{
    return static_cast<quint16>(ui->spinBoxInterByteDelay->value());
}

// Returns the state of "checkBoxPostAssertDelay"
bool DelaysDialog::postAssertDelayCheckBoxIsChecked()
{
    return ui->checkBoxPostAssertDelay->isChecked();
}

// Returns the value of "spinBoxPostAssertDelay"
quint16 DelaysDialog::postAssertDelaySpinBoxValue()
{
    return static_cast<quint16>(ui->spinBoxPostAssertDelay->value());
}

// Returns the state of "checkBoxPreDeassertDelay"
bool DelaysDialog::preDeassertDelayCheckBoxIsChecked()
{
    return ui->checkBoxPreDeassertDelay->isChecked();
}

// Returns the value of "spinBoxPreDeassertDelay"
quint16 DelaysDialog::preDeassertDelaySpinBoxValue()
{
    return static_cast<quint16>(ui->spinBoxPreDeassertDelay->value());
}

// Sets the state of "checkBoxCSToggle"
void DelaysDialog::setCSToggleCheckBox(bool csToggleEnable)
{
    ui->checkBoxCSToggle->setChecked(csToggleEnable);
}

// Sets the state of "checkBoxInterByteDelay"
void DelaysDialog::setInterByteDelayCheckBox(bool interByteDelayEnable)
{
    ui->checkBoxInterByteDelay->setChecked(interByteDelayEnable);
}

// Sets the value of "spinBoxInterByteDelay"
void DelaysDialog::setInterByteDelaySpinBoxValue(quint16 interByteDelay)
{
    ui->spinBoxInterByteDelay->setValue(interByteDelay);
}

// Sets the state of "checkBoxPostAssertDelay"
void DelaysDialog::setPostAssertDelayCheckBox(bool postAssertDelayEnable)
{
    ui->checkBoxPostAssertDelay->setChecked(postAssertDelayEnable);
}

// Sets the value of "spinBoxPostAssertDelay"
void DelaysDialog::setPostAssertDelaySpinBoxValue(quint16 postAssertDelay)
{
    ui->spinBoxPostAssertDelay->setValue(postAssertDelay);
}

// Sets the state of "checkBoxPreDeassertDelay"
void DelaysDialog::setPreDeassertDelayCheckBox(bool preDeassertDelayEnable)
{
    ui->checkBoxPreDeassertDelay->setChecked(preDeassertDelayEnable);
}

// Sets the value of "spinBoxPreDeassertDelay"
void DelaysDialog::setPreDeassertDelaySpinBoxValue(quint16 preDeassertDelay)
{
    ui->spinBoxPreDeassertDelay->setValue(preDeassertDelay);
}
