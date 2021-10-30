#include "sysroomtype.h"
#include "ui_sysroomtype.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QMessageBox>

#include "Service/SystemService/RoomManageService/roomtypeservice.h"
#include "Entitys/SystemRoomManageEntitys.h"

SysRoomType::SysRoomType(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysRoomType)
{
    ui->setupUi(this);
    init();
}

SysRoomType::~SysRoomType()
{
    delete ui;
}

/**
 * @brief SysRoomType::init
 * 初始化
 */
void SysRoomType::init()
{
    m_service = new RoomTypeService(this);
    m_dialog = nullptr;
    m_room_type_id = 0;

    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "id" << "代码" << "描述" << "床位数" << "房价");

    loadTableData();

    //新增（对话框）
    connect(ui->btn_insert,&QPushButton::clicked,this,&SysRoomType::onInsertBtnOnClicked);
    //修改（对话框）
    connect(ui->btn_update,&QPushButton::clicked,this,&SysRoomType::onUpdateBtnClicked);
    //删除
    connect(ui->btn_delete,&QPushButton::clicked,this,&SysRoomType::onDelBtnOnClicked);
    //刷新
    connect(ui->btn_refresh,&QPushButton::clicked,this,&SysRoomType::loadTableData);
}

/**
 * @brief SysRoomType::loadTableData
 * 加载表格数据/刷新
 */
void SysRoomType::loadTableData()
{
    //清除表格数据
    ui->tableWidget->clearContents();

    QList<RoomTypeInfo> data = m_service->selectRoomType();

    ui->tableWidget->setRowCount(data.size());

    for(int i = 0; i < data.size(); i++)
    {
        RoomTypeInfo info = data.at(i);

        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(info.room_type_id));
        item0->setData(Qt::UserRole,info.room_type_id);
        item0->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,0,item0);

        QTableWidgetItem *item1 = new QTableWidgetItem(info.code);
        item1->setData(Qt::UserRole,info.room_type_id);
        item1->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,1,item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(info.m_describe);
        item2->setData(Qt::UserRole,info.room_type_id);
        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,2,item2);

        QTableWidgetItem *item3 = new QTableWidgetItem(QString::number(info.bed_number));
        item3->setData(Qt::UserRole,info.room_type_id);
        item3->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,3,item3);

        QTableWidgetItem *item4 = new QTableWidgetItem(QString::number(info.room_rate));
        item4->setData(Qt::UserRole,info.room_type_id);
        item4->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,4,item4);
    }

    ui->tableWidget->hideColumn(0);
}

/**
 * @brief SysRoomType::createDialog
 * @param name
 * 创建对话框
 */
void SysRoomType::createDialog(QString name)
{
    onDiaCanBtnClicked();

    m_dialog = new QDialog(this,Qt::Dialog|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);

    m_mainLayout = new QVBoxLayout(m_dialog);
    m_contentLayout = new QHBoxLayout;
    m_labelLayout = new QVBoxLayout;
    m_editLayout = new QVBoxLayout;
    m_btnLayout = new QHBoxLayout;


    m_dmLabel = new QLabel(tr("代码："));
    m_dmLabel->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    m_desLabel = new QLabel(tr("描述："));
    m_desLabel->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    m_bedNumLabel = new QLabel(tr("床数："));
    m_bedNumLabel->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    m_rateLable = new QLabel(tr("房价："));
    m_rateLable->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

    m_labelLayout->addWidget(m_dmLabel);
    m_labelLayout->addWidget(m_desLabel);
    m_labelLayout->addWidget(m_bedNumLabel);
    m_labelLayout->addWidget(m_rateLable);


    m_dmEdit = new QLineEdit;
    m_desEdit = new QLineEdit;
    m_bedNumEdit = new QLineEdit;
    m_rateEdit = new QLineEdit;

    m_editLayout->addWidget(m_dmEdit);
    m_editLayout->addWidget(m_desEdit);
    m_editLayout->addWidget(m_bedNumEdit);
    m_editLayout->addWidget(m_rateEdit);

    m_contentLayout->addLayout(m_labelLayout);
    m_contentLayout->addLayout(m_editLayout);

    m_btnLayout->addStretch();
    if(name.compare("修改") == 0)
    {
        m_updateOKBtn = new QPushButton(tr("修改"));
        connect(m_updateOKBtn,&QPushButton::clicked,this,&SysRoomType::onUpDiaUpdateClicked);
        m_btnLayout->addWidget(m_updateOKBtn);
    }
    else
    {
        m_insertOKBtn = new QPushButton(tr("新增"));
        connect(m_insertOKBtn,&QPushButton::clicked,this,&SysRoomType::onInsDiaInsBtnClicked);
        m_btnLayout->addWidget(m_insertOKBtn);
    }

    m_cancel = new QPushButton(tr("取消"));
    connect(m_cancel,&QPushButton::clicked,this,&SysRoomType::onDiaCanBtnClicked);
    m_btnLayout->addWidget(m_cancel);

    m_mainLayout->addLayout(m_contentLayout);
    m_mainLayout->addLayout(m_btnLayout);

    m_dialog->show();
}

/**
 * @brief SysRoomType::onInsertBtnOnClicked
 * 新建按钮被按下
 */
