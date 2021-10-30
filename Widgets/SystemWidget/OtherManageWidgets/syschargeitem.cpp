#include "syschargeitem.h"
#include "ui_syschargeitem.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <QMessageBox>

#include "Service/SystemService/OtherManageService/chargeitemservice.h"
#include "Entitys/SysOtherEntitys.h"

SysChargeItem::SysChargeItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysChargeItem)
{
    ui->setupUi(this);
    init();
}

SysChargeItem::~SysChargeItem()
{
    delete ui;
}

/**
 * @brief SysChargeItem::init
 * 初始化
 */
void SysChargeItem::init()
{
    m_service = new ChargeItemService(this);
    m_dialog = nullptr;
    charge_item_id = 0;

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "id" << "价格" << "描述");

    loadTableData();

    //新增（对话框）
    connect(ui->btn_insert,&QPushButton::clicked,this,&SysChargeItem::onInsertBtnOnClicked);
    //修改（对话框）
    connect(ui->btn_update,&QPushButton::clicked,this,&SysChargeItem::onUpdateBtnClicked);
    //删除
    connect(ui->btn_delete,&QPushButton::clicked,this,&SysChargeItem::onDelBtnOnClicked);
    //刷新
    connect(ui->btn_refresh,&QPushButton::clicked,this,&SysChargeItem::loadTableData);
}

/**
 * @brief SysChargeItem::createDialog
 * @param name
 * 创建对话框，默认为新增对话框
 */
void SysChargeItem::createDialog(QString name)
{
    onDiaCanBtnClicked();

    m_dialog = new QDialog(this,Qt::Dialog|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);

    m_mainLayout = new QVBoxLayout(m_dialog);
    m_contentLayout = new QHBoxLayout;
    m_labelLayout = new QVBoxLayout;
    m_editLayout = new QVBoxLayout;
    m_btnLayout = new QHBoxLayout;


    m_jeLabel = new QLabel(tr("金额："));
    m_jeLabel->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
    m_desLabel = new QLabel(tr("描述："));
    m_desLabel->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

    m_labelLayout->addWidget(m_jeLabel);
    m_labelLayout->addWidget(m_desLabel);

    m_jeEdit = new QLineEdit;
    m_desEdit = new QLineEdit;
    m_jeEdit->setClearButtonEnabled(true);
    m_desEdit->setClearButtonEnabled(true);

    m_editLayout->addWidget(m_jeEdit);
    m_editLayout->addWidget(m_desEdit);

    m_contentLayout->addLayout(m_labelLayout);
    m_contentLayout->addLayout(m_editLayout);

    m_btnLayout->addStretch();
    if(name.compare("修改") == 0)
    {
        m_updateOKBtn = new QPushButton(tr("修改"));
        m_dialog->setWindowTitle("修改收费项目");
        connect(m_updateOKBtn,&QPushButton::clicked,this,&SysChargeItem::onUpDiaUpdateClicked);
        m_btnLayout->addWidget(m_updateOKBtn);
    }
    else
    {
        m_insertOKBtn = new QPushButton(tr("新增"));
        m_dialog->setWindowTitle("新增收费项目");
        connect(m_insertOKBtn,&QPushButton::clicked,this,&SysChargeItem::onInsDiaInsBtnClicked);
        m_btnLayout->addWidget(m_insertOKBtn);
    }

    m_cancel = new QPushButton(tr("取消"));
    connect(m_cancel,&QPushButton::clicked,this,&SysChargeItem::onDiaCanBtnClicked);
    m_btnLayout->addWidget(m_cancel);

    m_mainLayout->addLayout(m_contentLayout);
    m_mainLayout->addLayout(m_btnLayout);

    m_dialog->show();
}

/**
 * @brief SysChargeItem::loadTableData
 * 加载表格数据
 */
