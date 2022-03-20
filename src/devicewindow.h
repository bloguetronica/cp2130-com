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


#ifndef DEVICEWINDOW_H
#define DEVICEWINDOW_H

// Includes
#include <QMainWindow>
#include <QMap>
#include <QString>
#include <QTimer>
#include "cp2130.h"

namespace Ui {
class DeviceWindow;
}

class DeviceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DeviceWindow(QWidget *parent = nullptr);
    ~DeviceWindow();

    void openDevice(quint16 vid, quint16 pid, const QString &serialstr);

private slots:
    void on_actionAbout_triggered();
    void on_actionInformation_triggered();
    void on_actionReset_triggered();
    void on_checkBoxGPIO0_clicked();
    void on_checkBoxGPIO1_clicked();
    void on_checkBoxGPIO2_clicked();
    void on_checkBoxGPIO3_clicked();
    void on_checkBoxGPIO4_clicked();
    void on_checkBoxGPIO5_clicked();
    void on_checkBoxGPIO6_clicked();
    void on_checkBoxGPIO7_clicked();
    void on_checkBoxGPIO8_clicked();
    void on_checkBoxGPIO9_clicked();
    void on_checkBoxGPIO10_clicked();
    void on_comboBoxChannel_activated();
    void on_comboBoxCSPinMode_activated();
    void on_comboBoxFrequency_activated();
    void on_lineEditWrite_textEdited();
    void on_pushButtonConfigureSPIDelays_clicked();
    void on_pushButtonRead_clicked();
    void on_pushButtonWrite_clicked();
    void on_pushButtonWriteRead_clicked();
    void on_spinBoxCPHA_valueChanged();
    void on_spinBoxCPOL_valueChanged();
    void on_spinBoxBytesToRead_valueChanged();
    void update();

private:
    Ui::DeviceWindow *ui;
    CP2130 cp2130_;
    CP2130::PinConfig pinConfig_;
    QMap<QString, CP2130::SPIDelays> spiDelaysMap_;
    QMap<QString, CP2130::SPIMode> spiModeMap_;
    QString serialstr_;
    quint16 pid_, vid_;
    QTimer *timer_;
    int erracc_ = 0;

    void configureSPIMode();
    void disableView();
    void displaySPIMode();
    void initializeGPIOControlBoxes();
    void initializeSPIControls();
    void initializeView();
    bool opCheck(const QString &op, int errcnt, QString errstr);
    void readConfiguration();
    void resetDevice();
    void updateView(bool gpio0, bool gpio1, bool gpio2, bool gpio3, bool gpio4, bool gpio5, bool gpio6, bool gpio7, bool gpio8, bool gpio9, bool gpio10);
};

#endif // DEVICEWINDOW_H
