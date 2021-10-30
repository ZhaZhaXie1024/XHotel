#include "sysstaffposition.h"
#include "ui_sysstaffposition.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QDebug>

#include "Service/SystemService/StaffManageService/sysstaffposservice.h"
#include "Entitys/SystemSimpleModelEntity.h"

SysStaffPosition::SysStaffPosition(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysStaffPosition),
    m_posId(0)
{
    ui->setupUi(this);
    init();
}

SysStaffPosition::~SysStaffPosition()
{
    delete ui;
}


/**
 * @brief SysStaffPosition::init
 * 功能：初始化
 */
void SysStaffPosition::init()
{
    m_service = new SysStaffPosService(this);
    m_dialog = nullptr;

    //初始化表格样式
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "职位代码" << "职位描述");

    //初始化表格数据
    loadTableData();

    //初始化连接
    connect(ui->btn_insert,&QPushButton::clicked,this,&SysStaffPosition::onInsertBtnOnClicked);
    connect(ui->btn_update,&QPushButton::clicked,this,&SysStaffPosition::onUpdateBtnClicked);
    connect(ui->btn_refresh,&QPushButton::clicked,this,&SysStaffPosition::loadTableData);
    connect(ui->btn_delete,&QPushButton::clicked,this,&SysStaffPosition::onDelBtnOnClicked);
}

/**
 * @brief SysStaffPosition::loadTableData
 * 功能：加载/刷新表格数据
 */
void SysStaffPosition::loadTableData()
{
    //清除表格数据
    ui->tableWidget->clearContents();

    //表格数据
    QList<StaffPosition> data = m_service->selectStaffPos();

    ui->tableWidget->setRowCount(data.size());

    int row = 0;

    for(int i = 0; i < data.size(); i++)
    {
        StaffPosition one = data.at(i);
        QTableWidgetItem *item1 = new QTableWidgetItem(one.code);
        item1->setData(Qt::UserRole,one.staff_position_id);
        item1->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row,0,item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(one.m_describe);
        item2->setData(Qt::UserRole,one.staff_position_id);
        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row,1,item2);

        row++;
    }
}

/**
 * @brief SysStaffPosition::createDialog
 * @param name
 * 功能：创建对话框，对话框的复用
 */
void SysStaffPosition::createDialog(QString name)
{
    if(m_dialog != nullptr)
    {
        onDiaCanBtnClicked();
    }

    //Qt::WindowCloseButtonHint , 把 ？去掉
    m_dialog = new QDialog(this,Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    //设置为模态框
    m_dialog->setModal(true);
    //设置对话框标题
    m_dialog->setWindowTitle(name);

    m_mainLayout = new QVBoxLayout(m_dialog);

    m_DMLabel = new QLabel(tr("代码："),m_dialog);
    m_DMEdit = new QLineEdit(m_dialog);

    m_DescribeLabel = new QLabel(tr("描述："),m_dialog);
    m_DescribeEdit = new QLineEdit(m_dialog);

    m_DMEdit->setClearButtonEnabled(true);
    m_DescribeEdit->setClearButtonEnabled(true);

    m_mainLayout->addWidget(m_DMLabel);
    m_mainLayout->addWidget(m_DMEdit);
    m_mainLayout->addWidget(m_DescribeLabel);
    m_mainLayout->addWidget(m_DescribeEdit);

    m_btnLayout = new QHBoxLayout;

    m_btnLayout->addStretch();

    if(name.compare("新增") == 0)
    {
        m_insertOKBtn = new QPushButton(tr("新增"),m_dialog);
        connect(m_insertOKBtn,&QPushButton::clicked,this,&SysStaffPosition::onInsDiaInsBtnClicked);
        m_btnLayout->addWidget(m_insertOKBtn);
    }
    else if(name.compare("修改") == 0)
    {
        m_updateOKBtn = new QPushButton(tr("修改"),m_dialog);
        connect(m_updateOKBtn,&QPushButton::clicked,this,&SysStaffPosition::onUpDiaUpdateClicked);
        m_btnLayout->addWidget(m_updateOKBtn);
    }

    m_Cancel = new QPushButton(tr("取消"),m_dialog);
    connect(m_Cancel,&QPushButton::clicked,this,&SysStaffPosition::onDiaCanBtnClicked);

    m_btnLayout->addWidget(m_Cancel);

    m_mainLayout->addLayout(m_btnLayout);

    m_dialog->show();
}

/**
 * @brief SysStaffPosition::onInsertBtnClicked
 * 功能：新增（对话框）
 */
void SysStaffPosition::onInsertBtnOnClicked()
{
    createDialog();
}

/**
 * @brief SysStaffPosition::onUpdateBtnClicked
 * 功能：修改（对话框）
 */
void SysStaffPosition::onUpdateBtnClicked()
{
    QModelIndex index = ui->tableWidget->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::information(this,"通知",
                                 "请选中一行后再点击修改");
        return;
    }

    m_posId = index.data(Qt::UserRole).toInt();

    QString code = index.sibling(index.row(),0).data().toString();
    QString describe = index.sibling(index.row(),1).data().toString();

    if(m_posId > 0)
    {
        createDialog("修改");

        m_DMEdit->setText(code);
        m_DescribeEdit->setText(describe);
    }
}