void SysChargeItem::loadTableData()
{
    //清除表格数据
    ui->tableWidget->clearContents();

    QList<ChargeItemInfo> data = m_service->select();

    ui->tableWidget->setRowCount(data.size());

    for(int i = 0; i < data.size(); i++)
    {
        ChargeItemInfo info = data.at(i);

        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(info.charge_item_id));
        item0->setData(Qt::UserRole,info.charge_item_id);
        item0->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,0,item0);

        QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(info.money));
        item1->setData(Qt::UserRole,info.charge_item_id);
        item1->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,1,item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(info.m_describe);
        item2->setData(Qt::UserRole,info.charge_item_id);
        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,2,item2);

    }

    ui->tableWidget->hideColumn(0);
}

/**
 * @brief SysChargeItem::onInsertBtnOnClicked
 * 新建按钮被按下
 */
void SysChargeItem::onInsertBtnOnClicked()
{
    createDialog();
}

/**
 * @brief SysChargeItem::onInsDiaInsBtnClicked
 * 对话框中，新增按钮被按下(真正的保存)
 */
void SysChargeItem::onInsDiaInsBtnClicked()
{
    //获取数据
    double money = m_jeEdit->text().trimmed().toDouble();
    QString describe = m_desEdit->text().trimmed();

    //判断
    if(money <= 0.00 || describe.isEmpty())
    {
        QMessageBox::information(m_dialog,"新增提示","您输入的数据不完整，请检查数据后重新点击新增！");
        return;
    }

    QVariantList values;
    values << money << describe;

    if(m_service->insertChargeItem(values))
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
 * @brief SysChargeItem::onUpdateBtnClicked
 * 修改按钮被按下
 */
void SysChargeItem::onUpdateBtnClicked()
{
    QModelIndex index = ui->tableWidget->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::warning(this,"修改提示","请选中一行数据后再点击修改");
        return;
    }

    charge_item_id = 0;

    charge_item_id = index.sibling(index.row(),0).data(Qt::UserRole).toInt();


    if(charge_item_id <= 0)
    {
        QMessageBox::warning(this,"修改提示","数据错误，请刷新页面后重试！");
        return;
    }

    createDialog("修改");

    //回填数据
    m_jeEdit->setText(index.sibling(index.row(),1).data().toString().trimmed());
    m_desEdit->setText(index.sibling(index.row(),2).data().toString().trimmed());
}

/**
 * @brief SysChargeItem::onUpDiaUpdateClicked
 * 对话框中，修改按钮被按下
 */
void SysChargeItem::onUpDiaUpdateClicked()
{
    //获取数据
    if(charge_item_id <= 0)
    {
        QMessageBox::warning(m_dialog,"修改提示","数据错误，请刷新页面后重试！");
        createDialog("修改");
        return;
    }

    double money = m_jeEdit->text().trimmed().toDouble();
    QString describe = m_desEdit->text().trimmed();

    if(money <= 0.00 || describe.isEmpty())
    {
        QMessageBox::warning(m_dialog,"修改提示","请填写完整！");
        return;
    }

    if(m_service->updateChargeItem(charge_item_id,money,describe))
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
 * @brief SysChargeItem::onDiaCanBtnClicked
 * 对话框中，取消按钮被按下
 */
void SysChargeItem::onDiaCanBtnClicked()
{
    if(m_dialog != nullptr)
    {
        m_dialog->close();
        delete m_dialog;
        m_dialog = nullptr;
    }
}

/**
 * @brief SysChargeItem::onDelBtnOnClicked
 * 删除按钮被按下
 */
void SysChargeItem::onDelBtnOnClicked()
{
    QModelIndex index = ui->tableWidget->currentIndex();
    if(!index.isValid())
    {
        QMessageBox::warning(this,"删除提示","请选中一行数据后再点击删除！");
        return;
    }

    charge_item_id = 0;
    charge_item_id = index.sibling(index.row(),0).data(Qt::UserRole).toInt();

    if(charge_item_id <= 0)
    {
        QMessageBox::warning(this,"删除提示","数据错误，请刷新页面后重试！");
        return;
    }

    int btn = QMessageBox::question(this,"删除提示","确定要删除该数据吗？");

    if(btn == QMessageBox::Yes)
    {
        if(m_service->deleteChargeItem(charge_item_id))
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


