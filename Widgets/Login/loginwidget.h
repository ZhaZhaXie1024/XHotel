#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

class BaseDao;
class Widget;

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

    void initWidget();


private slots:
    //登录按钮点击事件
    void onLoginBtnClicked();

protected:
    void showEvent(QShowEvent *) override;

private:
    Ui::LoginWidget *ui;
    BaseDao *m_baseDao;
    Widget *m_widget;
};

#endif // LOGINWIDGET_H
