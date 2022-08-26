/* CP2130 Commander - Version 3.1 for Debian Linux
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
#include <QRegExpValidator>
#include <QStringList>
#include "aboutdialog.h"
#include "cp2130.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEditVID->setValidator(new QRegExpValidator(QRegExp("[A-Fa-f\\d]+"), this));
    ui->lineEditPID->setValidator(new QRegExpValidator(QRegExp("[A-Fa-f\\d]+"), this));
    ui->lineEditVID->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog aboutDialog;
    aboutDialog.exec();
}

void MainWindow::on_comboBoxDevices_currentIndexChanged(int index)
{
    ui->pushButtonOpen->setEnabled(index != 0);
}

void MainWindow::on_lineEditPID_textEdited()
{
    int curPosition = ui->lineEditPID->cursorPosition();
    ui->lineEditPID->setText(ui->lineEditPID->text().toLower());
    ui->lineEditPID->setCursorPosition(curPosition);
    validateInput();
}

void MainWindow::on_lineEditVID_textEdited()
{
    int curPosition = ui->lineEditVID->cursorPosition();
    ui->lineEditVID->setText(ui->lineEditVID->text().toLower());
    ui->lineEditVID->setCursorPosition(curPosition);
    validateInput();
}

// This function was expanded in version 3.0, so that when the user tries to open a device that is already open in the current instance of the application, it will bring the corresponding window to the top
void MainWindow::on_pushButtonOpen_clicked()
{
    QString serialstr = ui->comboBoxDevices->currentText();  // Extract the serial number from the chosen item in the combo box
    QString usbidstr = QString("%1%2%3").arg(vid_, 4, 16, QChar('0')).arg(pid_, 4, 16, QChar('0')).arg(serialstr);  // Unique identifier string for the USB device
    DeviceWindow *devWindow;
    if (devWindowMap_.contains(usbidstr) && !devWindowMap_[usbidstr].isNull() && (devWindow = devWindowMap_[usbidstr].data())->isViewEnabled()) {  // If the device is already mapped, and its window is open but not disabled
        devWindow->showNormal();  // Required if the corresponding device window is minimized
        devWindow->activateWindow();  // Set focus on the device window (window is raised and selected)
    } else {
        devWindow = new DeviceWindow(this);  // Create a new window that will close when its parent window closes
        devWindow->setAttribute(Qt::WA_DeleteOnClose);  // This will not only free the allocated memory once the window is closed, but will also automatically call the destructor of the respective device, which in turn closes it
        devWindow->openDevice(vid_, pid_, serialstr);  // Access the selected device and prepare its view
        devWindow->show();  // Then open the corresponding window
        devWindowMap_[usbidstr] = devWindow;  // Map the device window, via a QPointer, to the unique identifier string of the device
    }
}

void MainWindow::on_pushButtonRefresh_clicked()
{
    refresh();
}

// Refreshes the combo box list
void MainWindow::refresh()
{
    int errcnt = 0;
    QString errstr;
    QStringList comboBoxList = {tr("Select device...")};
    comboBoxList += CP2130::listDevices(vid_, pid_, errcnt, errstr);
    if (errcnt > 0) {
        QMessageBox::critical(this, tr("Critical Error"), tr("%1\nThis is a critical error and execution will be aborted.").arg(errstr));
        exit(EXIT_FAILURE);  // This error is critical because either libusb failed to initialize, or could not retrieve a list of devices
    } else {
        ui->comboBoxDevices->clear();
        ui->comboBoxDevices->addItems(comboBoxList);
    }
}

// Checks for valid user input, enabling or disabling the combo box and the "Refresh" button, accordingly
void MainWindow::validateInput()
{
    QString vidstr = ui->lineEditVID->text();
    QString pidstr = ui->lineEditPID->text();
    if (vidstr.size() == 4 && pidstr.size() == 4) {
        vid_ = static_cast<quint16>(vidstr.toUInt(nullptr, 16));
        pid_ = static_cast<quint16>(pidstr.toUInt(nullptr, 16));
        refresh();  // This has the "side effect" of disabling the "Open" button - Note that this is the intended behavior!
        ui->comboBoxDevices->setEnabled(true);
        ui->pushButtonRefresh->setEnabled(true);
    } else {
        ui->comboBoxDevices->setCurrentIndex(0);  // This also disables the "Open" button
        ui->comboBoxDevices->setEnabled(false);
        ui->pushButtonRefresh->setEnabled(false);
    }
}
