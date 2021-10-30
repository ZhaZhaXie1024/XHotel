#include "systowerrange.h"
#include "ui_systowerrange.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QMessageBox>

#include "Service/SystemService/RoomManageService/towerrangeservie.h"
#include "Entitys/SystemRoomManageEntitys.h"

SysTowerRange::SysTowerRange(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysTowerRange)
{
    ui->setupUi(this);
    init();
}

SysTowerRange::~SysTowerRange()
{
    delete ui;
}

void SysTowerRange::init()
{
    m_service = new TowerRangeServie(this);
    m_dialog = nullptr;
    m_towerRangeID = 0;

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "id" << "代码" << "描述" << "说明");

    //加载刷新数据
    loadTableData();

    //新增（对话框）
    connect(ui->btn_insert,&QPushButton::clicked,this,&SysTowerRange::onInsertBtnOnClicked);
    //修改（对话框）
    connect(ui->btn_update,&QPushButton::clicked,this,&SysTowerRange::onUpdateBtnClicked);
    //删除
    connect(ui->btn_delete,&QPushButton::clicked,this,&SysTowerRange::onDelBtnOnClicked);
    //刷新
    connect(ui->btn_refresh,&QPushButton::clicked,this,&SysTowerRange::loadTableData);
}

/**
 * @brief SysTowerRange::loadTableData
 * 初始化表格
 */
void SysTowerRange::loadTableData()
{
    //清除表格数据
    ui->tableWidget->clearContents();

    QList<TowerRangInfo> data = m_service->selectTowerRang();

    ui->tableWidget->setRowCount(data.size());

    for(int i = 0; i < data.size(); i++)
    {
        TowerRangInfo info = data.at(i);

        QTableWidgetItem *item = new QTableWidgetItem(QString::number(info.tower_range_id));
        item->setData(Qt::UserRole,info.tower_range_id);
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,0,item);

        QTableWidgetItem *item2 = new QTableWidgetItem(info.code);
        item2->setData(Qt::UserRole,info.tower_range_id);
        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,1,item2);

        QTableWidgetItem *item3 = new QTableWidgetItem(info.m_describe);
        item3->setData(Qt::UserRole,info.tower_range_id);
        item3->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,2,item3);

        QTableWidgetItem *item4 = new QTableWidgetItem(info.attribute);
        item4->setData(Qt::UserRole,info.tower_range_id);
        item4->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,3,item4);
    }

    ui->tableWidget->hideColumn(0);
}

/**
 * @brief SysTowerRange::createDialog
 * @param name
 * 创建新增/修改对话框（复用）
 */
void SysTowerRange::createDialog(QString name)
{
    onDiaCanBtnClicked();

    m_dialog = new QDialog(this,Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowCloseButtonHint);
    m_dialog->setModal(true);
    m_dialog->setWindowTitle(name);

    m_mainLayout = new QVBoxLayout(m_dialog);
    m_contentLayout = new QHBoxLayout;
    m_labelLayout = new QVBoxLayout;
    m_editLayout = new QVBoxLayout;
    m_btnLayout = new QHBoxLayout;

    m_dMLabel = new QLabel(tr("代码："));
    m_dMLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    m_describeLabel = new QLabel(tr("描述："));
    m_describeLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    m_describeLabel2 = new QLabel(tr("属性描述："));
    m_describeLabel2->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    m_labelLayout->addWidget(m_dMLabel);
    m_labelLayout->addWidget(m_describeLabel);
    m_labelLayout->addWidget(m_describeLabel2);

    m_dMEdit = new QLineEdit;
    m_describeEdit = new QLineEdit;
    m_describeEdit2 = new QLineEdit;

    m_editLayout->addWidget(m_dMEdit);
    m_editLayout->addWidget(m_describeEdit);
    m_editLayout->addWidget(m_describeEdit2);

    m_contentLayout->addLayout(m_labelLayout);
    m_contentLayout->addLayout(m_editLayout);

    m_btnLayout->addStretch();

    if(name.compare("修改") == 0)
    {
        m_updateOKBtn = new QPushButton(tr("修改"));
        connect(m_updateOKBtn,&QPushButton::clicked,this,&SysTowerRange::onUpDiaUpdateClicked);
        m_btnLayout->addWidget(m_updateOKBtn);
    }
    else
    {
        m_insertOKBtn = new QPushButton(tr("新增"));
        connect(m_insertOKBtn,&QPushButton::clicked,this,&SysTowerRange::onInsDiaInsBtnClicked);
        m_btnLayout->addWidget(m_insertOKBtn);
    }

    m_cancel = new QPushButton(tr("取消"));
    connect(m_cancel,&QPushButton::clicked,this,&SysTowerRange::onDiaCanBtnClicked);
    m_btnLayout->addWidget(m_cancel);

    m_mainLayout->addLayout(m_contentLayout);
    m_mainLayout->addLayout(m_btnLayout);

    m_dialog->show();
}

