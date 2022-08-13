/* CP2130 Commander - Version 3.0 for Debian Linux
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
#include "cp2130.h"
#include "informationdialog.h"
#include "ui_informationdialog.h"

InformationDialog::InformationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InformationDialog)
{
    ui->setupUi(this);
}

InformationDialog::~InformationDialog()
{
    delete ui;
}

// Sets the text of "labelManufacturerValue"
void InformationDialog::setManufacturerValueLabelText(const QString &manufacturerstr)
{
    QString manufacturer = manufacturerstr;
    if (manufacturerstr.size() > 26) {
        manufacturer.truncate(24);
        manufacturer += "...";
    }
    ui->labelManufacturerValue->setText(manufacturer);
}

// Sets the text of "labelMaxPowerValue"
void InformationDialog::setMaxPowerValueLabelText(quint8 maxpower)
{
    ui->labelMaxPowerValue->setText(QString("%1 mA [0x%2]").arg(2 * maxpower).arg(maxpower, 2, 16, QChar('0')));
}

// Sets the text of "labelPIDValue"
void InformationDialog::setPIDValueLabelText(quint16 pid)
{
    ui->labelPIDValue->setText(QString("0x%1").arg(pid, 4, 16, QChar('0')));
}

// Sets the text of "labelPowerModeValue"
void InformationDialog::setPowerModeValueLabelText(quint8 powmode)
{
    QString powerMode;
    if (powmode == CP2130::PMBUSREGEN) {
        powerMode = tr("Bus-powered (regulator enabled)");
    } else if (powmode == CP2130::PMBUSREGEN) {
        powerMode = tr("Bus-powered (regulator disabled)");
    } else if (powmode == CP2130::PMBUSREGEN) {
        powerMode = tr("Self-powered");
    } else {
        powerMode = tr("Unknown");
    }
    ui->labelPowerModeValue->setText(powerMode);
}

// Sets the text of "labelProductValue"
void InformationDialog::setProductValueLabelText(const QString &productstr)
{
    QString product = productstr;
    if (productstr.size() > 26) {
        product.truncate(24);
        product += "...";
    }
    ui->labelProductValue->setText(product);
}

// Sets the text of "labelReleaseVersionValue"
void InformationDialog::setReleaseVersionValueLabelText(quint8 majrelease, quint8 minrelease)
{
    ui->labelReleaseVersionValue->setText(QString("%1.%2 [0x%3]").arg(majrelease).arg(minrelease).arg(majrelease << 8 | minrelease, 4, 16, QChar('0')));
}

// Sets the text of "labelSerialValue"
void InformationDialog::setSerialValueLabelText(const QString &serialstr)
{
    QString serial = serialstr;
    if (serialstr.size() > 26) {
        serial.truncate(24);
        serial += "...";
    }
    ui->labelSerialValue->setText(serial);
}

// Sets the text of "labelSiliconVersionValue"
void InformationDialog::setSiliconVersionValueLabelText(quint8 majver, quint8 minver)
{
    ui->labelSiliconVersionValue->setText(QString("%1.%2 [0x%3]").arg(majver).arg(minver).arg(majver << 8 | minver, 4, 16, QChar('0')));
}

// Sets the text of "labelVIDValue"
void InformationDialog::setVIDValueLabelText(quint16 vid)
{
    ui->labelVIDValue->setText(QString("0x%1").arg(vid, 4, 16, QChar('0')));
}
