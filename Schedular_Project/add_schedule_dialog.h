#ifndef ADD_SCHEDULE_DIALOG_H
#define ADD_SCHEDULE_DIALOG_H

#include <QDialog>

namespace Ui {
class add_schedule_dialog;
}

class add_schedule_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit add_schedule_dialog(QWidget *parent = nullptr);
    ~add_schedule_dialog();

private:
    Ui::add_schedule_dialog *ui;
};

#endif // ADD_SCHEDULE_DIALOG_H
