#include "systeminitialvalue.h"
#include "ui_systeminitialvalue.h"

#include <QMessageBox>

#include "Service/SystemService/OtherManageService/teminitialvalueservice.h"
#include "Entitys/SysOtherEntitys.h"
#include "Widgets/SystemWidget/OtherManageWidgets/teminitialvaluedialog.h"

SystemInitialValue::SystemInitialValue(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SystemInitialValue)
{
    ui->setupUi(this);

    m_service = new TeminitialValueService(this);
    m_dialog = new TeminitialValueDialog(this);

    init();
}

SystemInitialValue::~SystemInitialValue()
{
    delete ui;
}

void SystemInitialValue::init()
{
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "id" << "应用类型" << "描述" << "前缀" << "初始值" << "当前值");

    loadData();

    connect(ui->btn_insert,&QPushButton::clicked,this,&SystemInitialValue::onInsertBtnOnClicked);
    connect(ui->btn_refresh,&QPushButton::clicked,this,&SystemInitialValue::loadData);
    connect(ui->btn_delete,&QPushButton::clicked,this,&SystemInitialValue::onDelBtnOnClicked);
    connect(m_dialog,&TeminitialValueDialog::insertData,this,&SystemInitialValue::onInsDiaInsBtnClicked);
}

/**
 * @brief SystemInitialValue::loadData
 * 加载表格数据
 */
void SystemInitialValue::loadData()
{
    //清除表格数据
    ui->tableWidget->clearContents();

    QList<TeminitialValueInfo> data = m_service->select();

    ui->tableWidget->setRowCount(data.size());

    for(int i = 0; i < data.size(); i++)
    {
        TeminitialValueInfo info = data.at(i);

        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(info.systemInitial_value_id));
        item0->setData(Qt::UserRole,info.systemInitial_value_id);
        item0->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,0,item0);

        QTableWidgetItem *item1 = new QTableWidgetItem(info.state);
        item1->setData(Qt::UserRole,info.systemInitial_value_id);
        item1->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,1,item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(info.m_describe);
        item2->setData(Qt::UserRole,info.systemInitial_value_id);
        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,2,item2);

        QTableWidgetItem *item3 = new QTableWidgetItem(info.prefix);
        item3->setData(Qt::UserRole,info.systemInitial_value_id);
        item3->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,3,item3);

        QTableWidgetItem *item4 = new QTableWidgetItem(QString::number(info.init_value));
        item4->setData(Qt::UserRole,info.systemInitial_value_id);
        item4->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,4,item4);

        QTableWidgetItem *item5 = new QTableWidgetItem(QString::number(info.this_value));
        item5->setData(Qt::UserRole,info.systemInitial_value_id);
        item5->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i,5,item5);
    }

    ui->tableWidget->hideColumn(0);
}

/**
 * @brief SystemInitialValue::onInsertBtnOnClicked
 * 新建按钮被按下
 */
void SystemInitialValue::onInsertBtnOnClicked()
{
    m_dialog->show();
}

/**
 * @brief SystemInitialValue::onInsDiaInsBtnClicked
 * @param values
 * 对话框中，新增按钮被按下(真正的保存)
 */
void SystemInitialValue::onInsDiaInsBtnClicked(QVariantList values)
{
    if(m_service->insertTeminital(values))
    {
        QMessageBox::information(this,"新增","新增成功");
        loadData();
        m_dialog->clearInput();
        m_dialog->close();
    }
    else
    {
        QMessageBox::warning(this,"新增","新增失败");
    }
}

/**
 * @brief SystemInitialValue::onDelBtnOnClicked
 * 删除按钮被按下
 */
void SystemInitialValue::onDelBtnOnClicked()
{
    QModelIndex index = ui->tableWidget->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::warning(this,"提示","当前没有选中数据");
        return;
    }

    int id = index.data(Qt::UserRole).toInt();

    if(id <= 0)
    {
        QMessageBox::warning(this,"提示","当前数据错误，刷新页面后重试！");
        return;
    }

    int btn = QMessageBox::question(this,"提示","您确定要删除吗？");

    if(btn == QMessageBox::Yes)
    {
        if(m_service->deleteTeminital(id))
        {
            QMessageBox::information(this,"删除","删除成功");
            loadData();
        }
        else
        {
            QMessageBox::warning(this,"删除","删除失败");
        }
    }
}
