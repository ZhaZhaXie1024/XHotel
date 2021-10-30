#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

class BaseDao;

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

//登录按钮点击事件
private slots:
    void onLoginBtnClicked();

private:
    Ui::LoginDialog *ui;
    BaseDao *baseDao;
};

#endif // LOGINDIALOG_H
