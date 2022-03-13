#include "delaysdialog.h"
#include "ui_delaysdialog.h"

DelaysDialog::DelaysDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DelaysDialog)
{
    ui->setupUi(this);
}

DelaysDialog::~DelaysDialog()
{
    delete ui;
}
