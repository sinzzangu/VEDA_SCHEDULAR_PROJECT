#include "add_schedule_dialog.h"
#include "ui_add_schedule_dialog.h"

add_schedule_dialog::add_schedule_dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::add_schedule_dialog)
{
    ui->setupUi(this);
}

add_schedule_dialog::~add_schedule_dialog()
{
    delete ui;
}