/**
 * @brief SysTowerRange::onInsertBtnOnClicked
 * 新建按钮被按下
 */
void SysTowerRange::onInsertBtnOnClicked()
{
    createDialog();
}

/**
 * @brief SysTowerRange::onInsDiaInsBtnClicked
 * 对话框中，新增按钮被按下(真正的保存)
 */
void SysTowerRange::onInsDiaInsBtnClicked()
{
    //获取数据
    QString code = m_dMEdit->text().trimmed();
    QString describe = m_describeEdit->text().trimmed();
    QString attribute = m_describeEdit2->text().trimmed();

    //判断
    if(code.isEmpty() || describe.isEmpty() || attribute.isEmpty())
    {
        QMessageBox::information(m_dialog,"新增提示","您输入的数据不完整，请检查数据后重新点击新增！");
        return;
    }

    if(m_service->insertTowerRang(code,describe,attribute))
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
 * @brief SysTowerRange::onUpdateBtnClicked
 * 修改按钮被按下
 */
void SysTowerRange::onUpdateBtnClicked()
{
    QModelIndex index = ui->tableWidget->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::warning(this,"修改提示","请选中一行数据后再点击修改");
        return;
    }

    m_towerRangeID = 0;

    m_towerRangeID = index.sibling(index.row(),0).data(Qt::UserRole).toInt();


    if(m_towerRangeID <= 0)
    {
        QMessageBox::warning(this,"修改提示","数据错误，请刷新页面后重试！");
        return;
    }

    createDialog("修改");

    //回填数据
    m_dMEdit->setText(index.sibling(index.row(),1).data().toString().trimmed());
    m_describeEdit->setText(index.sibling(index.row(),2).data().toString().trimmed());
    m_describeEdit2->setText(index.sibling(index.row(),3).data().toString().trimmed());
}

/**
 * @brief SysTowerRange::onUpDiaUpdateClicked
 * 对话框中，修改按钮被按下（修改保存了）
 */
void SysTowerRange::onUpDiaUpdateClicked()
{
    //获取数据
    if(m_towerRangeID <= 0)
    {
        QMessageBox::warning(m_dialog,"修改提示","数据错误，请刷新页面后重试！");
        createDialog("修改");
        return;
    }

    QString code = m_dMEdit->text().trimmed();
    QString describe = m_describeEdit->text().trimmed();
    QString attribue = m_describeEdit2->text().trimmed();

    if(code.isEmpty() || describe.isEmpty() || attribue.isEmpty())
    {
        QMessageBox::warning(m_dialog,"修改提示","请填写完整！");
        return;
    }

    if(m_service->updateTowerRang(m_towerRangeID,code,describe,attribue))
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
 * @brief SysTowerRange::onDiaCanBtnClicked
 * 对话框中，取消按钮被按下
 */
void SysTowerRange::onDiaCanBtnClicked()
{
    if(m_dialog != nullptr)
    {
        m_dialog->close();
        delete m_dialog;
        m_dialog = nullptr;
    }
}

/**
 * @brief SysTowerRange::onDelBtnOnClicked
 * 删除
 */
void SysTowerRange::onDelBtnOnClicked()
{
    QModelIndex index = ui->tableWidget->currentIndex();
    if(!index.isValid())
    {
        QMessageBox::warning(this,"删除提示","请选中一行数据后再点击删除！");
        return;
    }

    m_towerRangeID = 0;
    m_towerRangeID = index.sibling(index.row(),0).data(Qt::UserRole).toInt();

    if(m_towerRangeID <= 0)
    {
        QMessageBox::warning(this,"删除提示","数据错误，请刷新页面后重试！");
        return;
    }

    int btn = QMessageBox::question(this,"删除提示","确定要删除该数据吗？");

    if(btn == QMessageBox::Yes)
    {
        if(m_service->deleteTowerRang(m_towerRangeID))
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
