#include "questiondialog.h"
#include "ui_questiondialog.h"

QuestionDialog::QuestionDialog(QString message, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QuestionDialog)
{
    ui->setupUi(this);
    ui->questionTitle->setText(message);
}

QuestionDialog::~QuestionDialog()
{
    delete ui;
}

void QuestionDialog::on_declineButton_clicked()
{
    this->close();
}

void QuestionDialog::on_acceptButton_clicked()
{
    this->accept();
}
