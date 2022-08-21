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


#ifndef DELAYSDIALOG_H
#define DELAYSDIALOG_H

// Includes
#include <QDialog>

namespace Ui {
class DelaysDialog;
}

class DelaysDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DelaysDialog(QWidget *parent = nullptr);
    ~DelaysDialog();

    bool csToggleCheckBoxIsChecked();
    bool interByteDelayCheckBoxIsChecked();
    quint16 interByteDelaySpinBoxValue();
    bool postAssertDelayCheckBoxIsChecked();
    quint16 postAssertDelaySpinBoxValue();
    bool preDeassertDelayCheckBoxIsChecked();
    quint16 preDeassertDelaySpinBoxValue();
    void setCSToggleCheckBox(bool cstglen);
    void setInterByteDelayCheckBox(bool itbyten);
    void setInterByteDelaySpinBoxValue(quint16 itbytdly);
    void setPostAssertDelayCheckBox(bool pstasten);
    void setPostAssertDelaySpinBoxValue(quint16 itbytdly);
    void setPreDeassertDelayCheckBox(bool prdasten);
    void setPreDeassertDelaySpinBoxValue(quint16 itbytdly);

private:
    Ui::DelaysDialog *ui;
};

#endif  // DELAYSDIALOG_H
