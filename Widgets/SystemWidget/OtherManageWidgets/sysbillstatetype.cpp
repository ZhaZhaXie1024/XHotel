#include "sysbillstatetype.h"
#include "ui_sysbillstatetype.h"

#include "Entitys/SysOtherEntitys.h"
#include "Service/SystemService/OtherManageService/billstatetypeservice.h"

#include <QMessageBox>
#include <QDebug>

SysBillStateType::SysBillStateType(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysBillStateType)
{
    ui->setupUi(this);
    init();
}

SysBillStateType::~SysBillStateType()
{
    delete ui;
}

void SysBillStateType::init()
{
    m_service = new BillStateTypeService(this);
    bill_state_type_id = 0;

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "id" << "代码" << "订单状态");

    loadTableData();

    //单击表格
    connect(ui->tableWidget,&QTableWidget::cellClicked,this,&SysBillStateType::onCellClicked);

    //新增
    connect(ui->btn_insert,&QPushButton::clicked,this,&SysBillStateType::oninsertBtnClicked);
    //修改
    connect(ui->btn_update,&QPushButton::clicked,this,&SysBillStateType::onupdateBtnClicked);
    //删除
    connect(ui->btn_delete,&QPushButton::clicked,this,&SysBillStateType::ondeleteBtnClicked);
    //刷新
    connect(ui->btn_refresh,&QPushButton::clicked,this,&SysBillStateType::loadTableData);
}

/**
 * @brief SysBillStateType::loadTableData
 * 加载数据
 */
void SysBillStateType::loadTableData()
{
    //清除表格数据
    ui->tableWidget->clearContents();

    QList<BillStateTypeInfo> data = m_service->select();

    ui->tableWidget->setRowCount(data.size());

    for(int i = 0; i < data.size(); i++)
    {
        BillStateTypeInfo info = data.at(i);

        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(info.bill_state_type_id));
        item0->setData(Qt::UserRole,info.bill_state_type_id);
        item0->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,0,item0);

        QTableWidgetItem *item1 = new QTableWidgetItem(info.code);
        item1->setData(Qt::UserRole,info.bill_state_type_id);
        item1->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,1,item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(info.bill_state);
        item2->setData(Qt::UserRole,info.bill_state_type_id);
        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,2,item2);

    }

    ui->tableWidget->hideColumn(0);
}

/**
 * @brief SysBillStateType::onCellClicked
 * @param row
 * @param col
 * 单击表格中任意一，回填数据
 */
void SysBillStateType::onCellClicked(int , int )
{
    QModelIndex index = ui->tableWidget->currentIndex();

    if(!index.isValid())
    {
        return;
    }

    //回填数据
    ui->line_code->setText(index.sibling(index.row(),1).data().toString());
    ui->line_state->setText(index.sibling(index.row(),2).data().toString());
}

/**
 * @brief SysBillStateType::oninsertBtnClicked
 * 新增按钮被按下
 */
void SysBillStateType::oninsertBtnClicked()
{
    //获取数据
    QString code = ui->line_code->text().trimmed();
    QString describe = ui->line_state->text().trimmed();

    //判断
    if(code.isEmpty() || describe.isEmpty())
    {
        QMessageBox::information(this,"新增提示","您输入的数据不完整，请检查数据后重新点击新增！");
        return;
    }

    if(m_service->insertBillStateType(code,describe))
    {
        QMessageBox::information(this,"新增提示","成功新增一条数据");
        ui->line_state->clear();
        ui->line_code->clear();
        loadTableData();
    }
    else
    {
        QString text = m_service->getLastError().trimmed();
        if(text.isEmpty())
        {
            text = "新增失败，请刷新页面后重试！";
        }
        QMessageBox::warning(this,"新增提示",text);
    }

}

/**
 * @brief SysBillStateType::onupdateBtnClicked
 * 修改按钮被按下
 */
void SysBillStateType::onupdateBtnClicked()
{
    QModelIndex index = ui->tableWidget->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::warning(this,"修改提示","请选中一条数据后重试！");
        return;
    }

    bill_state_type_id = 0;
    bill_state_type_id = index.sibling(index.row(),0).data(Qt::UserRole).toInt();

    //获取数据
    if(bill_state_type_id <= 0)
    {
        QMessageBox::warning(this,"修改提示","数据错误，请刷新页面后重试！");
        return;
    }

    QString code = ui->line_code->text().trimmed();
    QString describe = ui->line_state->text().trimmed();

    if(code.isEmpty() || describe.isEmpty())
    {
        QMessageBox::warning(this,"修改提示","请填写完整！");
        return;
    }

    if(m_service->updateBillStateType(bill_state_type_id,code,describe))
    {
        QMessageBox::information(this,"修改提示","成功修改一条数据");
        ui->line_state->clear();
        ui->line_code->clear();
        loadTableData();
    }
    else
    {
        QString text = m_service->getLastError().trimmed();
        if(text.isEmpty())
        {
            text = "修改失败，请刷新页面后重试！";
        }
        QMessageBox::warning(this,"修改提示",text);
    }
}

/**
 * @brief SysBillStateType::ondeleteBtnClicked
 * 删除按钮被按下
 */
void SysBillStateType::ondeleteBtnClicked()
{
    QModelIndex index = ui->tableWidget->currentIndex();
    if(!index.isValid())
    {
        QMessageBox::warning(this,"删除提示","请选中一行数据后再点击删除！");
        return;
    }

    bill_state_type_id = 0;
    bill_state_type_id = index.sibling(index.row(),0).data(Qt::UserRole).toInt();

    if(bill_state_type_id <= 0)
    {
        QMessageBox::warning(this,"删除提示","数据错误，请刷新页面后重试！");
        return;
    }

    int btn = QMessageBox::question(this,"删除提示","确定要删除该数据吗？");

    if(btn == QMessageBox::Yes)
    {
        if(m_service->deleteBillStateType(bill_state_type_id))
        {
            QMessageBox::information(this,"删除提示","成功删除一条数据");
            ui->line_state->clear();
            loadTableData();
        }
        else
        {
            QString text = m_service->getLastError().trimmed();
            if(text.isEmpty())
            {
                text = "删除失败，请刷新页面后重试！";
            }
            QMessageBox::warning(this,"删除提示",text);
        }
    }
}

