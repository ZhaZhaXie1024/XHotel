#include "sysoperator.h"
#include "ui_sysoperator.h"

#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QDebug>

#include "Service/SystemService/StaffManageService/sysoperatorservice.h"
#include "Entitys/SystemSimpleModelEntity.h"

SysOperator::SysOperator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysOperator)
{
    ui->setupUi(this);
    init();
}

SysOperator::~SysOperator()
{
    delete ui;
}

void SysOperator::init()
{
    m_service = new SysOperatorService(this);
    m_dialog = nullptr;
    m_staffDialog = nullptr;

    m_operatorID = 0;
    m_staffID = 0;

    //初始化表格样式
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "operator_id" << "staff_id"
                                               << "员工名称" << "部门" << "职业");

    loadTableData();

    //新建按钮被按下
    connect(ui->btn_insert,&QPushButton::clicked,this,&SysOperator::onInsertBtnOnClicked);
    //修改按钮被按下
    connect(ui->btn_update,&QPushButton::clicked,this,&SysOperator::onUpdateBtnClicked);
    //刷新按钮按下
    connect(ui->btn_refresh,&QPushButton::clicked,this,&SysOperator::loadTableData);
    //删除按钮被按下
    connect(ui->btn_delete,&QPushButton::clicked,this,&SysOperator::onDelBtnOnClicked);
}

/**
 * @brief SysOperator::loadTableData
 * 加载表格数据
 */
void SysOperator::loadTableData()
{
    //清除表格数据
    ui->tableWidget->clearContents();

    QList<StaffOperator> data = m_service->select();

    ui->tableWidget->setRowCount(data.size());

    int row = 0;

    for(int i = 0; i < data.size(); i++)
    {
        StaffOperator dataItem = data.at(i);
        QTableWidgetItem *item1 = new QTableWidgetItem(dataItem.operator_id);
        item1->setData(Qt::UserRole,dataItem.operator_id);
        item1->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row,0,item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(dataItem.staff_id);
        item2->setData(Qt::UserRole,dataItem.staff_id);
        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row,1,item2);

        QTableWidgetItem *item3 = new QTableWidgetItem(dataItem.staffName);
        item3->setData(Qt::UserRole,dataItem.staff_id);
        item3->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row,2,item3);

        QTableWidgetItem *item4= new QTableWidgetItem(dataItem.depName);
        item4->setData(Qt::UserRole,dataItem.operator_id);
        item4->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row,3,item4);

        QTableWidgetItem *item5= new QTableWidgetItem(dataItem.posName);
        item5->setData(Qt::UserRole,dataItem.operator_id);
        item5->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(row,4,item5);

        row++;
    }

    ui->tableWidget->hideColumn(0);
    ui->tableWidget->hideColumn(1);
}

/**
 * @brief SysOperator::createDialog
 * @param name
 * 创建新增/修改对话框
 */
