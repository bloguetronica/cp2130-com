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
#include <QMessageBox>
#include "aboutdialog.h"
#include "devicewindow.h"
#include "ui_devicewindow.h"

// Definitions
const int ERR_LIMIT = 10;     // Error limit

DeviceWindow::DeviceWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DeviceWindow)
{
    ui->setupUi(this);
}

DeviceWindow::~DeviceWindow()
{
    delete ui;
}

// Opens the device and prepares the corresponding window
void DeviceWindow::openDevice(quint16 vid, quint16 pid, const QString &serialstr)
{
    int err = cp2130_.open(vid, pid, serialstr);
    if (err == CP2130::ERROR_INIT) {  // Failed to initialize libusb
        QMessageBox::critical(this, tr("Critical Error"), tr("Could not initialize libusb.\n\nThis is a critical error and execution will be aborted."));
        exit(EXIT_FAILURE);  // This error is critical because libusb failed to initialize
    } else if (err == CP2130::ERROR_NOT_FOUND) {  // Failed to find device
        QMessageBox::critical(this, tr("Error"), tr("Could not find device."));
        this->deleteLater();  // Close window after the subsequent show() call
    } else if (err == CP2130::ERROR_BUSY) {  // Failed to claim interface
        QMessageBox::critical(this, tr("Error"), tr("Device is currently unavailable.\n\nPlease confirm that the device is not in use."));
        this->deleteLater();  // Close window after the subsequent show() call
    } else {
        vid_ = vid;  // Pass VID
        pid_ = pid;  // and PID
        serialstr_ = serialstr;  // and the serial number as well
        readPinConfiguration();
        this->setWindowTitle(tr("CP2130 Device (S/N: %1)").arg(serialstr_));
        applyPinConfiguration();
        timer_ = new QTimer(this);  // Create a timer
        QObject::connect(timer_, SIGNAL(timeout()), this, SLOT(update()));
        timer_->start(200);
    }
}

void DeviceWindow::on_actionAbout_triggered()
{
    AboutDialog about;
    about.exec();
}

void DeviceWindow::on_checkBoxGPIO0_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO0(ui->checkBoxGPIO0->isChecked(), errcnt, errstr);
    opCheck(tr("gpio0-switch-op"), errcnt, errstr);  // The string "gpio0-switch-op" should be translated to "GPIO0 switch"
}

void DeviceWindow::on_checkBoxGPIO1_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO1(ui->checkBoxGPIO1->isChecked(), errcnt, errstr);
    opCheck(tr("gpio1-switch-op"), errcnt, errstr);  // The string "gpio1-switch-op" should be translated to "GPIO1 switch"
}

void DeviceWindow::on_checkBoxGPIO2_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO2(ui->checkBoxGPIO2->isChecked(), errcnt, errstr);
    opCheck(tr("gpio2-switch-op"), errcnt, errstr);  // The string "gpio2-switch-op" should be translated to "GPIO2 switch"
}

void DeviceWindow::on_checkBoxGPIO3_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO3(ui->checkBoxGPIO3->isChecked(), errcnt, errstr);
    opCheck(tr("gpio3-switch-op"), errcnt, errstr);  // The string "gpio3-switch-op" should be translated to "GPIO3 switch"
}

void DeviceWindow::on_checkBoxGPIO4_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO4(ui->checkBoxGPIO4->isChecked(), errcnt, errstr);
    opCheck(tr("gpio4-switch-op"), errcnt, errstr);  // The string "gpio4-switch-op" should be translated to "GPIO4 switch"
}

void DeviceWindow::on_checkBoxGPIO5_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO5(ui->checkBoxGPIO5->isChecked(), errcnt, errstr);
    opCheck(tr("gpio5-switch-op"), errcnt, errstr);  // The string "gpio5-switch-op" should be translated to "GPIO5 switch"
}

void DeviceWindow::on_checkBoxGPIO6_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO6(ui->checkBoxGPIO6->isChecked(), errcnt, errstr);
    opCheck(tr("gpio6-switch-op"), errcnt, errstr);  // The string "gpio6-switch-op" should be translated to "GPIO6 switch"
}

void DeviceWindow::on_checkBoxGPIO7_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO7(ui->checkBoxGPIO7->isChecked(), errcnt, errstr);
    opCheck(tr("gpio7-switch-op"), errcnt, errstr);  // The string "gpio7-switch-op" should be translated to "GPIO7 switch"
}

void DeviceWindow::on_checkBoxGPIO8_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO8(ui->checkBoxGPIO8->isChecked(), errcnt, errstr);
    opCheck(tr("gpio8-switch-op"), errcnt, errstr);  // The string "gpio8-switch-op" should be translated to "GPIO8 switch"
}

