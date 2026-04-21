#include "login_register_success.h"
#include "ui_login_register_success.h"

login_register_success::login_register_success(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login_register_success)
{
    ui->setupUi(this);
}

login_register_success::~login_register_success()
{
    delete ui;
}

void login_register_success::on_mismatch_confirm_clicked()
{
    reject();
}

