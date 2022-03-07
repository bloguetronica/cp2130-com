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
        readConfiguration();
        this->setWindowTitle(tr("CP2130 Device (S/N: %1)").arg(serialstr_));
        initializeView();
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
    quint16 gpios = cp2130_.getGPIOs(errcnt, errstr);
    bool gpio0 = (CP2130::BMGPIO0 & gpios) != 0x0000;
    bool gpio1 = (CP2130::BMGPIO1 & gpios) != 0x0000;
    bool gpio2 = (CP2130::BMGPIO2 & gpios) != 0x0000;
    bool gpio3 = (CP2130::BMGPIO3 & gpios) != 0x0000;
    bool gpio4 = (CP2130::BMGPIO4 & gpios) != 0x0000;
    bool gpio5 = (CP2130::BMGPIO5 & gpios) != 0x0000;
    bool gpio6 = (CP2130::BMGPIO6 & gpios) != 0x0000;
    bool gpio7 = (CP2130::BMGPIO7 & gpios) != 0x0000;
    bool gpio8 = (CP2130::BMGPIO8 & gpios) != 0x0000;
    bool gpio9 = (CP2130::BMGPIO9 & gpios) != 0x0000;
    bool gpio10 = (CP2130::BMGPIO10 & gpios) != 0x0000;
    if (opCheck(tr("update-op"), errcnt, errstr)) {  // Update values if no errors occur (the string "update-op" should be translated to "Update")
        updateView(gpio0, gpio1, gpio2, gpio3, gpio4, gpio5, gpio6, gpio7, gpio8, gpio9, gpio10);
    }
}

// Partially disables device window
void::DeviceWindow::disableView()
{
    ui->centralWidget->setEnabled(false);
    ui->statusBar->setEnabled(false);
}

// Initializes the GPIO check boxes
void DeviceWindow::initializeGPIOControlBoxes()
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

// Initializes the SPI configuration controls
void DeviceWindow::initializeSPIConfigurationControls()
{
    if (spimodes_.size() != 0) {
        QList<QString> keys = spimodes_.keys();
        for (QString key : keys) {
            ui->comboBoxChannel->addItem(key);
        }
        CP2130::SPIMode spimode = spimodes_[ui->comboBoxChannel->currentText()];
        ui->comboBoxCSPinMode->setCurrentIndex(spimode.csmode);
        ui->comboBoxFrequency->setCurrentIndex(spimode.cfrq);
        ui->spinBoxCPol->setValue(spimode.cpol);
        ui->spinBoxCPha->setValue(spimode.cpha);
        ui->comboBoxChannel->setEnabled(true);
        ui->comboBoxCSPinMode->setEnabled(true);
        ui->comboBoxFrequency->setEnabled(true);
        ui->spinBoxCPol->setEnabled(true);
        ui->spinBoxCPha->setEnabled(true);
    }
}

// This is the routine that is used to initialize the device window
void DeviceWindow::initializeView()
{
    initializeGPIOControlBoxes();
    initializeSPIConfigurationControls();
}

// Checks for errors and validates (or ultimately halts) device operations
bool DeviceWindow::opCheck(const QString &op, int errcnt, QString errstr)
{
    bool retval;
    if (errcnt > 0) {
        if (cp2130_.disconnected()) {
            timer_->stop();  // This prevents further errors
            disableView();  // Disable device window
            cp2130_.close();
            QMessageBox::critical(this, tr("Error"), tr("Device disconnected.\n\nPlease reconnect it and try again."));
        } else {
            errstr.chop(1);  // Remove the last character, which is always a newline
            QMessageBox::critical(this, tr("Error"), tr("%1 operation returned the following error(s):\n– %2", "", errcnt).arg(op, errstr.replace("\n", "\n– ")));
            erracc_ += errcnt;
            if (erracc_ > ERR_LIMIT) {  // If the session accumulated more errors than the limit set by "ERR_LIMIT" [10]
                timer_->stop();  // Again, this prevents further errors
                disableView();  // Disable device window
                cp2130_.reset(errcnt, errstr);  // Try to reset the device for sanity purposes, but don't check if it was successful
                cp2130_.close();  // Ensure that the device is freed, even if the previous device reset is not effective (device_.reset() also frees the device interface, as an effect of re-enumeration)
                // It is essential that device_.close() is called, since some important checks rely on device_.isOpen() to retrieve a proper status
                QMessageBox::critical(this, tr("Error"), tr("Detected too many errors."));
            }
        }
        retval = false;  // Failed check
    } else {
        retval = true;  // Passed check
    }
    return retval;
}

