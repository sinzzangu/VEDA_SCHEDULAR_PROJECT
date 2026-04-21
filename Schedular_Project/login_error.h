#ifndef LOGIN_ERROR_H
#define LOGIN_ERROR_H

#include <QDialog>

namespace Ui {
class login_error;
}

class login_error : public QDialog
{
    Q_OBJECT

public:
    explicit login_error(QWidget *parent = nullptr);
    ~login_error();

    void login_dialog_changeText(int check_value);

private slots:
    void on_pushButton_confirm_clicked();

private:
    Ui::login_error *ui;
};

#endif // LOGIN_ERROR_H
