#include "functionsdialog.h"
#include "ui_functionsdialog.h"

FunctionsDialog::FunctionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FunctionsDialog)
{
    ui->setupUi(this);
}

FunctionsDialog::~FunctionsDialog()
{
    delete ui;
}