void SysOperator::createDialog(QString name)
{
    if(m_dialog != nullptr)
    {
        onDiaCanBtnClicked();
    }

    m_dialog = new QDialog(this,Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    //设置为模态框
    m_dialog->setModal(true);
    //设置对话框标题
    m_dialog->setWindowTitle(name);

    m_mainLayout = new QVBoxLayout(m_dialog);
    m_mainLayout->setSpacing(10);
    m_mainLayout->setContentsMargins(12,20,12,12);

    m_selectStaff = new QPushButton(tr("--选择员工--"));
    m_mainLayout->addWidget(m_selectStaff);

    m_pasLayout = new QHBoxLayout;
    m_pasLabelLayout = new QVBoxLayout;
    m_pasEditLayout = new QVBoxLayout;

    m_passwordLabel = new QLabel(tr("密码："));
    m_passwordLabel->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

    m_verifyPasswordbeLabel = new QLabel(tr("确认密码："));
    m_verifyPasswordbeLabel->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

    m_pasLabelLayout->addWidget(m_passwordLabel);
    m_pasLabelLayout->addWidget(m_verifyPasswordbeLabel);

    m_passwordEdit = new QLineEdit;
    m_passwordEdit->setClearButtonEnabled(true);
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_verifyPasswordbeEdit = new QLineEdit;
    m_verifyPasswordbeEdit->setClearButtonEnabled(true);
    m_verifyPasswordbeEdit->setEchoMode(QLineEdit::Password);


    m_pasEditLayout->addWidget(m_passwordEdit);
    m_pasEditLayout->addWidget(m_verifyPasswordbeEdit);

    m_pasLayout->addLayout(m_pasLabelLayout);
    m_pasLayout->addLayout(m_pasEditLayout);

    m_btnLayout = new QHBoxLayout;
    m_btnLayout->addStretch();
    m_cancel = new QPushButton(tr("取消"));
    if(name.trimmed().compare("修改") == 0)
    {
        m_updateOKBtn = new QPushButton(tr("修改"));
        connect(m_updateOKBtn,&QPushButton::clicked,this,&SysOperator::onUpDiaUpdateClicked);
        m_btnLayout->addWidget(m_updateOKBtn);
    }
    else
    {
        m_staffID = 0;
        m_insertOKBtn = new QPushButton(tr("新增"));
        connect(m_insertOKBtn,&QPushButton::clicked,this,&SysOperator::onInsDiaInsBtnClicked);
        m_btnLayout->addWidget(m_insertOKBtn);
    }

    //选择员工按钮被按下
    connect(m_selectStaff,&QPushButton::clicked,this,&SysOperator::onDiaSelectStaffBtnClicked);

    //对话框取消按钮被按下
    connect(m_cancel,&QPushButton::clicked,this,&SysOperator::onDiaCanBtnClicked);

    m_btnLayout->addWidget(m_cancel);

    m_mainLayout->addLayout(m_pasLayout);
    m_mainLayout->addLayout(m_btnLayout);

    m_dialog->show();
}

/**
 * @brief SysOperator::initStaffTableWidget
 * 初始化选择员工表格
 */
void SysOperator::initStaffTableWidget()
{
    //清除表格数据
    m_staffWidget->clearContents();

    m_staffWidget->setColumnCount(4);
    m_staffWidget->setHorizontalHeaderLabels(QStringList()<<"id"<<"员工名称"<<"部门"<<"职业");
    m_staffWidget->verticalHeader()->hide();

    //获取数据
    QString name = m_staffNameEdit->text().trimmed();
    QString idCard = m_staffIdCardEdit->text().trimmed();
    QString phone = m_staffPhoneEdit->text().trimmed();

    QStringList keys;
    QVariantList values;

    if(!name.isEmpty())
    {
        keys.append("name");
        values.append(name);
    }
    if(!idCard.isEmpty())
    {
        keys.append("id_card");
        values.append(idCard);
    }
    if(!phone.isEmpty())
    {
        keys.append("phone");
        values.append(phone);
    }

    //开始查询
    QList<StaffOperator> data = m_service->selectStaff(keys,values);

    //开始绑定数据
    m_staffWidget->setRowCount(data.size());

    int row = 0;

    for(int i = 0; i < data.size(); i++)
    {
        StaffOperator dataItem = data.at(i);
        QTableWidgetItem *item1 = new QTableWidgetItem(dataItem.staff_id);
        item1->setData(Qt::UserRole,dataItem.staff_id);
        m_staffWidget->setItem(row,0,item1);

        QTableWidgetItem *item3 = new QTableWidgetItem(dataItem.staffName);
        item3->setData(Qt::UserRole,dataItem.staff_id);
        m_staffWidget->setItem(row,1,item3);

        QTableWidgetItem *item4= new QTableWidgetItem(dataItem.depName);
        item4->setData(Qt::UserRole,dataItem.staff_id);
        m_staffWidget->setItem(row,2,item4);

        QTableWidgetItem *item5= new QTableWidgetItem(dataItem.posName);
        item5->setData(Qt::UserRole,dataItem.staff_id);
        m_staffWidget->setItem(row,3,item5);

        row++;
    }

    m_staffWidget->hideColumn(0);
}


/**
 * @brief SysOperator::onInsertBtnOnClicked
 * 新建按钮被按下
 */
void SysOperator::onInsertBtnOnClicked()
{
    createDialog();
}

/**
 * @brief SysOperator::onDiaSelectStaffBtnClicked
 * 新增对话框中，选择员工按钮被按下
 * 弹出对话框选择一名员工
 */
void SysOperator::onDiaSelectStaffBtnClicked()
{
    if(m_staffDialog == nullptr)
    {
        m_staffDialog = new QDialog(m_dialog);
        m_staffDialog->setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::MSWindowsFixedSizeDialogHint);
        m_staffDialog->setModal(true);
        m_staffDialog->setWindowTitle(tr("选择员工"));
        m_staffLayout = new QVBoxLayout(m_staffDialog);

        m_staffEditLayout = new QHBoxLayout;
        m_staffNameLabel = new QLabel(tr("员工姓名："));
        m_staffNameEdit = new QLineEdit;
        m_staffIdCardLabel = new QLabel(tr("身份证："));
        m_staffIdCardEdit = new QLineEdit;
        m_staffIdCardEdit->setMaxLength(18);
        m_staffPhoneLabel = new QLabel(tr("手机号："));
        m_staffPhoneEdit = new QLineEdit;
        m_staffPhoneEdit->setMaxLength(11);
        m_staffDiaSelect = new QPushButton(tr("查询"));

        m_staffEditLayout->addWidget(m_staffNameLabel);
        m_staffEditLayout->addWidget(m_staffNameEdit);
        m_staffEditLayout->addWidget(m_staffIdCardLabel);
        m_staffEditLayout->addWidget(m_staffIdCardEdit);
        m_staffEditLayout->addWidget(m_staffPhoneLabel);
        m_staffEditLayout->addWidget(m_staffPhoneEdit);
        m_staffEditLayout->addWidget(m_staffDiaSelect);
        m_staffEditLayout->addStretch();

        m_staffWidget = new QTableWidget;
        m_staffWidget->horizontalHeader()->setStretchLastSection(true);
        m_staffWidget->setSelectionMode(QAbstractItemView::SingleSelection);
        m_staffWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

        //双击行，保存它的staffID到m_staffID
        connect(m_staffWidget,&QTableWidget::doubleClicked,this,&SysOperator::onStaffWidgetDClicked);

        m_staffLayout->addLayout(m_staffEditLayout);
        m_staffLayout->addWidget(m_staffWidget);

        connect(m_staffDiaSelect,&QPushButton::clicked,this,&SysOperator::onInsDiaDiaSelBtnClicked);
    }

    initStaffTableWidget();
    m_staffNameEdit->clear();
    m_staffIdCardEdit->clear();
    m_staffPhoneEdit->clear();
    m_staffDialog->show();
}