void DeviceWindow::on_checkBoxGPIO9_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO9(ui->checkBoxGPIO9->isChecked(), errcnt, errstr);
    opCheck(tr("gpio9-switch-op"), errcnt, errstr);  // The string "gpio9-switch-op" should be translated to "GPIO9 switch"
}

void DeviceWindow::on_checkBoxGPIO10_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO10(ui->checkBoxGPIO10->isChecked(), errcnt, errstr);
    opCheck(tr("gpio10-switch-op"), errcnt, errstr);  // The string "gpio10-switch-op" should be translated to "GPIO10 switch"
}

// This is the main update routine
void DeviceWindow::update()
{
    int errcnt = 0;
    QString errstr;
    bool gpio0 = cp2130_.getGPIO0(errcnt, errstr);
    bool gpio1 = cp2130_.getGPIO1(errcnt, errstr);
    bool gpio2 = cp2130_.getGPIO2(errcnt, errstr);
    bool gpio3 = cp2130_.getGPIO3(errcnt, errstr);
    bool gpio4 = cp2130_.getGPIO4(errcnt, errstr);
    bool gpio5 = cp2130_.getGPIO5(errcnt, errstr);
    bool gpio6 = cp2130_.getGPIO6(errcnt, errstr);
    bool gpio7 = cp2130_.getGPIO7(errcnt, errstr);
    bool gpio8 = cp2130_.getGPIO8(errcnt, errstr);
    bool gpio9 = cp2130_.getGPIO9(errcnt, errstr);
    bool gpio10 = cp2130_.getGPIO10(errcnt, errstr);
    if (opCheck(tr("update-op"), errcnt, errstr)) {  // Update values if no errors occur (the string "update-op" should be translated to "Update")
        updateView(gpio0, gpio1, gpio2, gpio3, gpio4, gpio5, gpio6, gpio7, gpio8, gpio9, gpio10);
    }
}

// This is the routine that is used to apply the pin configuration to the window
void DeviceWindow::applyPinConfiguration()
{
    enableGPIOControlBoxes();
    enableChipSelectBoxes();
}

// Partially disables device window
void::DeviceWindow::disableView()
{
    ui->centralWidget->setEnabled(false);
    ui->statusBar->setEnabled(false);
}

// Enables or disables the GPIO check boxes
void DeviceWindow::enableChipSelectBoxes()
{
    ui->checkBoxCS0->setEnabled(pinconfig_.gpio0 == CP2130::PCCS);
    ui->checkBoxCS1->setEnabled(pinconfig_.gpio1 == CP2130::PCCS);
    ui->checkBoxCS2->setEnabled(pinconfig_.gpio2 == CP2130::PCCS);
    ui->checkBoxCS3->setEnabled(pinconfig_.gpio3 == CP2130::PCCS);
    ui->checkBoxCS4->setEnabled(pinconfig_.gpio4 == CP2130::PCCS);
    ui->checkBoxCS5->setEnabled(pinconfig_.gpio5 == CP2130::PCCS);
    ui->checkBoxCS6->setEnabled(pinconfig_.gpio6 == CP2130::PCCS);
    ui->checkBoxCS7->setEnabled(pinconfig_.gpio7 == CP2130::PCCS);
    ui->checkBoxCS8->setEnabled(pinconfig_.gpio8 == CP2130::PCCS);
    ui->checkBoxCS9->setEnabled(pinconfig_.gpio9 == CP2130::PCCS);
    ui->checkBoxCS10->setEnabled(pinconfig_.gpio10 == CP2130::PCCS);
}

