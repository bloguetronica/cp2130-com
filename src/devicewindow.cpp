/* CP2130 Commander - Version 5.0 for Debian Linux
   Copyright (c) 2022-2024 Samuel Lourenço

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
#include <cmath>
#include <QClipboard>
#include <QElapsedTimer>
#include <QGuiApplication>
#include <QMessageBox>
#include <QProgressDialog>
#include <QRegExp>
#include <QRegExpValidator>
#include <QThread>
#include <QVector>
#include <unistd.h>
#include "common.h"
#include "delaysdialog.h"
#include "dividerdialog.h"
#include "devicewindow.h"
#include "ui_devicewindow.h"

// Definitions
const int ENUM_RETRIES = 10;   // Number of enumeration retries
const int ERR_LIMIT = 10;      // Error limit
const float TIME_LIMIT = 100;  // Soft time limit per partial transfer, in milliseconds

DeviceWindow::DeviceWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DeviceWindow)
{
    ui->setupUi(this);
    ui->lineEditWrite->setValidator(new QRegExpValidator(QRegExp("[A-Fa-f\\d\\s]+"), this));  // Spaces are allowed since version 2.0
    labelStatus_ = new QLabel(this);
    this->statusBar()->addWidget(labelStatus_);
    timer_ = new QTimer(this);  // The timer is initialized in the constructor since version 3.1
    connect(QGuiApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(updatePushButtonClipboardPasteWrite()));  // Added in version 4.1 and modified in version 5.0
    connect(timer_, SIGNAL(timeout()), this, SLOT(update()));  // This call doesn't need to be scoped (fixed in version 4.0)
}

DeviceWindow::~DeviceWindow()
{
    delete ui;
}

// Checks if the device window is currently fully enabled (implemented in version 3.0)
bool DeviceWindow::isViewEnabled()
{
    return viewEnabled_;
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
    showAboutDialog();  // Implemented in "common.h" and "common.cpp" since version 4.0
}

// Implemented in version 3.0
void DeviceWindow::on_actionGPIOPinFunctions_triggered()
{
    if (pinFunctionsDialog_.isNull()) {  // If the dialog is not open (implemented in version 4.0, because the pin functions dialog is now modeless)
        pinFunctionsDialog_ = new PinFunctionsDialog(this);  // The dialog is no longer modal (version 4.0 feature)
        pinFunctionsDialog_->setAttribute(Qt::WA_DeleteOnClose);  // It is important to delete the dialog in memory once closed, in order to force the application to retrieve information about the device if the window is opened again
        pinFunctionsDialog_->setWindowTitle(tr("GPIO Pin Functions (S/N: %1)").arg(serialstr_));
        pinFunctionsDialog_->setGPIO0ValueLabelText(pinConfig_.gpio0);
        pinFunctionsDialog_->setGPIO1ValueLabelText(pinConfig_.gpio1);
        pinFunctionsDialog_->setGPIO2ValueLabelText(pinConfig_.gpio2);
        pinFunctionsDialog_->setGPIO3ValueLabelText(pinConfig_.gpio3);
        pinFunctionsDialog_->setGPIO4ValueLabelText(pinConfig_.gpio4);
        pinFunctionsDialog_->setGPIO5ValueLabelText(pinConfig_.gpio5);
        pinFunctionsDialog_->setGPIO6ValueLabelText(pinConfig_.gpio6);
        pinFunctionsDialog_->setGPIO7ValueLabelText(pinConfig_.gpio7);
        pinFunctionsDialog_->setGPIO8ValueLabelText(pinConfig_.gpio8);
        pinFunctionsDialog_->setGPIO9ValueLabelText(pinConfig_.gpio9);
        pinFunctionsDialog_->setGPIO10ValueLabelText(pinConfig_.gpio10);
        pinFunctionsDialog_->show();
    } else {
        pinFunctionsDialog_->showNormal();  // Required if the dialog is minimized
        pinFunctionsDialog_->activateWindow();  // Set focus on the previous dialog (dialog is raised and selected)
    }
}

void DeviceWindow::on_actionInformation_triggered()
{
    if (informationDialog_.isNull()) {  // If the dialog is not open (implemented in version 4.0, because the device information dialog is now modeless)
        int errcnt = 0;
        QString errstr;
        informationDialog_ = new InformationDialog(this);  // The dialog is no longer modal (version 4.0 feature)
        informationDialog_->setAttribute(Qt::WA_DeleteOnClose);  // It is important to delete the dialog in memory once closed, in order to force the application to retrieve information about the device if the window is opened again
        informationDialog_->setWindowTitle(tr("Device Information (S/N: %1)").arg(serialstr_));
        informationDialog_->setManufacturerValueLabelText(cp2130_.getManufacturerDesc(errcnt, errstr));
        informationDialog_->setProductValueLabelText(cp2130_.getProductDesc(errcnt, errstr));
        informationDialog_->setSerialValueLabelText(cp2130_.getSerialDesc(errcnt, errstr));  // It is important to read the serial number from the OTP ROM, instead of just passing the value of serialstr_
        CP2130::USBConfig config = cp2130_.getUSBConfig(errcnt, errstr);
        informationDialog_->setVIDValueLabelText(config.vid);
        informationDialog_->setPIDValueLabelText(config.pid);
        informationDialog_->setReleaseVersionValueLabelText(config.majrel, config.minrel);
        informationDialog_->setPowerModeValueLabelText(config.powmode);
        informationDialog_->setMaxPowerValueLabelText(config.maxpow);
        CP2130::SiliconVersion siversion = cp2130_.getSiliconVersion(errcnt, errstr);
        informationDialog_->setSiliconVersionValueLabelText(siversion.maj, siversion.min);
        if (opCheck(tr("device-information-retrieval-op"), errcnt, errstr)) {  // If error check passes (the string "device-information-retrieval-op" should be translated to "Device information retrieval")
            informationDialog_->show();
        }
    } else {
        informationDialog_->showNormal();  // Required if the dialog is minimized
        informationDialog_->activateWindow();  // Set focus on the previous dialog (dialog is raised and selected)
    }
}

void DeviceWindow::on_actionReset_triggered()
{
    resetDevice();
}

// Implemented in version 3.0
void DeviceWindow::on_actionSetClockDivider_triggered()
{
    int errcnt = 0;
    QString errstr;
    DividerDialog dividerDialog(this);  // The clock divider dialog is now a child of the device window, as it should (fixed in version 4.0)
    dividerDialog.setClockDividerSpinBoxValue(cp2130_.getClockDivider(errcnt, errstr));
    if (opCheck(tr("clock-divider-retrieval-op"), errcnt, errstr) && dividerDialog.exec() == QDialog::Accepted) {  // If error check passes (the string "clock-divider-retrieval-op" should be translated to "Clock divider retrieval") and if the user click "OK" on the dialog that opens after that, the new clock divider setting is applied
        cp2130_.setClockDivider(dividerDialog.clockDividerSpinBoxValue(), errcnt, errstr);
        opCheck(tr("clock-divider-setting-op"), errcnt, errstr);  // The string "clock-divider-setting-op" should be translated to "Clock divider setting"
    }
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

// Implemented in version 3.0
void DeviceWindow::on_comboBoxTriggerMode_activated()
{
    setEventCounter();
}

// Implemented in version 4.1
void DeviceWindow::on_lineEditRead_textChanged()
{
    ui->pushButtonClipboardCopyRead->setEnabled(!ui->lineEditRead->text().isEmpty());
}

void DeviceWindow::on_lineEditWrite_editingFinished()
{
    ui->lineEditWrite->setText(write_.toHexadecimal());  // Required to reformat the hexadecimal string
}

void DeviceWindow::on_lineEditWrite_textChanged()
{
    ui->pushButtonClipboardCopyWrite->setEnabled(!ui->lineEditWrite->text().isEmpty());  // Added in version 4.1 and modified in version 5.0
    write_.fromHexadecimal(ui->lineEditWrite->text());  //This also forces a retrim whenever on_lineEditWrite_editingFinished() is triggered, which is useful case the reformatted hexadecimal string does not fit the line edit box (required in order to follow the WYSIWYG principle)
    int size = write_.vector.size();
    bool enableWrite = size != 0;  // The buttons "Write" and "Write/Read" are enabled if the string is valid, that is, its conversion leads to a non-empty QVector (method changed in version 2.0)
    ui->pushButtonWrite->setEnabled(enableWrite);
    ui->pushButtonWriteRead->setEnabled(enableWrite);
}

void DeviceWindow::on_lineEditWrite_textEdited()
{
    int curPosition = ui->lineEditWrite->cursorPosition();
    ui->lineEditWrite->setText(ui->lineEditWrite->text().toLower());
    ui->lineEditWrite->setCursorPosition(curPosition);
}

// Implemented in version 5.0
void DeviceWindow::on_pushButtonClipboardCopyRead_clicked()
{
    QGuiApplication::clipboard()->setText(ui->lineEditRead->text());
}

// Implemented in version 5.0
void DeviceWindow::on_pushButtonClipboardCopyWrite_clicked()
{
    QGuiApplication::clipboard()->setText(ui->lineEditWrite->text());
}

// Implemented in version 5.0
void DeviceWindow::on_pushButtonClipboardPasteWrite_clicked()
{
    ui->lineEditWrite->setText(QGuiApplication::clipboard()->text().toLower());  // No need to filter the clipboard contents
    ui->lineEditWrite->setFocus();  // This ensures that on_lineEditWrite_editingFinished() is triggered once the user clicks elsewhere
}

// This function no longer reads SPI delays directly (changed in version 3.0)
void DeviceWindow::on_pushButtonConfigureSPIDelays_clicked()
{
    QString channelName = ui->comboBoxChannel->currentText();
    CP2130::SPIDelays spiDelays = spiDelaysMap_[channelName];
    DelaysDialog delaysDialog(this);  // The SPI delays dialog is now a child of the device window, as it should (fixed in version 4.0)
    delaysDialog.setCSToggleCheckBox(spiDelays.cstglen);
    delaysDialog.setPostAssertDelaySpinBoxValue(spiDelays.pstastdly);
    delaysDialog.setPostAssertDelayCheckBox(spiDelays.pstasten);
    delaysDialog.setPreDeassertDelaySpinBoxValue(spiDelays.prdastdly);
    delaysDialog.setPreDeassertDelayCheckBox(spiDelays.prdasten);
    delaysDialog.setInterByteDelaySpinBoxValue(spiDelays.itbytdly);
    delaysDialog.setInterByteDelayCheckBox(spiDelays.itbyten);
    if (delaysDialog.exec() == QDialog::Accepted) {  // If the user clicks "OK", the new delay settings are applied to the current channel
        spiDelays.cstglen = delaysDialog.csToggleCheckBoxIsChecked();
        spiDelays.pstasten = delaysDialog.postAssertDelayCheckBoxIsChecked();
        spiDelays.prdasten = delaysDialog.preDeassertDelayCheckBoxIsChecked();
        spiDelays.itbyten = delaysDialog.interByteDelayCheckBoxIsChecked();
        spiDelays.pstastdly = delaysDialog.postAssertDelaySpinBoxValue();
        spiDelays.prdastdly = delaysDialog.preDeassertDelaySpinBoxValue();
        spiDelays.itbytdly = delaysDialog.interByteDelaySpinBoxValue();
        int errcnt = 0;
        QString errstr;
        cp2130_.configureSPIDelays(static_cast<quint8>(channelName.toUInt()), spiDelays, errcnt, errstr);
        if (opCheck(tr("spi-delays-configuration-op"), errcnt, errstr)) {  // If no errors occur (the string "spi-delays-configuration-op" should be translated to "SPI delays configuration")
            spiDelaysMap_[channelName] = spiDelays;  // Update "spiDelaysMap_" regarding the current channel
        }
    }
}

// This function was expanded in version 3.0, in order to support transfers greater than 4096 bytes
void DeviceWindow::on_pushButtonRead_clicked()
{
    quint8 channel = static_cast<quint8>(ui->comboBoxChannel->currentText().toUInt());
    size_t fragmentSizeLimit = calculateSizeLimit();
    size_t bytesToRead = static_cast<size_t>(ui->spinBoxBytesToRead->value());
    size_t bytesProcessed = 0;
    QProgressDialog spiReadProgress(tr("Performing SPI read..."), tr("Abort"), 0, static_cast<int>(bytesToRead), this);  // Progress dialog implemented in version 3.0
    spiReadProgress.setWindowTitle(tr("SPI Read"));
    spiReadProgress.setWindowModality(Qt::WindowModal);
    spiReadProgress.setMinimumDuration(500);  // The progress dialog should appear only if the operation takes more than 500 ms
    Data read;
    timer_->stop();  // The update timer is now stopped during SPI transfers (fix implemented in version 3.1)
    QElapsedTimer time;
    time.start();
    int errcnt = 0;
    QString errstr;
    cp2130_.selectCS(channel, errcnt, errstr);  // Enable the chip select corresponding to the selected channel, and disable any others
    usleep(100);  // Wait 100 us, in order to prevent possible errors after enabling the chip select (workaround implemented in version 3.1)
    while (bytesProcessed < bytesToRead) {
        if (spiReadProgress.wasCanceled()) {  // If the user clicks "Abort"
            break;  // Abort the SPI read operation
        }
        size_t bytesRemaining = bytesToRead - bytesProcessed;
        size_t fragmentSize = bytesRemaining > fragmentSizeLimit ? fragmentSizeLimit : bytesRemaining;
        QVector<quint8> readFragment = cp2130_.spiRead(static_cast<quint32>(fragmentSize), epin_, epout_, errcnt, errstr);  // Read from the SPI bus
        if (errcnt > 0) {  // In case of error
            spiReadProgress.cancel();  // Important!
            break;  // Abort the SPI read operation
        }
        read.vector += readFragment;  // The returned fragment could be considered valid at this point
        bytesProcessed += fragmentSize;
        spiReadProgress.setValue(static_cast<int>(bytesProcessed));
    }
    usleep(100);  // Wait 100 us, in order to prevent possible errors while disabling the chip select (workaround)
    cp2130_.disableCS(channel, errcnt, errstr);  // Disable the previously enabled chip select
    qint64 elapsedTime = time.elapsed();  // Elapsed time in milliseconds
    timer_->start();  // Restart the timer
    ui->lineEditRead->setText(read.toHexadecimal());  // At least, a partial result should be shown in case of error
    if (errcnt > 0) {  // Update status bar
        labelStatus_->setText(tr("SPI read failed."));
    } else if (spiReadProgress.wasCanceled()){
        labelStatus_->setText(tr("SPI read aborted by the user."));
    } else if (elapsedTime < 1000) {
        labelStatus_->setText(tr("SPI read completed. %1 bytes transferred in %2 ms.").arg(bytesProcessed).arg(elapsedTime));  // The number of transferred bytes is now reported (implemented in version 4.0)
    } else {
        labelStatus_->setText(tr("SPI read completed. %1 bytes transferred in %2 s.").arg(bytesProcessed).arg(locale_.toString(elapsedTime / 1000.0, 'f', 3)));
    }
    opCheck(tr("spi-read-op"), errcnt, errstr);  // The string "spi-read-op" should be translated to "SPI read"
}

// This function was expanded in version 3.0, in order to support transfers greater than 4096 bytes
void DeviceWindow::on_pushButtonWrite_clicked()
{
    quint8 channel = static_cast<quint8>(ui->comboBoxChannel->currentText().toUInt());
    size_t fragmentSizeLimit = calculateSizeLimit();
    size_t bytesToWrite = write_.vector.size();
    size_t bytesProcessed = 0;
    QProgressDialog spiWriteProgress(tr("Performing SPI write..."), tr("Abort"), 0, static_cast<int>(bytesToWrite), this);  // Progress dialog implemented in version 3.0
    spiWriteProgress.setWindowTitle(tr("SPI Write"));
    spiWriteProgress.setWindowModality(Qt::WindowModal);
    spiWriteProgress.setMinimumDuration(500);  // The progress dialog should appear only if the operation takes more than 500 ms
    timer_->stop();  // The update timer is now stopped during SPI transfers (fix implemented in version 3.1)
    QElapsedTimer time;
    time.start();
    int errcnt = 0;
    QString errstr;
    cp2130_.selectCS(channel, errcnt, errstr);  // Enable the chip select corresponding to the selected channel, and disable any others
    usleep(100);  // Wait 100 us, in order to prevent possible errors after enabling the chip select (workaround implemented in version 3.1)
    while (bytesProcessed < bytesToWrite) {
        if (spiWriteProgress.wasCanceled()) {  // If the user clicks "Abort"
            break;  // Abort the SPI write operation
        }
        size_t bytesRemaining = bytesToWrite - bytesProcessed;
        size_t fragmentSize = bytesRemaining > fragmentSizeLimit ? fragmentSizeLimit : bytesRemaining;
        cp2130_.spiWrite(write_.fragment(bytesProcessed, fragmentSize), epout_, errcnt, errstr);  // Write to the SPI bus
        if (errcnt > 0) {  // In case of error
            spiWriteProgress.cancel();  // Important!
            break;  // Abort the SPI write operation
        }
        bytesProcessed += fragmentSize;
        spiWriteProgress.setValue(static_cast<int>(bytesProcessed));
    }
    usleep(100);  // Wait 100 us, in order to prevent possible errors while disabling the chip select (workaround)
    cp2130_.disableCS(channel, errcnt, errstr);  // Disable the previously enabled chip select
    qint64 elapsedTime = time.elapsed();  // Elapsed time in milliseconds
    timer_->start();  // Restart the timer
    ui->lineEditRead->clear();
    if (errcnt > 0) {  // Update status bar
        labelStatus_->setText(tr("SPI write failed."));
    } else if (spiWriteProgress.wasCanceled()){
        labelStatus_->setText(tr("SPI write aborted by the user."));
    } else if (elapsedTime < 1000) {
        labelStatus_->setText(tr("SPI write completed. %1 bytes transferred in %2 ms.").arg(bytesProcessed).arg(elapsedTime));  // The number of transferred bytes is now reported (implemented in version 4.0)
    } else {
        labelStatus_->setText(tr("SPI write completed. %1 bytes transferred in %2 s.").arg(bytesProcessed).arg(locale_.toString(elapsedTime / 1000.0, 'f', 3)));
    }
    opCheck(tr("spi-write-op"), errcnt, errstr);  // The string "spi-write-op" should be translated to "SPI write"
}

// This function was expanded in version 3.0, similar to what was done with on_pushButtonRead_clicked() and on_pushButtonWrite_clicked(), but only to implement a progress dialog
void DeviceWindow::on_pushButtonWriteRead_clicked()
{
    quint8 channel = static_cast<quint8>(ui->comboBoxChannel->currentText().toUInt());
    size_t fragmentSizeLimit = calculateSizeLimit();
    size_t bytesToWriteRead = write_.vector.size();
    size_t bytesProcessed = 0;
    QProgressDialog spiWriteReadProgress(tr("Performing SPI write and read..."), tr("Abort"), 0, static_cast<int>(bytesToWriteRead), this);  // Progress dialog implemented in version 3.0
    spiWriteReadProgress.setWindowTitle(tr("SPI Write/Read"));
    spiWriteReadProgress.setWindowModality(Qt::WindowModal);
    spiWriteReadProgress.setMinimumDuration(500);  // The progress dialog should appear only if the operation takes more than 500 ms
    Data read;
    timer_->stop();  // The update timer is now stopped during SPI transfers (fix implemented in version 3.1)
    QElapsedTimer time;
    time.start();
    int errcnt = 0;
    QString errstr;
    cp2130_.selectCS(channel, errcnt, errstr);  // Enable the chip select corresponding to the selected channel, and disable any others
    usleep(100);  // Wait 100 us, in order to prevent possible errors after enabling the chip select (workaround implemented in version 3.1)
    while (bytesProcessed < bytesToWriteRead) {
        if (spiWriteReadProgress.wasCanceled()) {  // If the user clicks "Abort"
            break;  // Abort the SPI write and read operation
        }
        size_t bytesRemaining = bytesToWriteRead - bytesProcessed;
        size_t fragmentSize = bytesRemaining > fragmentSizeLimit ? fragmentSizeLimit : bytesRemaining;
        QVector<quint8> readFragment = cp2130_.spiWriteRead(write_.fragment(bytesProcessed, fragmentSize), epin_, epout_, errcnt, errstr);  // Write to and read from the SPI bus, simultaneously
        if (errcnt > 0) {  // In case of error
            spiWriteReadProgress.cancel();  // Important!
            break;  // Abort the SPI write and read operation
        }
        read.vector += readFragment;  // The returned fragment could be considered valid at this point
        bytesProcessed += fragmentSize;
        spiWriteReadProgress.setValue(static_cast<int>(bytesProcessed));
    }
    usleep(100);  // Wait 100 us, in order to prevent possible errors while disabling the chip select (workaround)
    cp2130_.disableCS(channel, errcnt, errstr);  // Disable the previously enabled chip select
    qint64 elapsedTime = time.elapsed();  // Elapsed time in milliseconds
    timer_->start();  // Restart the timer
    ui->lineEditRead->setText(read.toHexadecimal());  // At least, a partial result should be shown if an error occurs
    if (errcnt > 0) {  // Update status bar
        labelStatus_->setText(tr("SPI write and read failed."));
    } else if (spiWriteReadProgress.wasCanceled()){
        labelStatus_->setText(tr("SPI write and read aborted by the user."));
    } else if (elapsedTime < 1000) {
        labelStatus_->setText(tr("SPI write and read completed. %1 bytes transferred in %2 ms.").arg(2 * bytesProcessed).arg(elapsedTime));  // The number of transferred bytes is now reported (implemented in version 4.0)
    } else {
        labelStatus_->setText(tr("SPI write and read completed. %1 bytes transferred in %2 s.").arg(2 * bytesProcessed).arg(locale_.toString(elapsedTime / 1000.0, 'f', 3)));
    }
    opCheck(tr("spi-write-read-op"), errcnt, errstr);  // The string "spi-write-read-op" should be translated to "SPI write and read"
}

// Implemented in version 3.0
void DeviceWindow::on_pushButtonZero_clicked()
{
    setEventCounter();
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

// This is the main update routine (expanded in version 3.0)
void DeviceWindow::update()
{
    int errcnt = 0;
    QString errstr;
    quint16 gpios = cp2130_.getGPIOs(errcnt, errstr);
    CP2130::EventCounter evtcntr;
    if (pinConfig_.gpio5 == CP2130::PCEVTCNTRRE || pinConfig_.gpio5 == CP2130::PCEVTCNTRFE || pinConfig_.gpio5 == CP2130::PCEVTCNTRNP || pinConfig_.gpio5 == CP2130::PCEVTCNTRPP) {
        evtcntr = cp2130_.getEventCounter(errcnt, errstr);
    }
    if (opCheck(tr("update-op"), errcnt, errstr)) {  // If no errors occur (the string "update-op" should be translated to "Update")
        updateView(gpios, evtcntr);  // Update values
    }
}

// This is executed when the clipboard contents change (implemented in version 5.0)
void DeviceWindow::updatePushButtonClipboardPasteWrite()
{
    ui->pushButtonClipboardPasteWrite->setEnabled(isClipboardTextValid());
}

// Calculates the optimal fragment size limit based on the parameters of the currently selected channel (implemented in version 3.1, to replace evaluateSizeLimit())
size_t DeviceWindow::calculateSizeLimit()
{
    QString channelName = ui->comboBoxChannel->currentText();
    float preDeassertDelay = spiDelaysMap_[channelName].prdasten == true ? spiDelaysMap_[channelName].prdastdly / 100.0 : 0;  // Pre-deassert delay in milliseconds
    float postAssertDelay = spiDelaysMap_[channelName].pstasten == true ? spiDelaysMap_[channelName].pstastdly / 100.0 : 0;  // Post-assert delay in milliseconds
    float interByteDelay = spiDelaysMap_[channelName].itbyten == true ? spiDelaysMap_[channelName].itbytdly / 100.0 : 0;  // Inter-byte delay in milliseconds
    float timePerByte = std::pow(2, ui->comboBoxFrequency->currentIndex()) / 1500;  // Time duration for each byte, in milliseconds
    size_t sizeLimit = static_cast<size_t>((TIME_LIMIT + interByteDelay - preDeassertDelay - postAssertDelay) / (timePerByte + interByteDelay));
    return sizeLimit == 0 ? 1 : sizeLimit;  // The size limit cannot be zero
}

// Configures the SPI mode for the currently selected channel
void DeviceWindow::configureSPIMode()
{
    QString channelName = ui->comboBoxChannel->currentText();
    CP2130::SPIMode spiMode;
    spiMode.csmode = ui->comboBoxCSPinMode->currentIndex() != 0;
    spiMode.cfrq = static_cast<quint8>(ui->comboBoxFrequency->currentIndex());  // Corrected in version 3.0
    spiMode.cpol = ui->spinBoxCPOL->value() != 0;
    spiMode.cpha = ui->spinBoxCPHA->value() != 0;
    int errcnt = 0;
    QString errstr;
    cp2130_.configureSPIMode(static_cast<quint8>(channelName.toUInt()), spiMode, errcnt, errstr);
    if (opCheck(tr("spi-mode-configuration-op"), errcnt, errstr)) {  // If no errors occur (the string "spi-mode-configuration-op" should be translated to "SPI mode configuration")
        spiModeMap_[channelName] = spiMode;  // Update "spiModeMap_" regarding the current channel
    }
}

// Partially disables device window
void DeviceWindow::disableView()
{
    ui->actionInformation->setEnabled(false);
    ui->actionGPIOPinFunctions->setEnabled(false);
    ui->actionSetClockDivider->setEnabled(false);
    ui->actionReset->setEnabled(false);
    ui->actionClose->setText(tr("&Close Window"));  // Implemented in version 3.0, to hint the user that the device is effectively closed and only its window remains open
    ui->centralWidget->setEnabled(false);
    ui->checkBoxGPIO0->setStyleSheet("");
    ui->checkBoxGPIO1->setStyleSheet("");
    ui->checkBoxGPIO2->setStyleSheet("");
    ui->checkBoxGPIO3->setStyleSheet("");
    ui->checkBoxGPIO4->setStyleSheet("");
    ui->checkBoxGPIO5->setStyleSheet("");
    ui->checkBoxGPIO6->setStyleSheet("");
    ui->checkBoxGPIO7->setStyleSheet("");
    ui->checkBoxGPIO8->setStyleSheet("");
    ui->checkBoxGPIO9->setStyleSheet("");
    ui->checkBoxGPIO10->setStyleSheet("");
    ui->lcdNumberCount->setStyleSheet("");
    ui->statusBar->setEnabled(false);
    viewEnabled_ = false;
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

// Initializes the event counter controls (implemented in version 3.0)
void DeviceWindow::initializeEventCounterControls()
{
    ui->groupBoxEventCounter->setEnabled(pinConfig_.gpio5 == CP2130::PCEVTCNTRRE || pinConfig_.gpio5 == CP2130::PCEVTCNTRFE || pinConfig_.gpio5 == CP2130::PCEVTCNTRNP || pinConfig_.gpio5 == CP2130::PCEVTCNTRPP);
}

// Initializes the GPIO controls
void DeviceWindow::initializeGPIOControls()
{
    ui->checkBoxGPIO0->setStyleSheet(pinConfig_.gpio0 == CP2130::PCCS ? "color: darkred;" : "");
    ui->checkBoxGPIO0->setEnabled(pinConfig_.gpio0 == CP2130::PCOUTOD || pinConfig_.gpio0 == CP2130::PCOUTPP || pinConfig_.gpio0 == CP2130::PCCS);
    ui->checkBoxGPIO1->setStyleSheet(pinConfig_.gpio1 == CP2130::PCCS ? "color: darkred;" : "");
    ui->checkBoxGPIO1->setEnabled(pinConfig_.gpio1 == CP2130::PCOUTOD || pinConfig_.gpio1 == CP2130::PCOUTPP || pinConfig_.gpio1 == CP2130::PCCS);
    ui->checkBoxGPIO2->setStyleSheet(pinConfig_.gpio2 == CP2130::PCCS ? "color: darkred;" : "");
    ui->checkBoxGPIO2->setEnabled(pinConfig_.gpio2 == CP2130::PCOUTOD || pinConfig_.gpio2 == CP2130::PCOUTPP || pinConfig_.gpio2 == CP2130::PCCS);
    ui->checkBoxGPIO3->setStyleSheet(pinConfig_.gpio3 == CP2130::PCCS ? "color: darkred;" : "");
    ui->checkBoxGPIO3->setEnabled(pinConfig_.gpio3 == CP2130::PCOUTOD || pinConfig_.gpio3 == CP2130::PCOUTPP || pinConfig_.gpio3 == CP2130::PCCS);
    ui->checkBoxGPIO4->setStyleSheet(pinConfig_.gpio4 == CP2130::PCCS ? "color: darkred;" : "");
    ui->checkBoxGPIO4->setEnabled(pinConfig_.gpio4 == CP2130::PCOUTOD || pinConfig_.gpio4 == CP2130::PCOUTPP || pinConfig_.gpio4 == CP2130::PCCS);
    ui->checkBoxGPIO5->setStyleSheet(pinConfig_.gpio5 == CP2130::PCCS ? "color: darkred;" : "");
    ui->checkBoxGPIO5->setEnabled(pinConfig_.gpio5 == CP2130::PCOUTOD || pinConfig_.gpio5 == CP2130::PCOUTPP || pinConfig_.gpio5 == CP2130::PCCS);
    ui->checkBoxGPIO6->setStyleSheet(pinConfig_.gpio6 == CP2130::PCCS ? "color: darkred;" : "");
    ui->checkBoxGPIO6->setEnabled(pinConfig_.gpio6 == CP2130::PCOUTOD || pinConfig_.gpio6 == CP2130::PCOUTPP || pinConfig_.gpio6 == CP2130::PCCS);
    ui->checkBoxGPIO7->setStyleSheet(pinConfig_.gpio7 == CP2130::PCCS ? "color: darkred;" : "");
    ui->checkBoxGPIO7->setEnabled(pinConfig_.gpio7 == CP2130::PCOUTOD || pinConfig_.gpio7 == CP2130::PCOUTPP || pinConfig_.gpio7 == CP2130::PCCS);
    ui->checkBoxGPIO8->setStyleSheet(pinConfig_.gpio8 == CP2130::PCCS ? "color: darkred;" : "");
    ui->checkBoxGPIO8->setEnabled(pinConfig_.gpio8 == CP2130::PCOUTOD || pinConfig_.gpio8 == CP2130::PCOUTPP || pinConfig_.gpio8 == CP2130::PCCS);
    ui->checkBoxGPIO9->setStyleSheet(pinConfig_.gpio9 == CP2130::PCCS ? "color: darkred;" : "");
    ui->checkBoxGPIO9->setEnabled(pinConfig_.gpio9 == CP2130::PCOUTOD || pinConfig_.gpio9 == CP2130::PCOUTPP || pinConfig_.gpio9 == CP2130::PCCS);
    ui->checkBoxGPIO10->setStyleSheet(pinConfig_.gpio10 == CP2130::PCCS ? "color: darkred;" : "");
    ui->checkBoxGPIO10->setEnabled(pinConfig_.gpio10 == CP2130::PCOUTOD || pinConfig_.gpio10 == CP2130::PCOUTPP || pinConfig_.gpio10 == CP2130::PCCS);
    // Since version 2.0, the enabled boxes not only correspond to GPIO pins that are configured as outputs, but to CS pins as well
}

// Initializes the "Set Clock Divider" action (implemented in version 3.0)
void DeviceWindow::initializeSetClockDividerAction()
{
    ui->actionSetClockDivider->setEnabled(pinConfig_.gpio4 == CP2130::PCCLKOUT);
}

// Initializes the SPI controls
void DeviceWindow::initializeSPIControls()
{
    ui->comboBoxChannel->clear();  // The combo box is always cleared (revised in version 3.0)
    if (spiModeMap_.size() != 0) {  // In order for the SPI controls (including transfers) to be enabled, at least one pin should be configured to work as a chip select
        QList<QString> keys = spiModeMap_.keys();
        for (const QString &key : qAsConst(keys)) {  // Fixed in version 4.1, in order to clear Clazy warnings
            ui->comboBoxChannel->addItem(key);
        }
        displaySPIMode();
    }
    ui->groupBoxSPIConfiguration->setEnabled(spiModeMap_.size() != 0);  // It may be desirable to either enable or disable both group boxes, just in case the device configuration changes (revised in version 3.0)
    ui->groupBoxSPITransfers->setEnabled(spiModeMap_.size() != 0);
    ui->pushButtonClipboardPasteWrite->setEnabled(isClipboardTextValid());  // Added in version 4.1 and modified in version 5.0
}

// This is the routine that is used to initialize (or reinitialize) the device window
void DeviceWindow::initializeView()
{
    initializeSetClockDividerAction();
    initializeGPIOControls();
    initializeEventCounterControls();
    initializeSPIControls();
    viewEnabled_ = true;
}

// Returns true if the clipboard text contains a valid hexadecimal string (implemented in version 4.1)
bool DeviceWindow::isClipboardTextValid()
{
    QString clipboardText = QGuiApplication::clipboard()->text();
    int pos = 0;
    return QRegExpValidator(QRegExp("[A-Fa-f\\d\\s]+")).validate(clipboardText, pos) == QValidator::Acceptable;
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
                cp2130_.close();  // Ensure that the device is freed, even if the previous device reset is not effective (cp2130_.reset() also frees the device interface, as an effect of re-enumeration)
                // It is essential that cp2130_.close() is called, since some important checks rely on cp2130_.isOpen() to retrieve a proper status
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
        spiDelaysMap_["0"] = cp2130_.getSPIDelays(0, errcnt, errstr);  // Implemented in version 3.0
    }
    if (pinConfig_.gpio1 == CP2130::PCCS) {
        spiModeMap_["1"] = cp2130_.getSPIMode(1, errcnt, errstr);
        spiDelaysMap_["1"] = cp2130_.getSPIDelays(1, errcnt, errstr);  // Implemented in version 3.0
    }
    if (pinConfig_.gpio2 == CP2130::PCCS) {
        spiModeMap_["2"] = cp2130_.getSPIMode(2, errcnt, errstr);
        spiDelaysMap_["2"] = cp2130_.getSPIDelays(2, errcnt, errstr);  // Implemented in version 3.0
    }
    if (pinConfig_.gpio3 == CP2130::PCCS) {
        spiModeMap_["3"] = cp2130_.getSPIMode(3, errcnt, errstr);
        spiDelaysMap_["3"] = cp2130_.getSPIDelays(3, errcnt, errstr);  // Implemented in version 3.0
    }
    if (pinConfig_.gpio4 == CP2130::PCCS) {
        spiModeMap_["4"] = cp2130_.getSPIMode(4, errcnt, errstr);
        spiDelaysMap_["4"] = cp2130_.getSPIDelays(4, errcnt, errstr);  // Implemented in version 3.0
    }
    if (pinConfig_.gpio5 == CP2130::PCCS) {
        spiModeMap_["5"] = cp2130_.getSPIMode(5, errcnt, errstr);
        spiDelaysMap_["5"] = cp2130_.getSPIDelays(5, errcnt, errstr);  // Implemented in version 3.0
    }
    if (pinConfig_.gpio6 == CP2130::PCCS) {
        spiModeMap_["6"] = cp2130_.getSPIMode(6, errcnt, errstr);
        spiDelaysMap_["6"] = cp2130_.getSPIDelays(6, errcnt, errstr);  // Implemented in version 3.0
    }
    if (pinConfig_.gpio7 == CP2130::PCCS) {
        spiModeMap_["7"] = cp2130_.getSPIMode(7, errcnt, errstr);
        spiDelaysMap_["7"] = cp2130_.getSPIDelays(7, errcnt, errstr);  // Implemented in version 3.0
    }
    if (pinConfig_.gpio8 == CP2130::PCCS) {
        spiModeMap_["8"] = cp2130_.getSPIMode(8, errcnt, errstr);
        spiDelaysMap_["8"] = cp2130_.getSPIDelays(8, errcnt, errstr);  // Implemented in version 3.0
    }
    if (pinConfig_.gpio9 == CP2130::PCCS) {
        spiModeMap_["9"] = cp2130_.getSPIMode(9, errcnt, errstr);
        spiDelaysMap_["9"] = cp2130_.getSPIDelays(9, errcnt, errstr);  // Implemented in version 3.0
    }
    if (pinConfig_.gpio10 == CP2130::PCCS) {
        spiModeMap_["10"] = cp2130_.getSPIMode(10, errcnt, errstr);
        spiDelaysMap_["10"] = cp2130_.getSPIDelays(10, errcnt, errstr);  // Implemented in version 3.0
    }
    // Note that both "spiModeMap_" and "spiDelaysMap_" are populated in relation to pins that are configured as chip select pins
    epin_ = cp2130_.getEndpointInAddr(errcnt, errstr);  // Implemented in version 3.0
    epout_ = cp2130_.getEndpointOutAddr(errcnt, errstr);  // Implemented in version 3.0
    if (errcnt > 0) {
        this->hide();  // Hide the window, if applicable, to let the user know that the device is practically closed (implemented in version 3.0)
        if (cp2130_.disconnected()) {
            cp2130_.close();
            QMessageBox::critical(this, tr("Error"), tr("Device disconnected.\n\nPlease reconnect it and try again."));
        } else {
            cp2130_.reset(errcnt, errstr);  // Try to reset the device for sanity purposes, but don't check if it was successful
            cp2130_.close();
            errstr.chop(1);  // Remove the last character, which is always a newline
            QMessageBox::critical(this, tr("Error"), tr("Read configuration operation returned the following error(s):\n– %1\n\nPlease try accessing the device again.", "", errcnt).arg(errstr.replace("\n", "\n– ")));
        }
        this->deleteLater();  // This is a severe error that requires the window to be closed, because the state of the device is not known, and therefore it is not safe to proceed
    }
}

// Resets the device
void DeviceWindow::resetDevice()
{
    timer_->stop();  // Stop the update timer momentarily, in order to avoid recurrent errors if the device gets disconnected during a reset, or other unexpected behavior
    int errcnt = 0;
    QString errstr;
    cp2130_.reset(errcnt, errstr);
    opCheck(tr("reset-op"), errcnt, errstr);  // The string "reset-op" should be translated to "Reset"
    if (cp2130_.isOpen()) {  // If opCheck() passes, thus, not closing the device
        cp2130_.close();  // Important! - This should be done always, even if the previous reset operation shows an error, because an error doesn't mean that a device reset was not effected
        int err;
        for (int i = 0; i < ENUM_RETRIES; ++i) {  // Verify enumeration according to the number of times set by "ENUM_RETRIES" [10]
            QThread::msleep(500);  // Wait 500 ms each time
            err = cp2130_.open(vid_, pid_, serialstr_);
            if (err != CP2130::ERROR_NOT_FOUND) {  // Retry only if the device was not found yet (as it may take some time to enumerate)
                break;
            }
        }
        if (err == CP2130::SUCCESS) {  // Device was successfully reopened
            spiModeMap_.clear();  // Implemented in version 3.0 (this is not required, unless the device configuration somehow changes)
            spiDelaysMap_.clear();
            readConfiguration();  // Reread device configuration
            erracc_ = 0;  // Zero the error count accumulator, since a new session gets started once the reset is done
            ui->lineEditWrite->clear();
            ui->lineEditRead->clear();
            ui->spinBoxBytesToRead->setValue(0);
            labelStatus_->clear();
            initializeView();  // Reinitialize device window
            timer_->start();  // Restart the timer
        } else {  // Failed to reopen device
            disableView();
            if (err == CP2130::ERROR_INIT) {  // Failed to initialize libusb
                QMessageBox::critical(this, tr("Critical Error"), tr("Could not reinitialize libusb.\n\nThis is a critical error and execution will be aborted."));
                exit(EXIT_FAILURE);  // This error is critical because libusb failed to initialize
            } else if (err == CP2130::ERROR_NOT_FOUND) {  // Failed to find device
                QMessageBox::critical(this, tr("Error"), tr("Device disconnected.\n\nPlease reconnect it and try again."));
            } else if (err == CP2130::ERROR_BUSY) {  // Failed to claim interface
                QMessageBox::critical(this, tr("Error"), tr("Device ceased to be available.\n\nPlease verify that the device is not in use by another application."));
            }
            // Since version 3.0, the window will no longer close automatically if the device fails to reopen (this will give the user a chance to see the last state)
        }
    }
}

// Sets (and zeroes) the event counter according to the currently selected trigger mode (implemented in version 3.0)
void DeviceWindow::setEventCounter()
{
    CP2130::EventCounter evtcntr;
    evtcntr.overflow = false;
    evtcntr.mode = static_cast<quint8>(ui->comboBoxTriggerMode->currentIndex() + CP2130::PCEVTCNTRRE);
    evtcntr.value = 0;  // This effectively zeroes the event count
    int errcnt = 0;
    QString errstr;
    cp2130_.setEventCounter(evtcntr, errcnt, errstr);
    opCheck(tr("event-counter-setting-op"), errcnt, errstr);  // The string "event-counter-setting-op" should be translated to "Event counter setting"
}

// Updates the view (expanded in version 3.0)
void DeviceWindow::updateView(quint16 gpios, CP2130::EventCounter evtcntr)
{
    ui->checkBoxGPIO0->setChecked((CP2130::BMGPIO0 & gpios) != 0x0000);
    ui->checkBoxGPIO1->setChecked((CP2130::BMGPIO1 & gpios) != 0x0000);
    ui->checkBoxGPIO2->setChecked((CP2130::BMGPIO2 & gpios) != 0x0000);
    ui->checkBoxGPIO3->setChecked((CP2130::BMGPIO3 & gpios) != 0x0000);
    ui->checkBoxGPIO4->setChecked((CP2130::BMGPIO4 & gpios) != 0x0000);
    ui->checkBoxGPIO5->setChecked((CP2130::BMGPIO5 & gpios) != 0x0000);
    ui->checkBoxGPIO6->setChecked((CP2130::BMGPIO6 & gpios) != 0x0000);
    ui->checkBoxGPIO7->setChecked((CP2130::BMGPIO7 & gpios) != 0x0000);
    ui->checkBoxGPIO8->setChecked((CP2130::BMGPIO8 & gpios) != 0x0000);
    ui->checkBoxGPIO9->setChecked((CP2130::BMGPIO9 & gpios) != 0x0000);
    ui->checkBoxGPIO10->setChecked((CP2130::BMGPIO10 & gpios) != 0x0000);
    if (pinConfig_.gpio5 == CP2130::PCEVTCNTRRE || pinConfig_.gpio5 == CP2130::PCEVTCNTRFE || pinConfig_.gpio5 == CP2130::PCEVTCNTRNP || pinConfig_.gpio5 == CP2130::PCEVTCNTRPP) {
        ui->comboBoxTriggerMode->setCurrentIndex(evtcntr.mode - CP2130::PCEVTCNTRRE);
        ui->lcdNumberCount->setStyleSheet(evtcntr.overflow ? "color: darkred;" : "");
        ui->lcdNumberCount->display(evtcntr.value);
    }
}