/**
 * @brief SysOperator::onInsDiaDiaSelBtnClicked
 * 新增对话框中选择员工对话框中的查询按钮被按下
 */
void SysOperator::onInsDiaDiaSelBtnClicked()
{
    initStaffTableWidget();
}

/**
 * @brief SysOperator::onStaffWidgetDClicked
 * @param index
 * 选择员工对话框表格被双击
 */
void SysOperator::onStaffWidgetDClicked(const QModelIndex &index)
{
    m_staffID = 0;

    if(index.isValid())
    {
        m_staffID = index.data(Qt::UserRole).toInt();

        if(m_staffID <= 0)
        {
            QMessageBox::warning(m_staffDialog,"提示","数据异常请刷新页面后再试！");
            m_staffDialog->close();
            return;
        }

        QString name = index.sibling(index.row(),1).data().toString();

        m_selectStaff->setText(name);

        m_staffDialog->hide();
    }
}


/**
 * @brief SysOperator::onInsDiaInsBtnClicked
 * 对话框中新建按钮被按下，进行新增操作
 */
void SysOperator::onInsDiaInsBtnClicked()
{
    //获取数据
    QString password = m_passwordEdit->text().trimmed();
    QString verPassword = m_verifyPasswordbeEdit->text().trimmed();

    //数据异常
    if(m_staffID <= 0 || password.isEmpty() || verPassword.isEmpty())
    {
        QMessageBox::warning(m_dialog,"提示","数据异常，请刷新页面后重试！");
        m_dialog->close();
        return;
    }

    if(password.length() < 8 || password.length() > 18 ||
            verPassword.length() < 8 || verPassword.length() > 18)
    {
        QMessageBox::warning(m_dialog,"提示","密码长度要大于等于8小于等于18！");
        return;
    }

    if(password.compare(verPassword) != 0)
    {
        QMessageBox::warning(m_dialog,"提示","密码和确认密码不一致请重新输入！");
        return;
    }

    if(m_service->insertOperator(m_staffID,verPassword))
    {
        QMessageBox::information(m_dialog,"新增提示","成功新增了一条数据");
        loadTableData();
        onDiaCanBtnClicked();
    }
    else
    {
        QString text = m_service->getLastError();
        if(text.trimmed().isEmpty())
        {
            text = "新增失败！请刷新页面后重试！";
        }
        QMessageBox::warning(m_dialog,"新增提示",text);
    }

}

