#include "headerwidget.h"
#include "ui_headerwidget.h"

#include <QMessageBox>
#include <QFile>

HeaderWidget::HeaderWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HeaderWidget)
{
    init();
}

HeaderWidget::HeaderWidget(QString operatorName, QWidget *parent)
    :QWidget(parent),
    ui(new Ui::HeaderWidget)
{
    init();
    setUserName(operatorName);
}

HeaderWidget::~HeaderWidget()
{
    delete ui;
}

void HeaderWidget::init()
{
    ui->setupUi(this);
    connect(ui->btn_logout,&QPushButton::clicked,this,&HeaderWidget::onLoginOutBtn);

    //修改背景颜色
    setPalette(QPalette(QColor(255,255,255)));  //填充为白色
}

/**
 * @brief HeaderWidget::setUserName
 * @param name  用户名称
 * 功能：设置操作员名称
 */
void HeaderWidget::setUserName(QString name)
{
    ui->btn_userName->setText(name);
}

/**
 * @brief HeaderWidget::onLoginOutBtn
 * 功能：注销登录
 */
void HeaderWidget::onLoginOutBtn()
{
    int button = QMessageBox::warning(this,"注销登录","是否注销登录？",
                                      QMessageBox::Ok,QMessageBox::Cancel);

    if(button == QMessageBox::Ok)
    {
        //注销
        QFile file("../operatorInfo.txt");
        file.remove();
        file.close();

        qApp->exit(RETCODE_RESTART);
    }
}
