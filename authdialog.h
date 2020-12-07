#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class AuthDialog;
}

class AuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AuthDialog(QWidget *parent = nullptr);
    ~AuthDialog();

private slots:
    void on_authButton_clicked();

    void on_signUpCheckBox_stateChanged(int arg1);

    void on_AuthDialog_finished(int result);

private:
    Ui::AuthDialog *ui;
    bool is_signup=false;
    QSettings settings;
};

#endif // AUTHDIALOG_H