// This is the routine that reads the configuration from the CP2130 OTP ROM
void DeviceWindow::readConfiguration()
{
    int errcnt = 0;
    QString errstr;
    pinconfig_ = cp2130_.getPinConfig(errcnt, errstr);
    if (pinconfig_.gpio0 == CP2130::PCCS) {
        spimodes_["0"] = cp2130_.getSPIMode(0, errcnt, errstr);
        spidelays_["0"] = cp2130_.getSPIDelays(0, errcnt, errstr);
    }
    if (pinconfig_.gpio1 == CP2130::PCCS) {
        spimodes_["1"] = cp2130_.getSPIMode(1, errcnt, errstr);
        spidelays_["1"] = cp2130_.getSPIDelays(1, errcnt, errstr);
    }
    if (pinconfig_.gpio2 == CP2130::PCCS) {
        spimodes_["2"] = cp2130_.getSPIMode(2, errcnt, errstr);
        spidelays_["2"] = cp2130_.getSPIDelays(2, errcnt, errstr);
    }
    if (pinconfig_.gpio3 == CP2130::PCCS) {
        spimodes_["3"] = cp2130_.getSPIMode(3, errcnt, errstr);
        spidelays_["3"] = cp2130_.getSPIDelays(3, errcnt, errstr);
    }
    if (pinconfig_.gpio4 == CP2130::PCCS) {
        spimodes_["4"] = cp2130_.getSPIMode(4, errcnt, errstr);
        spidelays_["4"] = cp2130_.getSPIDelays(4, errcnt, errstr);
    }
    if (pinconfig_.gpio5 == CP2130::PCCS) {
        spimodes_["5"] = cp2130_.getSPIMode(5, errcnt, errstr);
        spidelays_["5"] = cp2130_.getSPIDelays(5, errcnt, errstr);
    }
    if (pinconfig_.gpio6 == CP2130::PCCS) {
        spimodes_["6"] = cp2130_.getSPIMode(6, errcnt, errstr);
        spidelays_["6"] = cp2130_.getSPIDelays(6, errcnt, errstr);
    }
    if (pinconfig_.gpio7 == CP2130::PCCS) {
        spimodes_["7"] = cp2130_.getSPIMode(7, errcnt, errstr);
        spidelays_["7"] = cp2130_.getSPIDelays(7, errcnt, errstr);
    }
    if (pinconfig_.gpio8 == CP2130::PCCS) {
        spimodes_["8"] = cp2130_.getSPIMode(8, errcnt, errstr);
        spidelays_["8"] = cp2130_.getSPIDelays(8, errcnt, errstr);
    }
    if (pinconfig_.gpio9 == CP2130::PCCS) {
        spimodes_["9"] = cp2130_.getSPIMode(9, errcnt, errstr);
        spidelays_["9"] = cp2130_.getSPIDelays(9, errcnt, errstr);
    }
    if (pinconfig_.gpio10 == CP2130::PCCS) {
        spimodes_["10"] = cp2130_.getSPIMode(10, errcnt, errstr);
        spidelays_["10"] = cp2130_.getSPIDelays(10, errcnt, errstr);
    }
    if (errcnt > 0) {
        if (cp2130_.disconnected()) {
            cp2130_.close();
            QMessageBox::critical(this, tr("Error"), tr("Device disconnected.\n\nPlease reconnect it and try again."));
        } else {
            cp2130_.reset(errcnt, errstr);  // Try to reset the device for sanity purposes, but don't check if it was successful
            cp2130_.close();
            errstr.chop(1);  // Remove the last character, which is always a newline
            QMessageBox::critical(this, tr("Error"), tr("Read configuration operation returned the following error(s):\n– %1\n\nPlease try accessing the device again.", "", errcnt).arg(errstr.replace("\n", "\n– ")));
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