void SysRoomType::onInsertBtnOnClicked()
{
    createDialog();
}

/**
 * @brief SysRoomType::onInsDiaInsBtnClicked
 * 对话框中，新增按钮被按下(真正的保存)
 */
void SysRoomType::onInsDiaInsBtnClicked()
{
    //获取数据
    QString code = m_dmEdit->text().trimmed();
    QString describe = m_desEdit->text().trimmed();
    int bedNum = m_bedNumEdit->text().toInt();
    double reta = m_rateEdit->text().toDouble();

    //判断
    if(code.isEmpty() || describe.isEmpty() || bedNum <= 0 || reta <= 0.00)
    {
        QMessageBox::information(m_dialog,"新增提示","您输入的数据不完整，请检查数据后重新点击新增！");
        return;
    }

    if(m_service->insertRoomType(code,describe,bedNum,reta))
    {
        QMessageBox::information(m_dialog,"新增提示","成功新增一条数据");
        loadTableData();
    }
    else
    {
        QString text = m_service->getLastError().trimmed();
        if(text.isEmpty())
        {
            text = "新增失败，请刷新页面后重试！";
        }
        QMessageBox::warning(m_dialog,"新增提示",text);
    }

    onDiaCanBtnClicked();
}

/**
 * @brief SysRoomType::onUpdateBtnClicked
 * 修改按钮被按下
 */
void SysRoomType::onUpdateBtnClicked()
{
    QModelIndex index = ui->tableWidget->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::warning(this,"修改提示","请选中一行数据后再点击修改");
        return;
    }

    m_room_type_id = 0;

    m_room_type_id = index.sibling(index.row(),0).data(Qt::UserRole).toInt();


    if(m_room_type_id <= 0)
    {
        QMessageBox::warning(this,"修改提示","数据错误，请刷新页面后重试！");
        return;
    }

    createDialog("修改");

    //回填数据
    m_dmEdit->setText(index.sibling(index.row(),1).data().toString().trimmed());
    m_desEdit->setText(index.sibling(index.row(),2).data().toString().trimmed());
    m_bedNumEdit->setText(index.sibling(index.row(),3).data().toString().trimmed());
    m_rateEdit->setText(index.sibling(index.row(),4).data().toString().trimmed());
}

/**
 * @brief SysRoomType::onUpDiaUpdateClicked
 * 对话框中，修改按钮被按下
 */
void SysRoomType::onUpDiaUpdateClicked()
{
    //获取数据
    if(m_room_type_id <= 0)
    {
        QMessageBox::warning(m_dialog,"修改提示","数据错误，请刷新页面后重试！");
        createDialog("修改");
        return;
    }

    QString code = m_dmEdit->text().trimmed();
    QString describe = m_desEdit->text().trimmed();
    int bedNum = m_bedNumEdit->text().trimmed().toInt();
    double rate = m_rateEdit->text().trimmed().toDouble();

    if(code.isEmpty() || describe.isEmpty() || bedNum <= 0  || rate <= 0.00 || m_room_type_id <= 0)
    {
        QMessageBox::warning(m_dialog,"修改提示","请填写完整！");
        return;
    }

    if(m_service->updateRoomType(m_room_type_id,code,describe,bedNum,rate))
    {
        QMessageBox::information(m_dialog,"修改提示","成功修改一条数据");
        loadTableData();
    }
    else
    {
        QString text = m_service->getLastError().trimmed();
        if(text.isEmpty())
        {
            text = "修改失败，请刷新页面后重试！";
        }
        QMessageBox::warning(m_dialog,"修改提示",text);
    }

    onDiaCanBtnClicked();
}

/**
 * @brief SysRoomType::onDiaCanBtnClicked
 * 对话框中，取消按钮被按下
 */
void SysRoomType::onDiaCanBtnClicked()
{
    if(m_dialog != nullptr)
    {
        m_dialog->close();
        delete m_dialog;
        m_dialog = nullptr;
    }
}

/**
 * @brief SysRoomType::onDelBtnOnClicked
 * 删除按钮被按下
 */
void SysRoomType::onDelBtnOnClicked()
{
    QModelIndex index = ui->tableWidget->currentIndex();
    if(!index.isValid())
    {
        QMessageBox::warning(this,"删除提示","请选中一行数据后再点击删除！");
        return;
    }

    m_room_type_id = 0;
    m_room_type_id = index.sibling(index.row(),0).data(Qt::UserRole).toInt();

    if(m_room_type_id <= 0)
    {
        QMessageBox::warning(this,"删除提示","数据错误，请刷新页面后重试！");
        return;
    }

    int btn = QMessageBox::question(this,"删除提示","确定要删除该数据吗？");

    if(btn == QMessageBox::Yes)
    {
        if(m_service->deleteRoomType(m_room_type_id))
        {
            QMessageBox::information(this,"删除提示","成功删除一条数据");
            loadTableData();
        }
        else
        {
            QString text = m_service->getLastError().trimmed();
            if(text.isEmpty())
            {
                text = "删除失败，请刷新页面后重试！";
            }
            QMessageBox::warning(m_dialog,"删除提示",text);
        }
    }
}
