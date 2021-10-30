#include "sysdepartment.h"
#include "ui_sysdepartment.h"

#include <QSqlTableModel>
#include <QHBoxLayout>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

#include <QDebug>

#include "Service/SystemService/StaffManageService/sysdepartmentservice.h"

SysDepartment::SysDepartment(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysDepartment),
    m_depID(0)
{
    ui->setupUi(this);
    init();
}

SysDepartment::~SysDepartment()
{
    delete ui;
}

void SysDepartment::init()
{
    //初始化表格
    m_service = new SysDepartmentService(this);
    m_model = m_service->selectDepAll();

    ui->tableView->setModel(m_model);
    ui->tableView->hideColumn(0);

    //初始化连接
    connect(ui->btn_insert,&QPushButton::clicked,this,&SysDepartment::onInsertBtnOnClicked);    //新增按钮
    connect(ui->btn_updata,&QPushButton::clicked,this,&SysDepartment::onUpdateBtnClicked);      //修改按钮
    connect(ui->btn_delete,&QPushButton::clicked,this,&SysDepartment::onDelBtnOnClicked);       //删除按钮
    connect(ui->btn_refresh,&QPushButton::clicked,this,&SysDepartment::onRefBtnOnClicked);      //刷新按钮
    connect(m_model,&QSqlTableModel::dataChanged,this,&SysDepartment::onModelDataChanged);      //model数据被改变
    connect(ui->btn_submit,&QPushButton::clicked,this,&SysDepartment::onSubmitBtnClicked);      //提交按钮被按下
    connect(ui->btn_rollback,&QPushButton::clicked,this,&SysDepartment::onRollbackBtnClicked);  //回滚按钮被按下

    //初始化成员变量
    m_Dialog = nullptr;
    m_mainLayout = nullptr;
    m_btnLayout = nullptr;
}

/**
 * @brief SysDepartment::createDialog
 * @param name 新增，为新增模态框。修改，为修改模态框
 * 功能：创建一个对话框，对话框的复用
 */
void SysDepartment::createDialog(QString name)
{
    if(m_Dialog != nullptr)
    {
        onDiaCanBtnClicked();
    }

    //Qt::WindowCloseButtonHint , 把 ？去掉
    m_Dialog = new QDialog(this,Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    //设置为模态框
    m_Dialog->setModal(true);
    //设置对话框标题
    m_Dialog->setWindowTitle(name);

    m_mainLayout = new QVBoxLayout(m_Dialog);

    m_DMLabel = new QLabel(tr("代码："),m_Dialog);
    m_DMEdit = new QLineEdit(m_Dialog);
    m_DMEdit->setClearButtonEnabled(true);

    m_DescribeLabel = new QLabel(tr("描述："),m_Dialog);
    m_DescribeEdit = new QLineEdit(m_Dialog);
    m_DescribeEdit->setClearButtonEnabled(true);

    m_mainLayout->addWidget(m_DMLabel);
    m_mainLayout->addWidget(m_DMEdit);
    m_mainLayout->addWidget(m_DescribeLabel);
    m_mainLayout->addWidget(m_DescribeEdit);

    m_btnLayout = new QHBoxLayout;

    m_btnLayout->addStretch();

    if(name.compare("新增") == 0)
    {
        m_insertOKBtn = new QPushButton(tr("新增"),m_Dialog);
        connect(m_insertOKBtn,&QPushButton::clicked,this,&SysDepartment::onInsDiaInsBtnClicked);
        m_btnLayout->addWidget(m_insertOKBtn);
    }
    else if(name.compare("修改") == 0)
    {
        m_updateOKBtn = new QPushButton(tr("修改"),m_Dialog);
        connect(m_updateOKBtn,&QPushButton::clicked,this,&SysDepartment::onUpDiaUpdateClicked);
        m_btnLayout->addWidget(m_updateOKBtn);
    }

    m_Cancel = new QPushButton(tr("取消"),m_Dialog);
    connect(m_Cancel,&QPushButton::clicked,this,&SysDepartment::onDiaCanBtnClicked);

    m_btnLayout->addWidget(m_Cancel);

    m_mainLayout->addLayout(m_btnLayout);

    m_Dialog->show();
}

/**
 * @brief SysDepartment::setEnabledInButton
 * @param enable true：启用，false：不启用
 * 功能：启用/禁用提交和回滚按钮
 */
void SysDepartment::setEnabledInButton(bool enable)
{
    ui->btn_submit->setEnabled(enable);
    ui->btn_rollback->setEnabled(enable);
}

/**
 * @brief SysDepartment::onInsertBtnOnClicked
 * 功能：新建按钮被按下，弹出QInputDialog输入数据后，保存近数据库
 */
void SysDepartment::onInsertBtnOnClicked()
{
    createDialog();
}

/**
 * @brief SysDepartment::onUpdateBtnClicked
 * 功能：页面上修改按钮被按下
 */
void SysDepartment::onUpdateBtnClicked()
{
    QModelIndex index = ui->tableView->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::information(this,tr("提示"),
                                 tr("请选中一行后再点击修改"));
        return;
    }

    int depId = index.sibling(index.row(),0).data().toInt();

    QString code = index.sibling(index.row(),1).data().toString();
    QString depName = index.sibling(index.row(),2).data().toString();

    if(depId > 0)
    {
        m_depID = depId;
        createDialog("修改");
        m_DMEdit->setText(code.trimmed());
        m_DescribeEdit->setText(depName.trimmed());
    }
}

