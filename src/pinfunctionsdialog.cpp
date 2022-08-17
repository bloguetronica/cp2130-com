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
#include "pinfunctionsdialog.h"
#include "ui_pinfunctionsdialog.h"

PinFunctionsDialog::PinFunctionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PinFunctionsDialog)
{
    ui->setupUi(this);
}

PinFunctionsDialog::~PinFunctionsDialog()
{
    delete ui;
}

// Sets the text of "labelGPIO0Value"
void PinFunctionsDialog::setGPIO0ValueLabelText(quint8 gpio0)
{
    ui->labelGPIO0Value->setText(genericGPIOConfigString(gpio0));
}

// Sets the text of "labelGPIO1Value"
void PinFunctionsDialog::setGPIO1ValueLabelText(quint8 gpio1)
{
    ui->labelGPIO1Value->setText(genericGPIOConfigString(gpio1));
}

// Sets the text of "labelGPIO2Value"
void PinFunctionsDialog::setGPIO2ValueLabelText(quint8 gpio2)
{
    ui->labelGPIO2Value->setText(genericGPIOConfigString(gpio2));
}

// Sets the text of "labelGPIO3Value"
void PinFunctionsDialog::setGPIO3ValueLabelText(quint8 gpio3)
{
    QString gpio3str;
    switch (gpio3) {
        case CP2130::PCNRTR:
            gpio3str = tr("!RTR input");
            break;
        case CP2130::PCRTR:
            gpio3str = tr("RTR input");
            break;
        default:
            gpio3str = genericGPIOConfigString(gpio3);
    }
    ui->labelGPIO3Value->setText(gpio3str);
}

// Sets the text of "labelGPIO4Value"
void PinFunctionsDialog::setGPIO4ValueLabelText(quint8 gpio4)
{
    QString gpio4str;
    switch (gpio4) {
        case CP2130::PCEVTCNTRRE:
            gpio4str = tr("EVTCNTR rising edge input");
            break;
        case CP2130::PCEVTCNTRFE:
            gpio4str = tr("EVTCNTR falling edge input");
            break;
        case CP2130::PCEVTCNTRNP:
            gpio4str = tr("EVTCNTR negative pulse input");
            break;
        case CP2130::PCEVTCNTRPP:
            gpio4str = tr("EVTCNTR positive pulse input");
            break;
        default:
            gpio4str = genericGPIOConfigString(gpio4);
    }
    ui->labelGPIO4Value->setText(gpio4str);
}

// Sets the text of "labelGPIO5Value"
void PinFunctionsDialog::setGPIO5ValueLabelText(quint8 gpio5)
{
    QString gpio5str;
    if (gpio5 == CP2130::PCCLKOUT) {
        gpio5str = tr("CLKOUT push-pull output");
    } else {
        gpio5str = genericGPIOConfigString(gpio5);
    }
    ui->labelGPIO5Value->setText(gpio5str);
}

// Sets the text of "labelGPIO6Value"
void PinFunctionsDialog::setGPIO6ValueLabelText(quint8 gpio6)
{
    ui->labelGPIO6Value->setText(genericGPIOConfigString(gpio6));
}

// Sets the text of "labelGPIO7Value"
void PinFunctionsDialog::setGPIO7ValueLabelText(quint8 gpio7)
{
    ui->labelGPIO7Value->setText(genericGPIOConfigString(gpio7));
}

// Sets the text of "labelGPIO8Value"
void PinFunctionsDialog::setGPIO8ValueLabelText(quint8 gpio8)
{
    QString gpio8str;
    if (gpio8 == CP2130::PCSPIACT) {
        gpio8str = tr("SPIACT push-pull output");
    } else {
        gpio8str = genericGPIOConfigString(gpio8);
    }
    ui->labelGPIO8Value->setText(gpio8str);
}

// Sets the text of "labelGPIO9Value"
void PinFunctionsDialog::setGPIO9ValueLabelText(quint8 gpio9)
{
    QString gpio9str;
    if (gpio9 == CP2130::PCSSPND) {
        gpio9str = tr("SUSPEND push-pull output");
    } else {
        gpio9str = genericGPIOConfigString(gpio9);
    }
    ui->labelGPIO9Value->setText(gpio9str);
}

// Sets the text of "labelGPIO10Value"
void PinFunctionsDialog::setGPIO10ValueLabelText(quint8 gpio10)
{
    QString gpio10str;
    if (gpio10 == CP2130::PCNSSPND) {
        gpio10str = tr("!SUSPEND push-pull output");
    } else {
        gpio10str = genericGPIOConfigString(gpio10);
    }
    ui->labelGPIO10Value->setText(gpio10str);
}

// Returns the string corresponding to the GPIO configuration if it is generic (i.e. input, output or chip select)
QString PinFunctionsDialog::genericGPIOConfigString(quint8 gpio)
{
    QString gpiostr;
    switch (gpio) {
        case CP2130::PCIN:
            gpiostr = tr("Input");
            break;
        case CP2130::PCOUTOD:
            gpiostr = tr("Open-drain output");
            break;
        case CP2130::PCOUTPP:
            gpiostr = tr("Push-pull output");
            break;
        case CP2130::PCCS:
            gpiostr = tr("Chip select");
            break;
        default:
            gpiostr = tr("Unknown");
    }
    return gpiostr;
}
