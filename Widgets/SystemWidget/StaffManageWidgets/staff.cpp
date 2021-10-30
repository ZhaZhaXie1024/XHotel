#include "staff.h"
#include "ui_staff.h"

#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>


#include <Widgets/SystemWidget/StaffManageWidgets/staffdialog.h>
#include <Service/SystemService/StaffManageService/staffservice.h>
#include "Delegate/usercomboboxdeletage.h"
#include "Delegate/userlineeditdelegate.h"
#include "Delegate/usercalendardelegate.h"

Staff::Staff(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Staff),
    m_dialog(nullptr)
{
    ui->setupUi(this);
    init();
}

Staff::~Staff()
{
    delete ui;
}

/**
 * @brief Staff::resizeEvent
 * @param event
 * 初始化表格数据，以及动态根据页面大小进行查询条数
 */
void Staff::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event) ;
    ui->retranslateUi(this);

    int width = ui->tableView->width();
    int height = ui->tableView->height()-40;

    if(m_col > 0)
    {
        ui->tableView->horizontalHeader()->setDefaultSectionSize(width/m_col - 1);
        int row = height/40;
        if(m_service != nullptr)
        {
            //提供index，和showNum，开始加载数据
            m_service->setLimit(0,row);
            m_service->select();

            QAbstractItemModel * model = m_service->getModel();
            ui->tableView->setModel(nullptr);
            ui->tableView->setModel(model);
            ui->tableView->hideColumn(0);

            //设置委托
            ui->tableView->setItemDelegateForColumn(3,m_depComboBoxDeletage);
            ui->tableView->setItemDelegateForColumn(2,m_sexComboBoxDelegate);
            ui->tableView->setItemDelegateForColumn(4,m_posComboBoxDelegate);
            ui->tableView->setItemDelegateForColumn(5,m_phoneEditDelegate);
            ui->tableView->setItemDelegateForColumn(6,m_idCardEditDelegate);
            ui->tableView->setItemDelegateForColumn(7,m_dateEditDelegate);

            m_thisPage = 1;
        }
    }
}

/**
 * @brief Staff::init
 * 功能：初始化
 */
void Staff::init()
{
    m_service = new StaffService(this);
    //初始化
    m_col = m_service->getCol() - 1;
    m_thisPage = 1;
    m_staffID = 0;

    //设置当前页的输入范围 QIntValidator整形的过滤器
    QIntValidator *intValidator = new QIntValidator;
    intValidator->setRange(0, 1000);
    ui->edit_thisPage->setValidator(intValidator);

    initDalegate();

    initConnect();
}

/**
 * @brief Staff::initConnect
 * 初始化信号和槽的连接
 */
void Staff::initConnect()
{
    //设置总页数
    connect(m_service,&StaffService::updataUiPageCount,this,&Staff::setPageCount);
    //显示错误提示
    connect(m_service,&StaffService::pageError,this,&Staff::showPageError);
    //跳转页
    connect(ui->edit_thisPage,&QLineEdit::editingFinished,this,&Staff::onThisPageEditOk);
    //首页
    connect(ui->btn_first,&QPushButton::clicked,this,&Staff::firstPage);
    //尾页
    connect(ui->btn_last,&QPushButton::clicked,this,&Staff::lastPage);
    //上一页
    connect(ui->btn_previous,&QPushButton::clicked,this,&Staff::previousPage);
    //下一页
    connect(ui->btn_next,&QPushButton::clicked,this,&Staff::nextPage);
    //刷新
    connect(ui->btn_refresh,&QPushButton::clicked,this,&Staff::onRefreshBtnClicked);
    //新增
    connect(ui->btn_insert,&QPushButton::clicked,this,&Staff::insertBtnClicked);
    //修改
    connect(ui->btn_updata,&QPushButton::clicked,this,&Staff::updateBtnClicked);
    //删除
    connect(ui->btn_delete,&QPushButton::clicked,this,&Staff::deleteData);
    //编辑表格
    connect(ui->tableView,&QTableView::doubleClicked,this,&Staff::onTableViewChanged);
    //提交
    connect(ui->btn_commit,&QPushButton::clicked,this,&Staff::submitData);
    //回滚
    connect(ui->btn_rollBack,&QPushButton::clicked,this,&Staff::rollBackData);
}

/**
 * @brief Staff::initDalegate
 * 初始化委托
 */
