/* CP2130 Commander - Version 2.0 for Debian Linux
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
#include <QStringList>
#include "reconfiguredialog.h"
#include "ui_reconfiguredialog.h"

// Definitions
const QStringList COMBOBOXSETUP = {QObject::tr("Input"), QObject::tr("Open-drain output"), QObject::tr("Push-pull output")};

ReconfigureDialog::ReconfigureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReconfigureDialog)
{
    ui->setupUi(this);
}

ReconfigureDialog::~ReconfigureDialog()
{
    delete ui;
}

// Sets up "comboBoxGPIO0"
void ReconfigureDialog::setupGPIO0ComboBox(int index)
{
    ui->comboBoxGPIO0->setEnabled(true);
    ui->comboBoxGPIO0->clear();
    ui->comboBoxGPIO0->addItems(COMBOBOXSETUP);
    ui->comboBoxGPIO0->setCurrentIndex(index);
}

// Sets up "comboBoxGPIO1"
void ReconfigureDialog::setupGPIO1ComboBox(int index)
{
    ui->comboBoxGPIO1->setEnabled(true);
    ui->comboBoxGPIO1->clear();
    ui->comboBoxGPIO1->addItems(COMBOBOXSETUP);
    ui->comboBoxGPIO1->setCurrentIndex(index);
}

// Sets up "comboBoxGPIO2"
void ReconfigureDialog::setupGPIO2ComboBox(int index)
{
    ui->comboBoxGPIO2->setEnabled(true);
    ui->comboBoxGPIO2->clear();
    ui->comboBoxGPIO2->addItems(COMBOBOXSETUP);
    ui->comboBoxGPIO2->setCurrentIndex(index);
}

// Sets up "comboBoxGPIO3"
void ReconfigureDialog::setupGPIO3ComboBox(int index)
{
    ui->comboBoxGPIO3->setEnabled(true);
    ui->comboBoxGPIO3->clear();
    ui->comboBoxGPIO3->addItems(COMBOBOXSETUP);
    ui->comboBoxGPIO3->setCurrentIndex(index);
}

// Sets up "comboBoxGPIO4"
void ReconfigureDialog::setupGPIO4ComboBox(int index)
{
    ui->comboBoxGPIO4->setEnabled(true);
    ui->comboBoxGPIO4->clear();
    ui->comboBoxGPIO4->addItems(COMBOBOXSETUP);
    ui->comboBoxGPIO4->setCurrentIndex(index);
}

// Sets up "comboBoxGPIO5"
void ReconfigureDialog::setupGPIO5ComboBox(int index)
{
    ui->comboBoxGPIO5->setEnabled(true);
    ui->comboBoxGPIO5->clear();
    ui->comboBoxGPIO5->addItems(COMBOBOXSETUP);
    ui->comboBoxGPIO5->setCurrentIndex(index);
}

// Sets up "comboBoxGPIO6"
void ReconfigureDialog::setupGPIO6ComboBox(int index)
{
    ui->comboBoxGPIO6->setEnabled(true);
    ui->comboBoxGPIO6->clear();
    ui->comboBoxGPIO6->addItems(COMBOBOXSETUP);
    ui->comboBoxGPIO6->setCurrentIndex(index);
}

// Sets up "comboBoxGPIO7"
void ReconfigureDialog::setupGPIO7ComboBox(int index)
{
    ui->comboBoxGPIO7->setEnabled(true);
    ui->comboBoxGPIO7->clear();
    ui->comboBoxGPIO7->addItems(COMBOBOXSETUP);
    ui->comboBoxGPIO7->setCurrentIndex(index);
}

// Sets up "comboBoxGPIO8"
void ReconfigureDialog::setupGPIO8ComboBox(int index)
{
    ui->comboBoxGPIO8->setEnabled(true);
    ui->comboBoxGPIO8->clear();
    ui->comboBoxGPIO8->addItems(COMBOBOXSETUP);
    ui->comboBoxGPIO8->setCurrentIndex(index);
}

// Sets up "comboBoxGPIO9"
void ReconfigureDialog::setupGPIO9ComboBox(int index)
{
    ui->comboBoxGPIO9->setEnabled(true);
    ui->comboBoxGPIO9->clear();
    ui->comboBoxGPIO9->addItems(COMBOBOXSETUP);
    ui->comboBoxGPIO9->setCurrentIndex(index);
}

// Sets up "comboBoxGPIO10"
void ReconfigureDialog::setupGPIO10ComboBox(int index)
{
    ui->comboBoxGPIO10->setEnabled(true);
    ui->comboBoxGPIO10->clear();
    ui->comboBoxGPIO10->addItems(COMBOBOXSETUP);
    ui->comboBoxGPIO10->setCurrentIndex(index);
}