// Enables or disables the GPIO check boxes
void DeviceWindow::enableGPIOControlBoxes()
{
    ui->checkBoxGPIO0->setEnabled(pinconfig_.gpio0 == CP2130::PCOUTOD || pinconfig_.gpio0 == CP2130::PCOUTPP);
    ui->checkBoxGPIO1->setEnabled(pinconfig_.gpio1 == CP2130::PCOUTOD || pinconfig_.gpio1 == CP2130::PCOUTPP);
    ui->checkBoxGPIO2->setEnabled(pinconfig_.gpio2 == CP2130::PCOUTOD || pinconfig_.gpio2 == CP2130::PCOUTPP);
    ui->checkBoxGPIO3->setEnabled(pinconfig_.gpio3 == CP2130::PCOUTOD || pinconfig_.gpio3 == CP2130::PCOUTPP);
    ui->checkBoxGPIO4->setEnabled(pinconfig_.gpio4 == CP2130::PCOUTOD || pinconfig_.gpio4 == CP2130::PCOUTPP);
    ui->checkBoxGPIO5->setEnabled(pinconfig_.gpio5 == CP2130::PCOUTOD || pinconfig_.gpio5 == CP2130::PCOUTPP);
    ui->checkBoxGPIO6->setEnabled(pinconfig_.gpio6 == CP2130::PCOUTOD || pinconfig_.gpio6 == CP2130::PCOUTPP);
    ui->checkBoxGPIO7->setEnabled(pinconfig_.gpio7 == CP2130::PCOUTOD || pinconfig_.gpio7 == CP2130::PCOUTPP);
    ui->checkBoxGPIO8->setEnabled(pinconfig_.gpio8 == CP2130::PCOUTOD || pinconfig_.gpio8 == CP2130::PCOUTPP);
    ui->checkBoxGPIO9->setEnabled(pinconfig_.gpio9 == CP2130::PCOUTOD || pinconfig_.gpio9 == CP2130::PCOUTPP);
    ui->checkBoxGPIO10->setEnabled(pinconfig_.gpio10 == CP2130::PCOUTOD || pinconfig_.gpio10 == CP2130::PCOUTPP);
}

// Checks for errors and validates (or ultimately halts) device operations
bool DeviceWindow::opCheck(const QString &op, int errcnt, QString errstr)
{
    bool retval;
    if (errcnt > 0) {
        if (cp2130_.disconnected()) {
            timer_->stop();  // This prevents further errors
            QMessageBox::critical(this, tr("Error"), tr("Device disconnected.\n\nThe corresponding window will be disabled."));
            disableView();  // Disable device window
            cp2130_.close();
        } else {
            errstr.chop(1);  // Remove the last character, which is always a newline
            QMessageBox::critical(this, tr("Error"), tr("%1 operation returned the following error(s):\n– %2", "", errcnt).arg(op, errstr.replace("\n", "\n– ")));
            erracc_ += errcnt;
            if (erracc_ > ERR_LIMIT) {  // If the session accumulated more errors than the limit set by "ERR_LIMIT" [10]
                timer_->stop();  // Again, this prevents further errors
                QMessageBox::critical(this, tr("Error"), tr("Detected too many errors.\n\nThe device window will be disabled."));
                disableView();  // Disable device window
                cp2130_.reset(errcnt, errstr);  // Try to reset the device for sanity purposes, but don't check if it was successful
                cp2130_.close();  // Ensure that the device is freed, even if the previous device reset is not effective (device_.reset() also frees the device interface, as an effect of re-enumeration)
                // It is essential that device_.close() is called, since some important checks rely on device_.isOpen() to retrieve a proper status
            }
        }
        retval = false;  // Failed check
    } else {
        retval = true;  // Passed check
    }
    return retval;
}

// This is the routine that reads the pin configuration from the CP2130 OTP ROM
void DeviceWindow::readPinConfiguration()
{
    int errcnt = 0;
    QString errstr;
    pinconfig_ = cp2130_.getPinConfig(errcnt, errstr);
    if (errcnt > 0) {
        if (cp2130_.disconnected()) {
            cp2130_.close();
            QMessageBox::critical(this, tr("Error"), tr("Device disconnected."));
        } else {
            cp2130_.reset(errcnt, errstr);  // Try to reset the device for sanity purposes, but don't check if it was successful
            cp2130_.close();
            errstr.chop(1);  // Remove the last character, which is always a newline
            QMessageBox::critical(this, tr("Error"), tr("Read operation returned the following error(s):\n– %1\n\nPlease try accessing the device again.", "", errcnt).arg(errstr.replace("\n", "\n– ")));
        }
        this->deleteLater();  // In a context where the window is already visible, it has the same effect as this->close()
    }
}

// Updates the view
void DeviceWindow::updateView(bool gpio0, bool gpio1, bool gpio2, bool gpio3, bool gpio4, bool gpio5, bool gpio6, bool gpio7, bool gpio8, bool gpio9, bool gpio10)
{
    ui->checkBoxGPIO0->setChecked(gpio0);
    ui->checkBoxGPIO1->setChecked(gpio1);
    ui->checkBoxGPIO2->setChecked(gpio2);
    ui->checkBoxGPIO3->setChecked(gpio3);
    ui->checkBoxGPIO4->setChecked(gpio4);
    ui->checkBoxGPIO5->setChecked(gpio5);
    ui->checkBoxGPIO6->setChecked(gpio6);
    ui->checkBoxGPIO7->setChecked(gpio7);
    ui->checkBoxGPIO8->setChecked(gpio8);
    ui->checkBoxGPIO9->setChecked(gpio9);
    ui->checkBoxGPIO10->setChecked(gpio10);
}
