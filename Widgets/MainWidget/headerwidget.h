#ifndef HEADERWIDGET_H
#define HEADERWIDGET_H

#include <QWidget>

/**
 * @brief RETCODE_RESTART
 *
 * define a retcode：773 = 'r'+'e'+'s'+'t'+'a'+'r'+'t' = restart
 *
 * 即为 重启 的意思
 */
static const int RETCODE_RESTART = 773;

namespace Ui {
class HeaderWidget;
}

class HeaderWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HeaderWidget(QWidget *parent = nullptr);
    HeaderWidget(QString operatorName,QWidget *parent = nullptr);
    ~HeaderWidget();

    //初始化
    void init();

    //设置用户名称
    void setUserName(QString name = "用户名");

public slots:
    //注销登录按钮
    void onLoginOutBtn();

private:
    Ui::HeaderWidget *ui;
};

#endif // HEADERWIDGET_H
