#ifndef FUNCTIONSDIALOG_H
#define FUNCTIONSDIALOG_H

#include <QDialog>

namespace Ui {
class FunctionsDialog;
}

class FunctionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FunctionsDialog(QWidget *parent = nullptr);
    ~FunctionsDialog();

private:
    Ui::FunctionsDialog *ui;
};

#endif // FUNCTIONSDIALOG_H
