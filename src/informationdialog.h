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


#ifndef INFORMATIONDIALOG_H
#define INFORMATIONDIALOG_H

// Includes
#include <QDialog>

namespace Ui {
class InformationDialog;
}

class InformationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InformationDialog(QWidget *parent = nullptr);
    ~InformationDialog();

    void setManufacturerLabelText(const QString &manufacturerstr);
    void setMaxPowerLabelText(quint8 maxpower);
    void setPIDLabelText(quint16 pid);
    void setPowerModeLabelText(quint8 powmode);
    void setProductLabelText(const QString &productstr);
    void setReleaseVersionLabelText(quint8 majrelease, quint8 minrelease);
    void setVIDLabelText(quint16 vid);
    void setSerialLabelText(const QString &serialstr);
    void setSiliconVersionLabelText(quint8 majver, quint8 minver);

private:
    Ui::InformationDialog *ui;
};

#endif  // INFORMATIONDIALOG_H