/**
 * @brief SysDepartment::InsDiaInsBtnClicked
 * 功能：新建对话框中，新增按钮被按下.新增数据
 */
void SysDepartment::onInsDiaInsBtnClicked()
{
    //获取数据
    //代码
    QString code = m_DMEdit->text().trimmed();
    //部门名称
    QString depName = m_DescribeEdit->text().trimmed();

    //两者不为空时
    if(!code.isEmpty() && !depName.isEmpty())
    {
        //新增
        bool ok = m_service->insertDepData(code,depName);

        if(ok)
        {
            QMessageBox::information(m_Dialog,"新增成功",
                                     tr("成功新增了一条数据"));
            m_service->refreshTable();
        }
        else
        {
            QString text = m_service->getLastError();

            if(text.trimmed().isEmpty())
            {
                text = "该数据已存在，请不要重复新增相同的数据";
            }

            QMessageBox::warning(m_Dialog,"新增失败",
                             text);
        }

        //关闭
        onDiaCanBtnClicked();
    }
    else
    {
        QMessageBox::warning(m_Dialog,"新增失败",
                             tr("请检查数据完整性"));
    }
}

/**
 * @brief SysDepartment::onUpDiaUpdateClicked
 * 功能：修改对话框中，修改按钮被按下
 */
void SysDepartment::onUpDiaUpdateClicked()
{
    //获取数据
    //代码
    QString code = m_DMEdit->text().trimmed();
    //部门名称
    QString depName = m_DescribeEdit->text().trimmed();

    if(m_depID > 0 && !code.isEmpty() && !depName.isEmpty())
    {
        bool ok = m_service->updateDepData(m_depID,code,depName);

        if(ok)
        {
            QMessageBox::information(m_Dialog,"修改成功",
                                     tr("成功修改了一条数据"));
            m_service->refreshTable();
        }
        else
        {
            QString text = m_service->getLastError();
            if(text.trimmed().isEmpty()){
                text = "您输入的数据在数据库中已经存在";
            }
            QMessageBox::warning(m_Dialog,"修改失败",
                                 text);
        }

        onDiaCanBtnClicked();
    }
    else
    {
        QMessageBox::warning(m_Dialog,"修改失败",
                             tr("请检查数据完整性"));
    }
}

/**
 * @brief SysDepartment::onInsDiaCanBtnClicked
 * 功能：对话框中，取消按钮被按下
 */
void SysDepartment::onDiaCanBtnClicked()
{
    if(m_Dialog != nullptr)
    {
        m_Dialog->close();
    }
    delete m_btnLayout;
    delete m_mainLayout;
    delete m_Dialog;
    m_btnLayout = nullptr;
    m_mainLayout = nullptr;
    m_Dialog = nullptr;
}

/**
 * @brief SysDepartment::onDelBtnOnClicked
 * 功能：删除选中的一行数据
 */
void SysDepartment::onDelBtnOnClicked()
{
    QModelIndex index = ui->tableView->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::information(this,tr("提示"),
                                 tr("请选中一行后再点击删除"));
        return;
    }

    int btn = QMessageBox::question(this,tr("提示"),
                                    tr("确定要删除该数据吗？"));

    if(btn == QMessageBox::Yes)
    {
        int depId = index.sibling(index.row(),0).data().toInt();

        if(depId > 0)
        {
            bool ok = m_service->deleteDepData(depId);

            if(ok)
            {
                QMessageBox::information(this,"删除成功",
                                         tr("成功删除了一条数据"));
                m_service->refreshTable();
            }
            else
            {
                QMessageBox::warning(this,"删除失败",
                                     m_service->getLastError());
            }
        }
        else
        {
            QMessageBox::warning(this,"删除失败",
                                 tr("数据异常，请关闭该页面后重试！"));
        }
    }
}

/**
 * @brief SysDepartment::onRefBtnOnClicked
 * 功能：刷新表格
 */
void SysDepartment::onRefBtnOnClicked()
{
    m_service->refreshTable();
}

/**
 * @brief SysDepartment::onSubmitBtnClicked
 * 功能：提交
 */
void SysDepartment::onSubmitBtnClicked()
{
    m_model->submitAll();
    setEnabledInButton(false);
}

/**
 * @brief SysDepartment::onRefreshBtnClicked
 * 功能：回滚
 */
void SysDepartment::onRollbackBtnClicked()
{
    m_model->revertAll();
    setEnabledInButton(false);
}

/**
 * @brief SysDepartment::onModelDataChanged
 * 功能：当表格的model被修改后触发，禁用刷新之前的按钮，启用提交和回滚
 */
void SysDepartment::onModelDataChanged(const QModelIndex &topLeft, const QModelIndex &, const QVector<int> &)
{
    int depID = topLeft.sibling(topLeft.row(),0).data().toInt();
    QString name = topLeft.model()->headerData(topLeft.column(),Qt::Horizontal,Qt::EditRole).toString();

    //获取数据库中存在数目
    int num = m_service->selectDepOwn(depID,name,topLeft.data());
    //获取model中有重复的数目，包含自身
    int matchNum = topLeft.model()->match(topLeft,Qt::DisplayRole,topLeft.data(),2).size();

    //将按钮改为启动
    setEnabledInButton(true);

    if(num > 0 || matchNum > 1)
    {
        m_model->revertRow(topLeft.row());

        QMessageBox::warning(this,"错误",
                             "已存在该数据,请重新修改");

    }
}
