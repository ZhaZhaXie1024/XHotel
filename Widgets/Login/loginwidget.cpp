#include "loginwidget.h"
#include "ui_loginwidget.h"

#include <QMessageBox>
#include <QFile>
#include <QTextStream>

#include "Dao/basedao.h"
#include "widget.h"

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    initWidget();

    m_baseDao = new BaseDao(this);
    m_widget = nullptr;

    //信号和槽
    connect(ui->loginBtn,&QPushButton::clicked,this,&LoginWidget::onLoginBtnClicked);
}

LoginWidget::~LoginWidget()
{
    delete ui;

    if(m_widget!=nullptr)
    {
        delete m_widget;
        m_widget = nullptr;
    }
}

/**
 * @brief LoginWidget::initWidget
 * 功能：初始化widget
 */
void LoginWidget::initWidget()
{
    //修改背景颜色
    setAutoFillBackground(true);                //允许填充背景
    setPalette(QPalette(QColor(255,255,255)));  //填充为白色

    //设置窗口名称
    setWindowTitle("九忆酒店管理系统登录");
    //设置窗口LOGO
    setWindowIcon(QIcon(":/images/logo.png"));

}

/**
 * @brief LoginWidget::onLoginBtnClicked
 * 功能：登录按钮点击事件，用于登录,同时导出操作员信息
 */
void LoginWidget::onLoginBtnClicked()
{
    //获取页面数据
    QString name = ui->nameEdit->text();
    QString password = ui->passwordEdit->text();

    //
    if(name.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this,"数据错误","请检查用户名和密码");
        return;
    }

    QList<QVariantMap> staffMapList = m_baseDao->selectByInfo("staff",
                                                   QStringList() << "name",
                                                   QVariantList() << name);

    int s_staffID = 0;

    if(staffMapList.size() == 1)
    {
        s_staffID = staffMapList[0].value("staff_id").toInt();
    }
    else
    {
        QMessageBox::warning(this,"数据错误1","数据错误1");
        return;
    }


    QList<QVariantMap> map = m_baseDao->selectByInfo("operator",
                                                   QStringList() << "staff_id",
                                                   QVariantList() << s_staffID);


    if(map.size() == 1)
    {
        int s_operatorID = map[0].value("operator_id").toInt();
        QString s_password = map[0].value("s_password").toString();

        //s_password.compare(password) == 0

        if(s_password.compare(password) == 0)
        {
            QFile file("../operatorInfo.txt");

            if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            {
                QTextStream stream(&file);
                stream << s_operatorID << " " << name ;
                file.close();
            }

            m_widget = new Widget(s_operatorID,name);
            m_widget->show();
            close();
        }
        else
        {
            QMessageBox::warning(this,"数据错误","请检查用户名和密码");
            return;
        }
    }
    else
    {
        QMessageBox::warning(this,"数据错误","数据错误2,数据库发生错误");
        return;
    }
}

/**
 * @brief LoginWidget::showEvent
 * @param event
 * 功能：当第一次页面打开显示时读取操作员信息
 */
void LoginWidget::showEvent(QShowEvent *)
{
    QFile file("../operatorInfo.txt");

    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);

        QString str = stream.readAll();

        QStringList list = str.split(" ");

        QString name = list[1];

        file.close();

        if(!name.isEmpty())
        {
            ui->nameEdit->setText(name);
        }
    }
}

