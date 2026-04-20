#include "login_register_mismatch.h"
#include "ui_login_register_mismatch.h"

login_register_mismatch::login_register_mismatch(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login_register_mismatch)
{
    ui->setupUi(this);
}

login_register_mismatch::~login_register_mismatch()
{
    delete ui;
}

void login_register_mismatch::on_mismatch_confirm_clicked()
{
    reject();
}

