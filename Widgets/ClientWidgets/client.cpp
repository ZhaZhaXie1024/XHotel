#include "client.h"
#include "ui_client.h"

#include <QSqlQueryModel>

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>

#include "Service/ClientService/clientservice.h"
#include "Widgets/ClientWidgets/clientselectdialog.h"

Client::Client(QWidget *parent,int openFlag) :
    QWidget(parent),
    ui(new Ui::Client),
    m_openFlag(openFlag)
{
    ui->setupUi(this);

    init();
}

Client::~Client()
{
    delete ui;
}

void Client::init()
{
    if(m_openFlag != 1)
    {
        ui->btn_insert->hide();
        ui->btn_updata->hide();
        ui->btn_delete->hide();
    }

    m_service = new ClientService(this);
    m_model = new QSqlQueryModel(this);
    m_dialog = new ClientSelectDialog(this);

    m_index = 0;
    m_showNum = 15;
    m_sql = m_service->select();

    select();

    initModelStyle();

    initConnect();
}

/**
 * @brief Client::initModelStyle
 * 初始化Model样式
 */
void Client::initModelStyle()
{
    m_model->setHeaderData(1,Qt::Horizontal,"姓名");
    m_model->setHeaderData(2,Qt::Horizontal,"性别");
    m_model->setHeaderData(3,Qt::Horizontal,"生日");
    m_model->setHeaderData(4,Qt::Horizontal,"电话");
    m_model->setHeaderData(5,Qt::Horizontal,"证件号");

    ui->tableView->setModel(m_model);
    ui->tableView->hideColumn(0);
}

/**
 * @brief Client::initConnect
 * 初始化全局连接
 */
void Client::initConnect()
{
    //点击查询按钮，打开查询对话框
    connect(ui->btn_select,&QPushButton::clicked,this,&Client::onSelectBtnClicked);

}

/**
 * @brief Client::select
 * 查询
 */
void Client::select()
{
    //设置limit
    m_service->setLimit(m_index,m_showNum);
    //更新数据
    m_model->setQuery(m_sql,m_service->getSqlDatabase());
    //更新总页数
    ui->label_pageCount->setText(QString::number(m_service->pageCount()));
}

/**
 * @brief Client::onSelectBtnClicked
 * 查询按钮被按下，打开查询对话框
 */
void Client::onSelectBtnClicked()
{
    m_selectDialog->show();
}

/**
 * @brief Client::createIUDialog
 * @param title
 * 创建新增/修改对话框
 */
void Client::createIUDialog(QString title)
{
    if(m_dialog == nullptr)
    {
        m_dialog = new QDialog(this,Qt::Dialog|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);

        m_dialog->setWindowTitle(title);

        m_diaMLayout = new QVBoxLayout(m_dialog);

        //装载两个垂直的布局
        m_diaElmLayout = new QHBoxLayout;

        //第一个垂直布局：存放Label
        m_diaLLayout = new QVBoxLayout;
        m_label_name = new QLabel("名称：",m_dialog);
        m_label_sex = new QLabel("性别：",m_dialog);
        m_label_time = new QLabel("出生日期：",m_dialog);
        m_label_phone = new QLabel("手机号码：",m_dialog);
        m_label_certificate = new QLabel("身份证：",m_dialog);
        //全部右对齐
        m_label_name->setAlignment(Qt::AlignHCenter|Qt::AlignRight);
        m_label_sex->setAlignment(Qt::AlignHCenter|Qt::AlignRight);
        m_label_time->setAlignment(Qt::AlignHCenter|Qt::AlignRight);
        m_label_phone->setAlignment(Qt::AlignHCenter|Qt::AlignRight);
        m_label_certificate->setAlignment(Qt::AlignHCenter|Qt::AlignRight);

        m_diaLLayout->addWidget(m_label_name);
        m_diaLLayout->addWidget(m_label_sex);
        m_diaLLayout->addWidget(m_label_time);
        m_diaLLayout->addWidget(m_label_phone);
        m_diaLLayout->addWidget(m_label_certificate);

        //第二个垂直布局：存放控件
        m_diaEdiLayout = new QVBoxLayout;
        m_edit_name = new QLineEdit(m_dialog);
        m_com_sex = new QComboBox(m_dialog);
        m_de_time = new QDateEdit(m_dialog);
        m_edit_phone = new QLineEdit(m_dialog);
        m_edit_certificate = new QLineEdit(m_dialog);

        //为sex添加数据
        m_com_sex->addItem("男","男");
        m_com_sex->addItem("女","女");
        //dateEdit设置日期弹出层
        m_de_time->setCalendarPopup(true);

        m_diaEdiLayout->addWidget(m_edit_name);
        m_diaEdiLayout->addWidget(m_com_sex);
        m_diaEdiLayout->addWidget(m_de_time);
        m_diaEdiLayout->addWidget(m_edit_phone);
        m_diaEdiLayout->addWidget(m_edit_certificate);

        //两个布局添加到 m_diaElmLayout
        m_diaElmLayout->addLayout(m_diaLLayout);
        m_diaElmLayout->addLayout(m_diaEdiLayout);

        //装载按钮的布局
        m_diaBLayout = new QHBoxLayout;
        m_btn_insert = new QPushButton("新增",m_dialog);
        m_btn_update = new QPushButton("修改",m_dialog);
        m_btn_clear = new QPushButton("清空",m_dialog);
        m_btn_cancel = new QPushButton("取消",m_dialog);

        m_diaBLayout->addStretch();
        m_diaBLayout->addWidget(m_btn_insert);
        m_diaBLayout->addWidget(m_btn_update);
        m_diaBLayout->addWidget(m_btn_clear);
        m_diaBLayout->addWidget(m_btn_cancel);

        //添加到总布局
        m_diaMLayout->addLayout(m_diaElmLayout);
        m_diaMLayout->addLayout(m_diaBLayout);

    }

    if(title.compare("新增"))
    {
        m_btn_update->hide();
        m_btn_insert->show();
    }
    else
    {
        m_btn_update->show();
        m_btn_insert->hide();
    }

    m_dialog->show();
}
