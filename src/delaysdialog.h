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

    quint16 interByteDelaySpinBoxValue() const;
    quint16 postAssertDelaySpinBoxValue() const;
    quint16 preDeassertDelaySpinBoxValue() const;
    void setInterByteDelaySpinBoxValue(quint16 itbytdly);
    void setPostAssertDelaySpinBoxValue(quint16 itbytdly);
    void setPreDeassertDelaySpinBoxValue(quint16 itbytdly);

private:
    Ui::DelaysDialog *ui;
};

#endif // DELAYSDIALOG_H
