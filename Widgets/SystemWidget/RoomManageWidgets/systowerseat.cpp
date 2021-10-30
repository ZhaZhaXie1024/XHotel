#include "systowerseat.h"
#include "ui_systowerseat.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QMessageBox>

#include "Service/SystemService/RoomManageService/towerseatservice.h"
#include "Entitys/SystemRoomManageEntitys.h"
#include "Service/comboboxitemservice.h"

SysTowerSeat::SysTowerSeat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysTowerSeat)
{
    ui->setupUi(this);
    init();
}

SysTowerSeat::~SysTowerSeat()
{
    delete ui;
}

void SysTowerSeat::init()
{
    m_service = new TowerSeatService(this);
    m_dialog = nullptr;
    m_comboBoxService = nullptr;

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "id" << "代码" << "描述");

    loadTableData();

    //新增（对话框）
    connect(ui->btn_insert,&QPushButton::clicked,this,&SysTowerSeat::onInsertBtnOnClicked);
    //修改（对话框）
    connect(ui->btn_update,&QPushButton::clicked,this,&SysTowerSeat::onUpdateBtnClicked);
    //删除
    connect(ui->btn_delete,&QPushButton::clicked,this,&SysTowerSeat::onDelBtnOnClicked);

    //刷新
    connect(ui->btn_refresh,&QPushButton::clicked,this,&SysTowerSeat::loadTableData);
}

/**
 * @brief SysTowerSeat::loadTableData
 * 加载表格数据/刷新
 */
void SysTowerSeat::loadTableData()
{
    //清除表格数据
    ui->tableWidget->clearContents();

    QList<TowerSeatInfo> data = m_service->selectTowerSeat();

    ui->tableWidget->setRowCount(data.size());

    for(int i = 0; i < data.size(); i++)
    {
        TowerSeatInfo dataItem = data.at(i);

        QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(dataItem.tower_seat_id));
        item1->setData(Qt::UserRole,dataItem.tower_seat_id);
        item1->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,0,item1);

        QTableWidgetItem *item3 = new QTableWidgetItem(dataItem.code);
        item3->setData(Qt::UserRole,dataItem.tower_seat_id);
        item3->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,1,item3);

        QTableWidgetItem *item4 = new QTableWidgetItem(dataItem.m_describe);
        item4->setData(Qt::UserRole,dataItem.tower_seat_id);
        item4->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,2,item4);
    }

    ui->tableWidget->hideColumn(0);
}

/**
 * @brief SysTowerSeat::createDialog
 * @param name
 * 创建对话框(复用)
 */
void SysTowerSeat::createDialog(QString name)
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
        connect(m_updateOKBtn,&QPushButton::clicked,this,&SysTowerSeat::onUpDiaUpdateClicked);
        m_btnLayout->addWidget(m_updateOKBtn);
    }
    else
    {
        m_insertOKBtn = new QPushButton(tr("新增"));
        connect(m_insertOKBtn,&QPushButton::clicked,this,&SysTowerSeat::onInsDiaInsBtnClicked);
        m_btnLayout->addWidget(m_insertOKBtn);
    }

    m_cancel = new QPushButton(tr("取消"));
    connect(m_cancel,&QPushButton::clicked,this,&SysTowerSeat::onDiaCanBtnClicked);
    m_btnLayout->addWidget(m_cancel);

    m_mainLayout->addLayout(m_contentLayout);
    m_mainLayout->addLayout(m_btnLayout);

    m_dialog->show();
}

/**
 * @brief SysTowerSeat::onInsertBtnOnClicked
 * 新建按钮被按下
 */
void SysTowerSeat::onInsertBtnOnClicked()
{
    createDialog();
}

/**
 * @brief SysTowerSeat::onInsDiaInsBtnClicked
 * 对话框中，新增按钮被按下(真正的保存)
 */
void SysTowerSeat::onInsDiaInsBtnClicked()
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

    if(m_service->insertTowerSeat(code,describe))
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
 * @brief SysTowerSeat::onUpdateBtnClicked
 * 修改按钮被按下
 */
void SysTowerSeat::onUpdateBtnClicked()
{
    QModelIndex index = ui->tableWidget->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::warning(this,"修改提示","请选中一行数据后再点击修改");
        return;
    }

    m_towerSeatID = 0;

    m_towerSeatID = index.sibling(index.row(),0).data(Qt::UserRole).toInt();


    if(m_towerSeatID <= 0)
    {
        QMessageBox::warning(this,"修改提示","数据错误，请刷新页面后重试！");
        return;
    }

    createDialog("修改");

    //回填数据
    m_dmEdit->setText(index.sibling(index.row(),2).data().toString().trimmed());
    m_desEdit->setText(index.sibling(index.row(),3).data().toString().trimmed());
}

/**
 * @brief SysTowerSeat::onUpDiaUpdateClicked
 * 对话框中，修改按钮被按下
 */
void SysTowerSeat::onUpDiaUpdateClicked()
{
    //获取数据
    if(m_towerSeatID <= 0)
    {
        QMessageBox::warning(m_dialog,"修改提示","数据错误，请刷新页面后重试！");
        createDialog("修改");
        return;
    }

    QString code = m_dmEdit->text().trimmed();
    QString describe = m_desEdit->text().trimmed();

    if(code.isEmpty() || describe.isEmpty())
    {
        QMessageBox::warning(m_dialog,"修改提示","请填写完整！");
        return;
    }

    if(m_service->updateTowerSeat(m_towerSeatID,code,describe))
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
 * @brief SysTowerSeat::onDiaCanBtnClicked
 * 对话框中，取消按钮被按下
 */
void SysTowerSeat::onDiaCanBtnClicked()
{
    if(m_dialog != nullptr)
    {
        m_dialog->close();
        delete  m_dialog;
        m_dialog = nullptr;
    }
}

/**
 * @brief SysTowerSeat::onDelBtnOnClicked
 * 删除按钮被按下
 */
void SysTowerSeat::onDelBtnOnClicked()
{
    QModelIndex index = ui->tableWidget->currentIndex();
    if(!index.isValid())
    {
        QMessageBox::warning(this,"删除提示","请选中一行数据后再点击删除！");
        return;
    }

    m_towerSeatID = 0;
    m_towerSeatID = index.sibling(index.row(),0).data(Qt::UserRole).toInt();

    if(m_towerSeatID <= 0)
    {
        QMessageBox::warning(this,"删除提示","数据错误，请刷新页面后重试！");
        return;
    }

    int btn = QMessageBox::question(this,"删除提示","确定要删除该数据吗？");

    if(btn == QMessageBox::Yes)
    {
        if(m_service->deleteTowerSeat(m_towerSeatID))
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
