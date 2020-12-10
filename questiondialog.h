#ifndef QUESTIONDIALOG_H
#define QUESTIONDIALOG_H

#include <QDialog>

namespace Ui {
class QuestionDialog;
}

class QuestionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QuestionDialog(QString message, QWidget *parent = nullptr);
    ~QuestionDialog();

private slots:
    void on_declineButton_clicked();

    void on_acceptButton_clicked();

private:
    Ui::QuestionDialog *ui;
};

#endif // QUESTIONDIALOG_H
