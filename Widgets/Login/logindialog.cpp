#include "logindialog.h"
#include "ui_logindialog.h"

#include <QMessageBox>
#include <QFile>
#include <QTextStream>

#include "Dao/basedao.h"
#include "widget.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    //设置关闭窗体就删除
    setAttribute(Qt::WA_DeleteOnClose);

    //由于在ui中已经设置了背景自动填充，不需要再次手动设置
    setPalette(QPalette(QColor(255,255,255)));  //填充为白色

    ui->loginBtn->setFlat(true);
    ui->loginBtn->setPalette(QPalette(QColor(255,255,255)));


    baseDao = new BaseDao(this);
    connect(ui->loginBtn,&QPushButton::clicked,this,&LoginDialog::onLoginBtnClicked);
}

LoginDialog::~LoginDialog()
{
    delete ui;
    delete baseDao;
    baseDao = nullptr;
}

/**
 * @brief LoginDialog::onLoginBtnClicked
 * 功能：登录按钮点击事件，用于登录,同时导出操作员信息
 */
void LoginDialog::onLoginBtnClicked()
{
    //获取页面数据
    QString name = ui->nameEdit->text();
    QString password = ui->passwordEdit->text();

    if(name.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this,"数据错误","请检查用户名和密码");
        return;
    }

    QList<QVariantMap> staffMapList = baseDao->selectByInfo("staff",
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


    QList<QVariantMap> map = baseDao->selectByInfo("operator",
                                                   QStringList() << "staff_id",
                                                   QVariantList() << s_staffID);


    if(map.size() == 1)
    {
        int s_operatorID = map[0].value("operator_id").toInt();
        QString s_password = map[0].value("s_password").toString();

        if(s_password.compare(password) == 0)
        {
            QFile file("../operatorInfo.txt");

            if(file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            {
                QTextStream stream(&file);

                stream << s_operatorID << " " << name ;

                file.close();
            }

            this->accept();
        }
        else
        {
            QMessageBox::warning(this,"数据错误","请检查用户名和密码");
            return;
        }
    }
    else
    {
        QMessageBox::warning(this,"数据错误2","数据错误2");
        return;
    }
}
