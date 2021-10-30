#include "sysroomstate.h"
#include "ui_sysroomstate.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QMessageBox>

#include "Service/SystemService/RoomManageService/roomstateservice.h"
#include "Entitys/SystemRoomManageEntitys.h"

SysRoomState::SysRoomState(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysRoomState)
{
    ui->setupUi(this);
    init();
}

SysRoomState::~SysRoomState()
{
    delete ui;
}

/**
 * @brief SysRoomState::init
 * 初始化
 */
void SysRoomState::init()
{
    m_service = new RoomStateService(this);
    m_dialog = nullptr;
    m_room_state_id = 0;

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "id" << "代码" << "描述");

    loadTableData();

    //新增（对话框）
    connect(ui->btn_insert,&QPushButton::clicked,this,&SysRoomState::onInsertBtnOnClicked);
    //修改（对话框）
    connect(ui->btn_update,&QPushButton::clicked,this,&SysRoomState::onUpdateBtnClicked);
    //删除
    connect(ui->btn_delete,&QPushButton::clicked,this,&SysRoomState::onDelBtnOnClicked);
    //刷新
    connect(ui->btn_refresh,&QPushButton::clicked,this,&SysRoomState::loadTableData);
}

/**
 * @brief SysRoomState::loadTableData
 * 加载表格数据/刷新
 */
void SysRoomState::loadTableData()
{
    //清除表格数据
    ui->tableWidget->clearContents();

    QList<RoomStateInfo> data = m_service->selectRoomState();

    ui->tableWidget->setRowCount(data.size());

    for(int i = 0; i < data.size(); i++)
    {
        RoomStateInfo info = data.at(i);

        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(info.room_state_id));
        item0->setData(Qt::UserRole,info.room_state_id);
        item0->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,0,item0);

        QTableWidgetItem *item1 = new QTableWidgetItem(info.code);
        item1->setData(Qt::UserRole,info.room_state_id);
        item1->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,1,item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(info.m_describe);
        item2->setData(Qt::UserRole,info.room_state_id);
        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,2,item2);
    }

    ui->tableWidget->hideColumn(0);
}

/**
 * @brief SysRoomState::createDialog
 * @param name
 * 创建对话框
 */
void SysRoomState::createDialog(QString name)
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

    m_labelLayout->addWidget(m_dmLabel);
    m_labelLayout->addWidget(m_desLabel);


    m_dmEdit = new QLineEdit;
    m_desEdit = new QLineEdit;

    m_editLayout->addWidget(m_dmEdit);
    m_editLayout->addWidget(m_desEdit);

    m_contentLayout->addLayout(m_labelLayout);
    m_contentLayout->addLayout(m_editLayout);

    m_btnLayout->addStretch();
    if(name.compare("修改") == 0)
    {
        m_updateOKBtn = new QPushButton(tr("修改"));
        connect(m_updateOKBtn,&QPushButton::clicked,this,&SysRoomState::onUpDiaUpdateClicked);
        m_btnLayout->addWidget(m_updateOKBtn);
    }
    else
    {
        m_insertOKBtn = new QPushButton(tr("新增"));
        connect(m_insertOKBtn,&QPushButton::clicked,this,&SysRoomState::onInsDiaInsBtnClicked);
        m_btnLayout->addWidget(m_insertOKBtn);
    }

    m_cancel = new QPushButton(tr("取消"));
    connect(m_cancel,&QPushButton::clicked,this,&SysRoomState::onDiaCanBtnClicked);
    m_btnLayout->addWidget(m_cancel);

    m_mainLayout->addLayout(m_contentLayout);
    m_mainLayout->addLayout(m_btnLayout);

    m_dialog->show();
}

/**
 * @brief SysRoomState::onInsertBtnOnClicked
 * 新建按钮被按下
 */
void SysRoomState::onInsertBtnOnClicked()
{
    createDialog();
}

/**
 * @brief SysRoomState::onInsDiaInsBtnClicked
 * 对话框中，新增按钮被按下(真正的保存)
 */
void SysRoomState::onInsDiaInsBtnClicked()
{
    //获取数据
    QString code = m_dmEdit->text().trimmed();
    QString describe = m_desEdit->text().trimmed();

    //判断
    if(code.isEmpty() || describe.isEmpty())
    {
        QMessageBox::information(m_dialog,"新增提示","您输入的数据不完整，请检查数据后重新点击新增！");
        return;
    }

    if(m_service->insertRoomState(code,describe))
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
 * @brief SysRoomState::onUpdateBtnClicked
 * 修改按钮被按下
 */
void SysRoomState::onUpdateBtnClicked()
{
    QModelIndex index = ui->tableWidget->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::warning(this,"修改提示","请选中一行数据后再点击修改");
        return;
    }

    m_room_state_id = 0;

    m_room_state_id = index.sibling(index.row(),0).data(Qt::UserRole).toInt();


    if(m_room_state_id <= 0)
    {
        QMessageBox::warning(this,"修改提示","数据错误，请刷新页面后重试！");
        return;
    }

    createDialog("修改");

    //回填数据
    m_dmEdit->setText(index.sibling(index.row(),1).data().toString().trimmed());
    m_desEdit->setText(index.sibling(index.row(),2).data().toString().trimmed());
}

/**
 * @brief SysRoomState::onUpDiaUpdateClicked
 * 对话框中，修改按钮被按下
 */
void SysRoomState::onUpDiaUpdateClicked()
{
    //获取数据
    if(m_room_state_id <= 0)
    {
        QMessageBox::warning(m_dialog,"修改提示","数据错误，请刷新页面后重试！");
        createDialog("修改");
        return;
    }

    QString code = m_dmEdit->text().trimmed();
    QString describe = m_desEdit->text().trimmed();

    if(code.isEmpty() || describe.isEmpty() || m_room_state_id <= 0)
    {
        QMessageBox::warning(m_dialog,"修改提示","请填写完整！");
        return;
    }

    if(m_service->updateRoomState(m_room_state_id,code,describe))
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
 * @brief SysRoomState::onDiaCanBtnClicked
 * 对话框中，取消按钮被按下
 */
void SysRoomState::onDiaCanBtnClicked()
{
    if(m_dialog != nullptr)
    {
        m_dialog->close();
        delete m_dialog;
        m_dialog = nullptr;
    }
}

/**
 * @brief SysRoomState::onDelBtnOnClicked
 * 删除
 */
void SysRoomState::onDelBtnOnClicked()
{
    QModelIndex index = ui->tableWidget->currentIndex();
    if(!index.isValid())
    {
        QMessageBox::warning(this,"删除提示","请选中一行数据后再点击删除！");
        return;
    }

    m_room_state_id = 0;
    m_room_state_id = index.sibling(index.row(),0).data(Qt::UserRole).toInt();

    if(m_room_state_id <= 0)
    {
        QMessageBox::warning(this,"删除提示","数据错误，请刷新页面后重试！");
        return;
    }

    int btn = QMessageBox::question(this,"删除提示","确定要删除该数据吗？");

    if(btn == QMessageBox::Yes)
    {
        if(m_service->deleteRoomState(m_room_state_id))
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