void Staff::initDalegate()
{
    //创建委托
    //部门
    m_depComboBoxDeletage = new UserComboBoxDeletage(m_service->comboBoxData("department",
                                                                             QStringList() << "department_id"
                                                                             << "m_describe"),this);

    //性别
    QList<QVariantMap> mapList;
    QVariantMap map;
    map.insert("男","男");
    QVariantMap map1;
    map1.insert("女","女");
    mapList.append(map);
    mapList.append(map1);
    m_sexComboBoxDelegate = new UserComboBoxDeletage(mapList,this);

    //职业
    m_posComboBoxDelegate = new UserComboBoxDeletage(m_service->comboBoxData("staff_position",
                                                                             QStringList() << "staff_position_id"
                                                                             << "m_describe"),this);

    //电话
    QRegExp reg("^1[0-9]{10}$");
    QRegExpValidator *phoneVal = new QRegExpValidator(reg,this);
    m_phoneEditDelegate = new UserLineEditDelegate(11,phoneVal,this);

    //身份证
    m_idCardEditDelegate = new UserLineEditDelegate(18,this);

    //入职日期
    m_dateEditDelegate = new UserCalendarDelegate(this);
}

/**
 * @brief Staff::createrDialog
 * @param name dialog名
 * 功能：创建对话框（复用）
 */
void Staff::createrDialog(QString name)
{
    if(m_dialog == nullptr)
    {
        m_dialog = new StaffDialog(name,this);

        connect(m_dialog,&StaffDialog::insertBtnOnClicked,this,&Staff::insertData);
        connect(m_dialog,&StaffDialog::updateBtnOnClicked,this,&Staff::updateData);
        connect(this,&Staff::upDateBackfillData,m_dialog,&StaffDialog::backfillData);
    }
    else
    {
        m_dialog->setName(name);
    }

    m_dialog->show();
}

/**
 * @brief Staff::setPageCount
 * @param num
 * 设置总页数
 */
void Staff::setPageCount(int num)
{
    ui->label_pageCount->setText(QString::number(num));
}

/**
 * @brief Staff::showPageError
 * @param errorStr
 * 显示分页异常
 */
void Staff::showPageError(QString errorStr)
{
    QMessageBox::information(this,"提示",errorStr);
}


/**
 * @brief Staff::onThisPageEditOk
 * 获取当前页
 */
void Staff::onThisPageEditOk()
{
    int page = ui->edit_thisPage->text().toInt();
    if(m_service->setPage(page))
    {
        ui->tableView->update();
        m_thisPage = page;
    }
    else
    {
        ui->edit_thisPage->setText(QString::number(m_thisPage));
    }
}

/**
 * @brief Staff::onRefreshBtnClicked
 * 刷新数据
 */
void Staff::onRefreshBtnClicked()
{
    ui->tableView->clearSelection();
    m_service->select();
    ui->tableView->update();
}

/**
 * @brief Staff::insertRow
 * 新增,对话框
 */
void Staff::insertBtnClicked()
{
    createrDialog();
}

/**
 * @brief Staff::insertData
 * @param staffName
 * @param phone
 * @param idCard
 * @param sex
 * @param depID
 * @param posID
 * 新增
 */
void Staff::insertData(QString staffName, QString phone, QString idCard, QString sex, int depID, int posID,QDate date)
{
    bool isOK = m_service->insertData(staffName,phone,idCard,sex,depID,posID,date);

    if(isOK)
    {
        QMessageBox::information(this,"新增","新增成功");
        onRefreshBtnClicked();
        m_dialog->close();
    }
    else
    {
        qDebug() << "Staff::insertData error: " << m_service->getLastError();
        QMessageBox::warning(this,"新增","新增失败，请稍后重试！");
        m_dialog->close();
    }
}

/**
 * @brief Staff::updateBtnClicked
 * 修改,对话框
 */
void Staff::updateBtnClicked()
{
    m_staffID = 0;

    QModelIndex index = ui->tableView->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::warning(this,"修改","请选中一行后再点击修改！");
        return;
    }

    m_staffID = index.sibling(index.row(),0).data().toInt();

    QString name = index.sibling(index.row(),1).data().toString();
    QString sex = index.sibling(index.row(),2).data().toString();
    QString dep = index.sibling(index.row(),3).data().toString();
    QString pos = index.sibling(index.row(),4).data().toString();
    QString phone = index.sibling(index.row(),5).data().toString();
    QString idCard = index.sibling(index.row(),6).data().toString();
    QDate date = index.sibling(index.row(),7).data().toDate();

    if(m_staffID > 0)
    {
        createrDialog("修改");
        emit upDateBackfillData(name,sex,phone,dep,pos,idCard,date);
    }
    else
    {
        QMessageBox::warning(this,"修改","修改失败，获取数据异常！");
    }
}

/**
 * @brief Staff::updateData
 * @param staffID
 * @param staffName
 * @param phone
 * @param idCard
 * @param sex
 * @param depID
 * @param posID
 * 修改
 */