/**
 * @brief SysOperator::onUpdateBtnClicked
 * 修改按钮被按下
 */
void SysOperator::onUpdateBtnClicked()
{
    m_operatorID = 0;

    QModelIndex index = ui->tableWidget->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::information(this,"通知",
                                 "请选中一行后再点击修改");
        return;
    }


    m_operatorID = index.sibling(index.row(),0).data(Qt::UserRole).toInt();

    QString name = index.sibling(index.row(),2).data().toString();

    if(m_operatorID > 0)
    {
        createDialog("修改");
        m_selectStaff->setText(name);
        m_selectStaff->setEnabled(false);
    }
    else
    {
        QMessageBox::information(this,"通知",
                                 "数据错误，请刷新页面后重试");
    }
}

/**
 * @brief SysOperator::onUpDiaUpdateClicked
 * 对话框中，修改按钮被按下
 */
void SysOperator::onUpDiaUpdateClicked()
{
    //获取数据
    QString password = m_passwordEdit->text().trimmed();
    QString verPassword = m_verifyPasswordbeEdit->text().trimmed();

    //数据异常
    if(m_operatorID <= 0 || password.isEmpty() || verPassword.isEmpty())
    {
        QMessageBox::warning(m_dialog,"提示","数据异常，请刷新页面后重试！");
        m_dialog->close();
        return;
    }

    if(password.length() < 8 || password.length() > 18 ||
            verPassword.length() < 8 || verPassword.length() > 18)
    {
        QMessageBox::warning(m_dialog,"提示","密码长度要大于等于8小于等于18！");
        return;
    }

    if(password.compare(verPassword) != 0)
    {
        QMessageBox::warning(m_dialog,"提示","密码和确认密码不一致请重新输入！");
        return;
    }

    if(m_service->updateOperator(m_operatorID,verPassword))
    {
        QMessageBox::information(m_dialog,"修改提示","成功修改了一条数据");
        loadTableData();
        onDiaCanBtnClicked();
    }
    else
    {
        QString text = m_service->getLastError();
        if(text.trimmed().isEmpty())
        {
            text = "修改失败！请刷新页面后重试！";
        }
        QMessageBox::warning(m_dialog,"修改提示",text);
    }
}

/**
 * @brief SysOperator::onDiaCanBtnClicked
 * 对话框中，取消按钮被按下
 */
void SysOperator::onDiaCanBtnClicked()
{
    if(m_dialog != nullptr)
    {
        m_dialog->close();
    }

    if(m_staffDialog != nullptr)
    {
        delete m_staffDialog;
        m_staffDialog = nullptr;
    }

    delete m_dialog;

    m_dialog = nullptr;
}

/**
 * @brief SysOperator::onDelBtnOnClicked
 * 删除一条数据
 */
void SysOperator::onDelBtnOnClicked()
{
    QModelIndex index = ui->tableWidget->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::information(this,"通知",
                                 "请选中一行后再点击删除");
        return;
    }

    m_operatorID = index.sibling(index.row(),0).data(Qt::UserRole).toInt();

    if(m_operatorID > 0)
    {
        int btn = QMessageBox::question(this,"删除提示","确定要删除改行数据？");
        if(btn == QMessageBox::Yes)
        {
            if(m_service->deleteOperator(m_operatorID))
            {
                QMessageBox::information(m_dialog,"删除提示","成功删除了一条数据");
                loadTableData();
            }
            else
            {
                QString text = m_service->getLastError();
                if(text.trimmed().isEmpty())
                {
                    text = "删除失败！请刷新页面后重试！";
                }
                QMessageBox::warning(m_dialog,"删除提示",text);
            }
        }
    }
    else
    {
        QMessageBox::information(this,"通知",
                                 "数据错误，请刷新页面后重试");
    }
}