/**
 * @brief SysStaffPosition::onInsDiaInsBtnClicked
 * 功能：新增，对话框中，新增按钮被按下
 */
void SysStaffPosition::onInsDiaInsBtnClicked()
{
    QString code = m_DMEdit->text();
    QString describe = m_DescribeEdit->text();

    if(code.isEmpty() || describe.isEmpty())
    {
        QMessageBox::warning(m_dialog,"警告",
                             "数据不能为空，请填写完整");
        return;
    }

    bool isOk = m_service->insertData(code,describe);

    if(isOk)
    {
        QMessageBox::information(m_dialog,"新增成功",
                                 "新增数据成功了！");
        loadTableData();
    }
    else
    {
        QString text = m_service->getLastError();

        qDebug() << text;

        if(text.trimmed().isEmpty())
        {
            text = "新增数据失败，数据重复，不能新增重复的数据哦";
        }

        QMessageBox::critical(m_dialog,"错误",text);
    }

    //关闭
    onDiaCanBtnClicked();
}

/**
 * @brief SysStaffPosition::onUpDiaUpdateClicked
 * 功能：修改，对话框中，修改按钮被按下
 */
void SysStaffPosition::onUpDiaUpdateClicked()
{
    QString code = m_DMEdit->text();
    QString describe = m_DescribeEdit->text();

    if(m_posId <= 0 || code.isEmpty() || describe.isEmpty())
    {
        QMessageBox::warning(m_dialog,"警告",
                             "数据不能为空，请填写完整");
        return;
    }

    bool isOk = m_service->updateData(m_posId,code,describe);

    if(isOk)
    {
        QMessageBox::information(m_dialog,"修改成功",
                                 "修改数据成功了！");
        loadTableData();
    }
    else
    {
        QString text = m_service->getLastError();

        qDebug() << text;

        if(text.trimmed().isEmpty())
        {
            text = "新增数据失败，数据重复，不能新增重复的数据哦";
        }

        QMessageBox::critical(m_dialog,"错误",text);
    }

    //关闭
    onDiaCanBtnClicked();
}

/**
 * @brief SysStaffPosition::onDiaCanBtnClicked
 * 功能：取消，对话框中，取消按钮被按下
 */
void SysStaffPosition::onDiaCanBtnClicked()
{
    if(m_dialog != nullptr)
    {
        m_dialog->close();
    }
    delete m_btnLayout;
    delete m_mainLayout;
    delete m_dialog;
    m_btnLayout = nullptr;
    m_mainLayout = nullptr;
    m_dialog = nullptr;
}

/**
 * @brief SysStaffPosition::onDelBtnOnClicked
 * 功能：删除
 */
void SysStaffPosition::onDelBtnOnClicked()
{
    QModelIndex index = ui->tableWidget->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::information(this,"通知",
                                 "请选中一行后再点击删除");
        return;
    }

    int btn = QMessageBox::question(m_dialog,"提示","确定要删除吗？");

    if(btn == QMessageBox::Yes)
    {

        m_posId = index.data(Qt::UserRole).toInt();

        if(m_posId > 0)
        {
            bool isOk = m_service->deleteData(m_posId);

            if(!isOk)
            {
                QString text = m_service->getLastError();
                if(text.trimmed().isEmpty())
                {
                    text = "删除失败，请稍后重试";
                }
                QMessageBox::critical(m_dialog,"错误",text);
            }
            else
            {
                loadTableData();
            }
        }
        else
        {
            QMessageBox::information(this,"通知",
                                     "数据错误");
            return;
        }
    }
}

