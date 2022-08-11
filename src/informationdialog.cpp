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

// Sets the text of "labelManufacturer"
void InformationDialog::setManufacturerLabelText(const QString &manufacturerstr)
{
    QString manufacturer = manufacturerstr;
    if (manufacturerstr.size() > 26) {
        manufacturer.truncate(24);
        manufacturer += "...";
    }
    ui->labelManufacturer->setText(manufacturer);
}

// Sets the text of "labelMaxPower"
void InformationDialog::setMaxPowerLabelText(quint8 maxpower)
{
    ui->labelMaxPower->setText(QString("%1 mA [0x%2]").arg(2 * maxpower).arg(maxpower, 2, 16, QChar('0')));
}

// Sets the text of "labelPID"
void InformationDialog::setPIDLabelText(quint16 pid)
{
    ui->labelPID->setText(QString("0x%1").arg(pid, 4, 16, QChar('0')));
}

// Sets the text of "labelPowerMode"
void InformationDialog::setPowerModeLabelText(quint8 powmode)
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
    ui->labelPowerMode->setText(powerMode);
}

// Sets the text of "labelProduct"
void InformationDialog::setProductLabelText(const QString &productstr)
{
    QString product = productstr;
    if (productstr.size() > 26) {
        product.truncate(24);
        product += "...";
    }
    ui->labelProduct->setText(product);
}

// Sets the text of "labelReleaseVersion"
void InformationDialog::setReleaseVersionLabelText(quint8 majrelease, quint8 minrelease)
{
    ui->labelReleaseVersion->setText(QString("%1.%2 [0x%3]").arg(majrelease).arg(minrelease).arg(majrelease << 8 | minrelease, 4, 16, QChar('0')));
}

// Sets the text of "labelSerial"
void InformationDialog::setSerialLabelText(const QString &serialstr)
{
    QString serial = serialstr;
    if (serialstr.size() > 26) {
        serial.truncate(24);
        serial += "...";
    }
    ui->labelSerial->setText(serial);
}

// Sets the text of "labelSiliconVersion"
void InformationDialog::setSiliconVersionLabelText(quint8 majver, quint8 minver)
{
    ui->labelSiliconVersion->setText(QString("%1.%2 [0x%3]").arg(majver).arg(minver).arg(majver << 8 | minver, 4, 16, QChar('0')));
}

// Sets the text of "labelVID"
void InformationDialog::setVIDLabelText(quint16 vid)
{
    ui->labelVID->setText(QString("0x%1").arg(vid, 4, 16, QChar('0')));
}
