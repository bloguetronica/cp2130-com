#ifndef DELAYSDIALOG_H
#define DELAYSDIALOG_H

#include <QDialog>

namespace Ui {
class DelaysDialog;
}

class DelaysDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DelaysDialog(QWidget *parent = nullptr);
    ~DelaysDialog();

private:
    Ui::DelaysDialog *ui;
};

#endif // DELAYSDIALOG_H
