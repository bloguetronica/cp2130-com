/* CP2130 Commander - Version 1.1 for Debian Linux
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
#include <QRegExp>
#include <QTextDocument>
#include <QThread>
#include <QVector>
#include <unistd.h>
#include "aboutdialog.h"
#include "convert.h"
#include "delaysdialog.h"
#include "informationdialog.h"
#include "devicewindow.h"
#include "ui_devicewindow.h"

// Definitions
const int ENUM_RETRIES = 10;  // Number of enumeration retries
const int ERR_LIMIT = 10;     // Error limit

DeviceWindow::DeviceWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DeviceWindow)
{
    ui->setupUi(this);
    ui->lineEditWrite->setValidator(new QRegExpValidator(QRegExp("[A-Fa-f\\d]+"), this));
}

DeviceWindow::~DeviceWindow()
{
    delete ui;
}

// Opens the device and prepares the corresponding window
void DeviceWindow::openDevice(quint16 vid, quint16 pid, const QString &serialstr)
{
    int err = cp2130_.open(vid, pid, serialstr);
    if (err == CP2130::SUCCESS) {  // Device was successfully opened
        vid_ = vid;  // Pass VID
        pid_ = pid;  // and PID
        serialstr_ = serialstr;  // and the serial number as well
        readConfiguration();  // Read device configuration
        this->setWindowTitle(tr("CP2130 Device (S/N: %1)").arg(serialstr_));
        initializeView();  // Initialize device window
        timer_ = new QTimer(this);  // Create a timer
        QObject::connect(timer_, SIGNAL(timeout()), this, SLOT(update()));
        timer_->start(100);  // Start the timer
    } else if (err == CP2130::ERROR_INIT) {  // Failed to initialize libusb
        QMessageBox::critical(this, tr("Critical Error"), tr("Could not initialize libusb.\n\nThis is a critical error and execution will be aborted."));
        exit(EXIT_FAILURE);  // This error is critical because libusb failed to initialize
    } else {
        if (err == CP2130::ERROR_NOT_FOUND) {  // Failed to find device
            QMessageBox::critical(this, tr("Error"), tr("Could not find device."));
        } else if (err == CP2130::ERROR_BUSY) {  // Failed to claim interface
            QMessageBox::critical(this, tr("Error"), tr("Device is currently unavailable.\n\nPlease confirm that the device is not in use."));
        }
        this->deleteLater();  // Close window after the subsequent show() call
    }
}

void DeviceWindow::on_actionAbout_triggered()
{
    AboutDialog aboutDialog;
    aboutDialog.exec();
}

void DeviceWindow::on_actionInformation_triggered()
{
    int errcnt = 0;
    QString errstr;
    InformationDialog infoDialog;
    infoDialog.setManufacturerLabelText(cp2130_.getManufacturerDesc(errcnt, errstr));
    infoDialog.setProductLabelText(cp2130_.getProductDesc(errcnt, errstr));
    infoDialog.setSerialLabelText(cp2130_.getSerialDesc(errcnt, errstr));  // It is important to read the serial number from the OTP ROM, instead of just passing the value of serialstr_
    CP2130::USBConfig config = cp2130_.getUSBConfig(errcnt, errstr);
    infoDialog.setVIDLabelText(config.vid);
    infoDialog.setPIDLabelText(config.pid);
    infoDialog.setReleaseVersionLabelText(config.majrel, config.minrel);
    infoDialog.setPowerModeLabelText(config.powmode);
    infoDialog.setMaxPowerLabelText(config.maxpow);
    CP2130::SiliconVersion siversion = cp2130_.getSiliconVersion(errcnt, errstr);
    infoDialog.setSiliconVersionLabelText(siversion.maj, siversion.min);
    if (opCheck(tr("device-information-retrieval-op"), errcnt, errstr)) {  // If error check passes (the string "device-information-retrieval-op" should be translated to "Device information retrieval")
        infoDialog.exec();
    }
}

void DeviceWindow::on_actionReset_triggered()
{
    resetDevice();
}

void DeviceWindow::on_checkBoxGPIO0_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO0(ui->checkBoxGPIO0->isChecked(), errcnt, errstr);  // Set GPIO.0 according to the user choice
    opCheck(tr("gpio0-switch-op"), errcnt, errstr);  // The string "gpio0-switch-op" should be translated to "GPIO.0 switch"
}

void DeviceWindow::on_checkBoxGPIO1_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO1(ui->checkBoxGPIO1->isChecked(), errcnt, errstr);  // Set GPIO.1 according to the user choice
    opCheck(tr("gpio1-switch-op"), errcnt, errstr);  // The string "gpio1-switch-op" should be translated to "GPIO.1 switch"
}

void DeviceWindow::on_checkBoxGPIO2_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO2(ui->checkBoxGPIO2->isChecked(), errcnt, errstr);  // Set GPIO.2 according to the user choice
    opCheck(tr("gpio2-switch-op"), errcnt, errstr);  // The string "gpio2-switch-op" should be translated to "GPIO.2 switch"
}

void DeviceWindow::on_checkBoxGPIO3_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO3(ui->checkBoxGPIO3->isChecked(), errcnt, errstr);  // Set GPIO.3 according to the user choice
    opCheck(tr("gpio3-switch-op"), errcnt, errstr);  // The string "gpio3-switch-op" should be translated to "GPIO.3 switch"
}

void DeviceWindow::on_checkBoxGPIO4_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO4(ui->checkBoxGPIO4->isChecked(), errcnt, errstr);  // Set GPIO.4 according to the user choice
    opCheck(tr("gpio4-switch-op"), errcnt, errstr);  // The string "gpio4-switch-op" should be translated to "GPIO.4 switch"
}

void DeviceWindow::on_checkBoxGPIO5_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO5(ui->checkBoxGPIO5->isChecked(), errcnt, errstr);  // Set GPIO.5 according to the user choice
    opCheck(tr("gpio5-switch-op"), errcnt, errstr);  // The string "gpio5-switch-op" should be translated to "GPIO.5 switch"
}

void DeviceWindow::on_checkBoxGPIO6_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO6(ui->checkBoxGPIO6->isChecked(), errcnt, errstr);  // Set GPIO.6 according to the user choice
    opCheck(tr("gpio6-switch-op"), errcnt, errstr);  // The string "gpio6-switch-op" should be translated to "GPIO.6 switch"
}

void DeviceWindow::on_checkBoxGPIO7_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO7(ui->checkBoxGPIO7->isChecked(), errcnt, errstr);  // Set GPIO.7 according to the user choice
    opCheck(tr("gpio7-switch-op"), errcnt, errstr);  // The string "gpio7-switch-op" should be translated to "GPIO.7 switch"
}

void DeviceWindow::on_checkBoxGPIO8_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO8(ui->checkBoxGPIO8->isChecked(), errcnt, errstr);  // Set GPIO.8 according to the user choice
    opCheck(tr("gpio8-switch-op"), errcnt, errstr);  // The string "gpio8-switch-op" should be translated to "GPIO.8 switch"
}

void DeviceWindow::on_checkBoxGPIO9_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO9(ui->checkBoxGPIO9->isChecked(), errcnt, errstr);  // Set GPIO.9 according to the user choice
    opCheck(tr("gpio9-switch-op"), errcnt, errstr);  // The string "gpio9-switch-op" should be translated to "GPIO.9 switch"
}

void DeviceWindow::on_checkBoxGPIO10_clicked()
{
    int errcnt = 0;
    QString errstr;
    cp2130_.setGPIO10(ui->checkBoxGPIO10->isChecked(), errcnt, errstr);  // Set GPIO.10 according to the user choice
    opCheck(tr("gpio10-switch-op"), errcnt, errstr);  // The string "gpio10-switch-op" should be translated to "GPIO.10 switch"
}

void DeviceWindow::on_comboBoxChannel_activated()
{
    displaySPIMode();  // It is important to note that the chip select corresponding to the selected channel is only enabled during an SPI transfer
}

void DeviceWindow::on_comboBoxCSPinMode_activated()
{
    configureSPIMode();
}

void DeviceWindow::on_comboBoxFrequency_activated()
{
    configureSPIMode();
}

void DeviceWindow::on_lineEditWrite_textChanged()
{
    int size = ui->lineEditWrite->text().size();
    bool enableWrite = size != 0 && size % 2 == 0;  // The buttons "Write" and "Write/Read" should only be enabled when the above referenced line edit contains a valid byte string
    ui->pushButtonWrite->setEnabled(enableWrite);
    ui->pushButtonWriteRead->setEnabled(enableWrite);
}

void DeviceWindow::on_lineEditWrite_textEdited()
{
    int curPosition = ui->lineEditWrite->cursorPosition();
    ui->lineEditWrite->setText(ui->lineEditWrite->text().toLower());
    ui->lineEditWrite->setCursorPosition(curPosition);
}

void DeviceWindow::on_pushButtonConfigureSPIDelays_clicked()
{
    quint8 channel = static_cast<quint8>(ui->comboBoxChannel->currentText().toUInt());
    int errcnt = 0;
    QString errstr;
    CP2130::SPIDelays spiDelays = cp2130_.getSPIDelays(channel, errcnt, errstr);
    if (opCheck(tr("spi-delays-retrieval-op"), errcnt, errstr)) {  // If error check passes (the string "spi-delays-retrieval-op" should be translated to "SPI delays retrieval")
        DelaysDialog delaysDialog;
        delaysDialog.setCSToggleCheckBox(spiDelays.cstglen);
        delaysDialog.setPostAssertDelaySpinBoxValue(spiDelays.pstastdly);
        delaysDialog.setPostAssertDelayCheckBox(spiDelays.pstasten);
        delaysDialog.setPreDeassertDelaySpinBoxValue(spiDelays.prdastdly);
        delaysDialog.setPreDeassertDelayCheckBox(spiDelays.prdasten);
        delaysDialog.setInterByteDelaySpinBoxValue(spiDelays.itbytdly);
        delaysDialog.setInterByteDelayCheckBox(spiDelays.itbyten);
        if (delaysDialog.exec() == QDialog::Accepted) {  // If the user clicks "OK", the new delay settings are applied to the current channel (only the first channel will be configured correctly, due to a design issue with the CP2130)
            spiDelays.cstglen = delaysDialog.csToggleCheckBoxIsChecked();
            spiDelays.pstasten = delaysDialog.postAssertDelayCheckBoxIsChecked();
            spiDelays.prdasten = delaysDialog.preDeassertDelayCheckBoxIsChecked();
            spiDelays.itbyten = delaysDialog.interByteDelayCheckBoxIsChecked();
            spiDelays.pstastdly = delaysDialog.postAssertDelaySpinBoxValue();
            spiDelays.prdastdly = delaysDialog.preDeassertDelaySpinBoxValue();
            spiDelays.itbytdly = delaysDialog.interByteDelaySpinBoxValue();
            cp2130_.configureSPIDelays(channel, spiDelays, errcnt, errstr);
            opCheck(tr("spi-delays-configuration-op"), errcnt, errstr);  // The string "spi-delays-configuration-op" should be translated to "SPI delays configuration"
        }
    }
}

void DeviceWindow::on_pushButtonRead_clicked()
{
    quint8 channel = static_cast<quint8>(ui->comboBoxChannel->currentText().toUInt());
    int errcnt = 0;
    QString errstr;
    cp2130_.selectCS(channel, errcnt, errstr);  // Enable the chip select corresponding to the selected channel, and disable any others
    QVector<quint8> result = cp2130_.spiRead(static_cast<quint32>(ui->spinBoxBytesToRead->value()), errcnt, errstr);  // Read from the SPI bus
    usleep(100);  // Wait 100us, in order to prevent possible errors while disabling the chip select (workaround)
    cp2130_.disableCS(channel, errcnt, errstr);  // Disable the previously enabled chip select
    if (opCheck(tr("spi-read-op"), errcnt, errstr)) {  // If no errors occur (the string "spi-read-op" should be translated to "SPI read")
        ui->lineEditRead->setText(DataToHexadecimal(result));
    }
}

void DeviceWindow::on_pushButtonWrite_clicked()
{
    quint8 channel = static_cast<quint8>(ui->comboBoxChannel->currentText().toUInt());
    int errcnt = 0;
    QString errstr;
    cp2130_.selectCS(channel, errcnt, errstr);  // Enable the chip select corresponding to the selected channel, and disable any others
    cp2130_.spiWrite(HexadecimalToData(ui->lineEditWrite->text()), errcnt, errstr);  // Write to the SPI bus
    usleep(100);  // Wait 100us, in order to prevent possible errors while disabling the chip select (workaround)
    cp2130_.disableCS(channel, errcnt, errstr);  // Disable the previously enabled chip select
    opCheck(tr("spi-write-op"), errcnt, errstr);  // The string "spi-write-op" should be translated to "SPI write"
    ui->lineEditRead->clear();
}

void DeviceWindow::on_pushButtonWriteRead_clicked()
{
    quint8 channel = static_cast<quint8>(ui->comboBoxChannel->currentText().toUInt());
    int errcnt = 0;
    QString errstr;
    cp2130_.selectCS(channel, errcnt, errstr);  // Enable the chip select corresponding to the selected channel, and disable any others
    QVector<quint8> result = cp2130_.spiWriteRead(HexadecimalToData(ui->lineEditWrite->text()), errcnt, errstr);  // Write to and read from the SPI bus, simultaneously
    usleep(100);  // Wait 100us, in order to prevent possible errors while disabling the chip select (workaround)
    cp2130_.disableCS(channel, errcnt, errstr);  // Disable the previously enabled chip select
    if (opCheck(tr("spi-write-read-op"), errcnt, errstr)) {  // If no errors occur (the string "spi-write-read-op" should be translated to "SPI write and read")
        ui->lineEditRead->setText(DataToHexadecimal(result));
    }
}

void DeviceWindow::on_spinBoxCPHA_valueChanged()
{
    configureSPIMode();
}

void DeviceWindow::on_spinBoxCPOL_valueChanged()
{
    configureSPIMode();
}

void DeviceWindow::on_spinBoxBytesToRead_valueChanged(int i)
{
    ui->pushButtonRead->setEnabled(i > 0);  // The button "Read" should only be enabled when the user specifies a number of bytes to read greater than zero
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
    if (opCheck(tr("update-op"), errcnt, errstr)) {  // If no errors occur (the string "update-op" should be translated to "Update")
        updateView(gpio0, gpio1, gpio2, gpio3, gpio4, gpio5, gpio6, gpio7, gpio8, gpio9, gpio10);  // Update values
    }
}

// Configures the SPI mode for the currently selected channel
void DeviceWindow::configureSPIMode()
{
    QString channel = ui->comboBoxChannel->currentText();
    CP2130::SPIMode spiMode;
    spiMode.csmode = ui->comboBoxCSPinMode->currentIndex() != 0;
    spiMode.cfrq = ui->comboBoxFrequency->currentIndex();
    spiMode.cpol = ui->spinBoxCPOL->value() != 0;
    spiMode.cpha = ui->spinBoxCPHA->value() != 0;
    int errcnt = 0;
    QString errstr;
    cp2130_.configureSPIMode(static_cast<quint8>(channel.toUInt()), spiMode, errcnt, errstr);
    if (opCheck(tr("spi-mode-configuration-op"), errcnt, errstr)) {  // If no errors occur (the string "spi-mode-configuration-op" should be translated to "SPI mode configuration")
        spiModeMap_[channel] = spiMode;  // Update "spiModeMap_" regarding the current channel
    }
}

// Partially disables device window
void DeviceWindow::disableView()
{
    ui->actionInformation->setEnabled(false);
    ui->actionReset->setEnabled(false);
    ui->centralWidget->setEnabled(false);
}

// Displays the SPI mode for the currently selected channel
void DeviceWindow::displaySPIMode()
{
    CP2130::SPIMode spiMode = spiModeMap_[ui->comboBoxChannel->currentText()];
    ui->comboBoxCSPinMode->setCurrentIndex(spiMode.csmode);
    ui->comboBoxFrequency->setCurrentIndex(spiMode.cfrq);
    ui->spinBoxCPOL->setValue(spiMode.cpol);
    ui->spinBoxCPHA->setValue(spiMode.cpha);
}

// Initializes the GPIO controls
void DeviceWindow::initializeGPIOControls()
{
    ui->checkBoxGPIO0->setEnabled(pinConfig_.gpio0 == CP2130::PCOUTOD || pinConfig_.gpio0 == CP2130::PCOUTPP);
    ui->checkBoxGPIO1->setEnabled(pinConfig_.gpio1 == CP2130::PCOUTOD || pinConfig_.gpio1 == CP2130::PCOUTPP);
    ui->checkBoxGPIO2->setEnabled(pinConfig_.gpio2 == CP2130::PCOUTOD || pinConfig_.gpio2 == CP2130::PCOUTPP);
    ui->checkBoxGPIO3->setEnabled(pinConfig_.gpio3 == CP2130::PCOUTOD || pinConfig_.gpio3 == CP2130::PCOUTPP);
    ui->checkBoxGPIO4->setEnabled(pinConfig_.gpio4 == CP2130::PCOUTOD || pinConfig_.gpio4 == CP2130::PCOUTPP);
    ui->checkBoxGPIO5->setEnabled(pinConfig_.gpio5 == CP2130::PCOUTOD || pinConfig_.gpio5 == CP2130::PCOUTPP);
    ui->checkBoxGPIO6->setEnabled(pinConfig_.gpio6 == CP2130::PCOUTOD || pinConfig_.gpio6 == CP2130::PCOUTPP);
    ui->checkBoxGPIO7->setEnabled(pinConfig_.gpio7 == CP2130::PCOUTOD || pinConfig_.gpio7 == CP2130::PCOUTPP);
    ui->checkBoxGPIO8->setEnabled(pinConfig_.gpio8 == CP2130::PCOUTOD || pinConfig_.gpio8 == CP2130::PCOUTPP);
    ui->checkBoxGPIO9->setEnabled(pinConfig_.gpio9 == CP2130::PCOUTOD || pinConfig_.gpio9 == CP2130::PCOUTPP);
    ui->checkBoxGPIO10->setEnabled(pinConfig_.gpio10 == CP2130::PCOUTOD || pinConfig_.gpio10 == CP2130::PCOUTPP);
    // Note that the enabled boxes correspond to pins that are configured as outputs
}

// Initializes the SPI controls
void DeviceWindow::initializeSPIControls()
{
    if (spiModeMap_.size() != 0) {  // In order for the SPI controls (including transfers) to be enabled, at least one pin should be configured to work as a chip select
        ui->comboBoxChannel->clear();
        QList<QString> keys = spiModeMap_.keys();
        for (QString key : keys) {
            ui->comboBoxChannel->addItem(key);
        }
        displaySPIMode();
        ui->comboBoxChannel->setEnabled(true);
        ui->pushButtonConfigureSPIDelays->setEnabled(true);
        ui->comboBoxCSPinMode->setEnabled(true);
        ui->comboBoxFrequency->setEnabled(true);
        ui->spinBoxCPOL->setEnabled(true);
        ui->spinBoxCPHA->setEnabled(true);
        ui->lineEditWrite->setEnabled(true);
        ui->lineEditRead->setEnabled(true);
        ui->spinBoxBytesToRead->setEnabled(true);
    }
}

// This is the routine that is used to initialize the device window
void DeviceWindow::initializeView()
{
    initializeGPIOControls();
    initializeSPIControls();
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
    pinConfig_ = cp2130_.getPinConfig(errcnt, errstr);
    if (pinConfig_.gpio0 == CP2130::PCCS) {
        spiModeMap_["0"] = cp2130_.getSPIMode(0, errcnt, errstr);
    }
    if (pinConfig_.gpio1 == CP2130::PCCS) {
        spiModeMap_["1"] = cp2130_.getSPIMode(1, errcnt, errstr);
    }
    if (pinConfig_.gpio2 == CP2130::PCCS) {
        spiModeMap_["2"] = cp2130_.getSPIMode(2, errcnt, errstr);
    }
    if (pinConfig_.gpio3 == CP2130::PCCS) {
        spiModeMap_["3"] = cp2130_.getSPIMode(3, errcnt, errstr);
    }
    if (pinConfig_.gpio4 == CP2130::PCCS) {
        spiModeMap_["4"] = cp2130_.getSPIMode(4, errcnt, errstr);
    }
    if (pinConfig_.gpio5 == CP2130::PCCS) {
        spiModeMap_["5"] = cp2130_.getSPIMode(5, errcnt, errstr);
    }
    if (pinConfig_.gpio6 == CP2130::PCCS) {
        spiModeMap_["6"] = cp2130_.getSPIMode(6, errcnt, errstr);
    }
    if (pinConfig_.gpio7 == CP2130::PCCS) {
        spiModeMap_["7"] = cp2130_.getSPIMode(7, errcnt, errstr);
    }
    if (pinConfig_.gpio8 == CP2130::PCCS) {
        spiModeMap_["8"] = cp2130_.getSPIMode(8, errcnt, errstr);
    }
    if (pinConfig_.gpio9 == CP2130::PCCS) {
        spiModeMap_["9"] = cp2130_.getSPIMode(9, errcnt, errstr);
    }
    if (pinConfig_.gpio10 == CP2130::PCCS) {
        spiModeMap_["10"] = cp2130_.getSPIMode(10, errcnt, errstr);
    }
    // Note that "spiModeMap_" is populated in relation to pins that are configured as chip select pins
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

// Resets the device
void DeviceWindow::resetDevice()
{
    timer_->stop();  // Stop the update timer momentarily, in order to avoid recurrent errors if the device gets disconnected during a reset, or other unexpected behavior
    ui->lineEditWrite->clear();
    ui->lineEditRead->clear();
    ui->spinBoxBytesToRead->setValue(0);
    int errcnt = 0;
    QString errstr;
    cp2130_.reset(errcnt, errstr);
    opCheck(tr("reset-op"), errcnt, errstr);  // The string "reset-op" should be translated to "Reset"
    if (cp2130_.isOpen()) {  // If opCheck() passes, thus, not closing the device
        cp2130_.close();  // Important! - This should be done always, even if the previous reset operation shows an error, because an error doesn't mean that a device reset was not effected
        int err;
        for (int i = 0; i < ENUM_RETRIES; ++i) {  // Verify enumeration according to the number of times set by "ENUM_RETRIES" [10]
            QThread::msleep(500);  // Wait 500ms each time
            err = cp2130_.open(vid_, pid_, serialstr_);
            if (err != CP2130::ERROR_NOT_FOUND) {  // Retry only if the device was not found yet (as it may take some time to enumerate)
                break;
            }
        }
        if (err == CP2130::SUCCESS) {  // Device was successfully reopened
            readConfiguration();  // Re-read device configuration
            erracc_ = 0;  // Zero the error count accumulator, since a new session gets started once the reset is done
            initializeView();  // Re-initialize device window
            timer_->start();  // Restart the timer
        } else {  // Failed to reopen device
            this->setEnabled(false);
            if (err == CP2130::ERROR_INIT) {  // Failed to initialize libusb
                QMessageBox::critical(this, tr("Critical Error"), tr("Could not reinitialize libusb.\n\nThis is a critical error and execution will be aborted."));
                exit(EXIT_FAILURE);  // This error is critical because libusb failed to initialize
            } else if (err == CP2130::ERROR_NOT_FOUND) {  // Failed to find device
                QMessageBox::critical(this, tr("Error"), tr("Device disconnected.\n\nPlease reconnect it and try again."));
                this->close();  // Close window
            } else if (err == CP2130::ERROR_BUSY) {  // Failed to claim interface
                QMessageBox::critical(this, tr("Error"), tr("Device ceased to be available.\n\nPlease verify that the device is not in use by another application."));
                this->close();  // Close window
            }
        }
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