void Staff::updateData(QString staffName, QString phone, QString idCard, QString sex, int depID, int posID,QDate date)
{
    bool isOK = m_service->updateData(m_staffID,staffName,phone,idCard,sex,depID,posID,date);

    if(isOK)
    {
        QMessageBox::information(this,"修改","修改成功");
        onRefreshBtnClicked();
        m_dialog->close();
    }
    else
    {
        qDebug() << "Staff::updateData error: " << m_service->getLastError();
        QMessageBox::warning(this,"修改","修改失败，请稍后重试！");
        m_dialog->close();
    }
}

/**
 * @brief Staff::deleteData
 * @param staffID
 * 删除
 */
void Staff::deleteData()
{
    m_staffID = 0;

    QModelIndex index = ui->tableView->currentIndex();

    if(!index.isValid())
    {
        QMessageBox::warning(this,"删除","请选中一行后再点击删除！");
        return;
    }

    m_staffID = index.sibling(index.row(),0).data().toInt();

    int btn = QMessageBox::question(this,"删除","确定要删除吗？");

    if(m_staffID > 0 && btn == QMessageBox::Yes)
    {
        bool isOK = m_service->deleteData(m_staffID);

        if(isOK)
        {
            QMessageBox::information(this,"删除","删除成功");
            onRefreshBtnClicked();
        }
        else
        {
            qDebug() << "Staff::updateData error: " << m_service->getLastError();
            QMessageBox::warning(this,"删除","删除失败，请稍后重试！");
        }
    }
    else
    {
        QMessageBox::warning(this,"删除","删除失败，获取数据异常！");
    }
}

/**
* @brief Staff::onTableViewChanged
* 表格开始编辑，启动按钮
*/
void Staff::onTableViewChanged(const QModelIndex &)
{
    ui->btn_commit->setEnabled(true);
    ui->btn_rollBack->setEnabled(true);

    ui->btn_last->setEnabled(false);
    ui->btn_first->setEnabled(false);
    ui->btn_previous->setEnabled(false);
    ui->btn_next->setEnabled(false);
    ui->edit_thisPage->setEnabled(false);

    ui->btn_insert->setEnabled(false);
    ui->btn_updata->setEnabled(false);
    ui->btn_delete->setEnabled(false);
    ui->btn_refresh->setEnabled(false);
}

/**
 * @brief Staff::submitData
 * 提交
 */
void Staff::submitData()
{
    ui->btn_commit->setEnabled(false);
    ui->btn_rollBack->setEnabled(false);

    ui->btn_last->setEnabled(true);
    ui->btn_first->setEnabled(true);
    ui->btn_previous->setEnabled(true);
    ui->btn_next->setEnabled(true);
    ui->edit_thisPage->setEnabled(true);

    ui->btn_insert->setEnabled(true);
    ui->btn_updata->setEnabled(true);
    ui->btn_delete->setEnabled(true);
    ui->btn_refresh->setEnabled(true);

    int i = m_service->commiteData();

    QString str = QString("修改成功的条数为 %1 ").arg(i);

    QMessageBox::information(this,"修改",str);
}

/**
 * @brief Staff::rollBackData
 * 回滚
 */
void Staff::rollBackData()
{
    ui->btn_commit->setEnabled(false);
    ui->btn_rollBack->setEnabled(false);

    ui->btn_last->setEnabled(true);
    ui->btn_first->setEnabled(true);
    ui->btn_previous->setEnabled(true);
    ui->btn_next->setEnabled(true);
    ui->edit_thisPage->setEnabled(true);

    ui->btn_insert->setEnabled(true);
    ui->btn_updata->setEnabled(true);
    ui->btn_delete->setEnabled(true);
    ui->btn_refresh->setEnabled(true);

    m_service->backRoll();
}

/**
 * @brief Staff::previousPage
 * 上一页
 */
void Staff::previousPage()
{
    int page = m_thisPage - 1;
    if(m_service->setPage(page))
    {
        ui->tableView->update();
        m_thisPage = page;
        ui->edit_thisPage->setText(QString::number(m_thisPage));
    }
}

/**
 * @brief Staff::nextPage
 * 下一页
 */
void Staff::nextPage()
{
    int page = m_thisPage + 1;
    if(m_service->setPage(page))
    {
        ui->tableView->update();
        m_thisPage = page;
        ui->edit_thisPage->setText(QString::number(m_thisPage));
    }
}

/**
 * @brief Staff::firstPage
 * 首页
 */
void Staff::firstPage()
{
    if(m_service->setPage(1))
    {
        ui->tableView->update();
        m_thisPage = 1;
        ui->edit_thisPage->setText(QString::number(m_thisPage));
    }
}

/**
 * @brief Staff::lastPage
 * 尾页
 */
void Staff::lastPage()
{
    int lastPage = ui->label_pageCount->text().toInt();

    if(m_service->setPage(lastPage))
    {
        ui->tableView->update();
        m_thisPage = lastPage;
        ui->edit_thisPage->setText(QString::number(m_thisPage));
    }
}
