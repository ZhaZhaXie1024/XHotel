#include "reserveroom.h"
#include "ui_reserveroom.h"

#include <QListView>
#include <QSqlQueryModel>

#include "Service/ReserveService/reserveservice.h"
#include "Entitys/ReserveInfo.h"
#include "Service/comboboxitemservice.h"
#include "Entitys/comboboxentity.h"

#include <QDialog>
#include <QHBoxLayout>
#include "Widgets/ReserveWidgets/roomstatemap.h"

#include "Widgets/ReserveWidgets/reserveroomselectdialog.h"

//#include <QDate>
#include <QMessageBox>
#include <QDebug>
#include <QHeaderView>

ReserveRoom::ReserveRoom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReserveRoom),
    this_index(0),
    this_showNum(15)
{
    ui->setupUi(this);
    init();
}

ReserveRoom::~ReserveRoom()
{
    delete ui;
}

/**
 * @brief ReserveRoom::init
 * 初始化
 */
void ReserveRoom::init()
{
    m_service = new ReserveService(this);
//    m_service->setLimit(this_index,this_showNum);
    m_model = new QSqlQueryModel(this);
    m_sql = m_service->select();
    m_model->setQuery(m_sql,m_service->getDatabase());

    m_room_id = 0;
    m_isUpdate = false;

    //初始化model样式
    initModel();

    //初始化房态对话框
    initRoomStateMapDialog();

    //初始化样式
    initStyle();

    //全局信号和槽-----
    initConnect();
}

/**
 * @brief ReserveRoom::initModel
 * 初始化model样式
 */
void ReserveRoom::initModel()
{
    m_model->setHeaderData(4,Qt::Horizontal,"订单状态");
    m_model->setHeaderData(5,Qt::Horizontal,"主订单号");
    m_model->setHeaderData(6,Qt::Horizontal,"客户名称");
    m_model->setHeaderData(7,Qt::Horizontal,"房间号");
    m_model->setHeaderData(8,Qt::Horizontal,"预约入住时间");
    m_model->setHeaderData(9,Qt::Horizontal,"预约天数");

    ui->tableView->setModel(m_model);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);
}

/**
 * @brief ReserveRoom::initRoomStateMapDialog
 * 初始化房态对话框
 */
void ReserveRoom::initRoomStateMapDialog()
{
    m_dialog = new QDialog(this,Qt::Dialog|Qt::WindowCloseButtonHint);
    m_mianLayout = new QHBoxLayout(m_dialog);
    m_roomStateMap = new RoomStateMap(m_dialog,0);
    m_mianLayout->addWidget(m_roomStateMap);

    m_dialog->setLayout(m_mianLayout);

    //获取房间ID
    connect(m_roomStateMap,&RoomStateMap::reserveWidgetRoom,this,&ReserveRoom::setRoom);
}

/**
 * @brief ReserveRoom::initStyle
 * 初始化样式
 */
void ReserveRoom::initStyle()
{
    m_selectDialog = nullptr;

    //隐藏取消修改/解锁按钮
    ui->btn_unLock->hide();

    //combobox样式需要
    ui->com_sex->setView(new QListView());

    //初始化装态下拉框
    ComboboxItemService comService;
    QList<int> sqlLinkSymbol;
    sqlLinkSymbol << 1 << 0;
    comService.init("bill_state_type",QStringList()<<"bill_state_type_id"<<"bill_state",QStringList()<<"code"<<"code",sqlLinkSymbol);
    comService.boundData(ui->com_billState,comService.select(QVariantList()<<"YY"<<"QX"));

    //生成订单号
    ui->label_billNum->setText(m_service->selectBillNum());

    //更改入住日期两个控件的最小值和最大值
    ui->dateEdit_in->setDateRange(QDate::currentDate(),QDate::currentDate().addDays(30));
    ui->dateEdit_out->setDateRange(QDate::currentDate().addDays(1),QDate::currentDate().addDays(30));
}

/**
 * @brief ReserveRoom::initConnect
 * 初始化全局信号和槽
 */
void ReserveRoom::initConnect()
{
    //查询按钮被按下
    connect(ui->btn_select,&QPushButton::clicked,this,&ReserveRoom::onSelectBtnClicked);

    //选择房间
    connect(ui->btn_selectRoom,&QPushButton::clicked,this,&ReserveRoom::onSelectRoomBtnClicked);
    //新增/预定房间
    connect(ui->btn_reserve,&QPushButton::clicked,this,&ReserveRoom::onInsertBtnClicked);

    //点击表格某一行，进行回填
    connect(ui->tableView,&QTableView::doubleClicked,this,&ReserveRoom::onTableViewDouClicked);
    //取消修改/解锁，按钮被按下
    connect(ui->btn_unLock,&QPushButton::clicked,this,&ReserveRoom::onUnLockBtnClicked);
    //点击修改按钮
    connect(ui->btn_update,&QPushButton::clicked,this,&ReserveRoom::onUpdateBtnClicked);

    //恢复输入控件初始状态
    connect(ui->btn_clear,&QPushButton::clicked,this,&ReserveRoom::onClearBtnClicked);
}

/**
 * @brief ReserveRoom::setRoomID
 * @param room_id
 * @param room_num
 * 设置房间ID和房间号码（显示）
 */
void ReserveRoom::setRoom(int room_id, QString room_num,int room_state_id)
{
    if(room_state_id != m_service->selectLeisureRoomStateID())
    {
        QMessageBox::information(this,"提示","当前房间无法进行预定，请选中空闲的房间预定");
        return;
    }

    ui->label_roomNum->setText(room_num);
    m_room_id = room_id;

    //当双击获取房间ID后，隐藏对话框
    if(!m_dialog->isHidden())
    {
        m_dialog->hide();
    }
}

/**
 * @brief ReserveRoom::setOperatorID
 * @param operator_id
 * 设置操作员ID
 */
void ReserveRoom::setOperatorID(int operator_id)
{
    m_operator_id = operator_id;
}

/**
 * @brief ReserveRoom::select
 * 查询
 */
void ReserveRoom::select()
{
    m_model->setQuery(m_sql,m_service->getDatabase());
}

/**
 * @brief ReserveRoom::onSelectBtnClicked
 * 查询按钮被按下
 */
void ReserveRoom::onSelectBtnClicked()
{
    //打开查询对话框
    if(m_selectDialog == nullptr)
    {
        m_selectDialog = new ReserveRoomSelectDialog(this);
        //查询对话框查询按钮被按下
        connect(m_selectDialog,&ReserveRoomSelectDialog::select,this,&ReserveRoom::onSelectDialogSelectBtnClicked);
    }
    m_selectDialog->show();
}

/**
 * @brief ReserveRoom::onSelectDialogSelectBtnClicked
 * @param name
 * @param phone
 * @param billNum
 * @param date
 * @param stateID
 * 查询对话框查询按钮被按下
 */
void ReserveRoom::onSelectDialogSelectBtnClicked(QString name, QString phone, QString billNum, QDate date, int stateID)
{
    QStringList keys;
    QStringList values;

    if(!name.isEmpty())
    {
        keys.append(" client.name LIKE '%");
        values.append(name+"%'");
    }

    if(!phone.isEmpty())
    {
        keys.append(" client.phone = ");
        values.append(phone);
    }

    if(!billNum.isEmpty())
    {
        keys.append(" main_bill.mian_bill_num LIKE '%");
        values.append(billNum+"%'");
    }

    if(!date.isNull())
    {
        keys.append(" client_history.appointment_date = ");
        values.append('\'' + date.toString("yyyy-MM-dd") + '\'');
    }

    if(stateID > 0)
    {
        keys.append(" main_bill.bill_state_type_id = ");
        values.append(QString::number(stateID));
    }

    m_sql = m_service->select(keys,values);

    select();
}

/**
 * @brief ReserveRoom::onSelectRoomBtnClicked
 * 选择房间被按下
 */
void ReserveRoom::onSelectRoomBtnClicked()
{
    //设置只查询空闲的房间
    m_roomStateMap->onGreenActTriggered();
    //最大化显示
    m_dialog->showMaximized();
}

/**
 * @brief ReserveRoom::onInsertBtnClicked
 * 新增按钮被按下
 */
void ReserveRoom::onInsertBtnClicked()
{
    //获取数据
    QString name = ui->edit_name->text().trimmed();
    QString idCard = ui->edit_idcard->text().trimmed();
    QString phone = ui->edit_phone->text().trimmed();

    QString sex = ui->com_sex->currentText();
    int billStateID = ui->com_billState->currentData().toInt();

    QDate clientDate = ui->dateEdit_clientDay->date();
    QDate inDate = ui->dateEdit_in->date();
    QDate outDate = ui->dateEdit_out->date();

    QString billNum = ui->label_billNum->text().trimmed();

    //验证数据
    if(name.isEmpty() || idCard.isEmpty() || sex.isEmpty() || billStateID <= 0 ||
       phone.isEmpty() || clientDate.isNull() || inDate.isNull() ||
        outDate.isNull() || billNum.isEmpty())
    {
        QMessageBox::warning(this,"新增警告","数据缺失或无效！");
        return;
    }

    if(m_room_id <= 0)
    {
        QMessageBox::warning(this,"新增警告","请为用户选择一间房间，进行预约！");
        return;
    }

    if(m_operator_id <= 0)
    {
        QMessageBox::warning(this,"新增警告","数据错误！关闭页面重新打开页面重试！");
        return;
    }

    int day = inDate.daysTo(outDate);

    if(day <= 0)
    {
        QMessageBox::warning(this,"新增警告","日期错误");
        return;
    }

    //组装数据
    ReserveInfo info;
    info.name = name;
    info.certificate_num = idCard;
    info.iphone = phone;
    info.sex = sex;
    info.bill_state_type_id = billStateID;
    info.time = clientDate;
    info.appointment_date = inDate;
    info.appointment_day = day;
    info.room_id = m_room_id;
    info.operator_id = m_operator_id;
    info.mian_bill_num = billNum;

    //新增
    if(m_service->insert(info))
    {
        QMessageBox::information(this,"新增成功","成功新增一条数据！");
        onClearBtnClicked();
        m_room_id = 0;

        ui->label_billNum->setText(m_service->selectBillNum());
        ui->label_roomNum->setText("无");

        //刷新数据
        select();
    }
    else
    {
        QString str = m_service->getLastError().trimmed();

        if(str.isEmpty())
        {
            str = "新增失败，请稍后重试！";
        }
        QMessageBox::critical(this,"新增错误",str);
    }
}

/**
 * @brief ReserveRoom::onTableViewClicked
 * @param index
 * 点击表格某一行，进行回填
 */
void ReserveRoom::onTableViewDouClicked(const QModelIndex &index)
{
    if(!index.isValid())
    {
        return;
    }

    m_isUpdate = true;

    //显示取消修改/解锁按钮
    ui->btn_unLock->show();

    //锁定其它不能修改的编辑框等
    ui->edit_name->setEnabled(false);
    ui->edit_idcard->setEnabled(false);
    ui->edit_phone->setEnabled(false);
    ui->com_sex->setEnabled(false);
    ui->dateEdit_clientDay->setEnabled(false);

    ui->btn_select->setEnabled(false);
    ui->btn_reserve->setEnabled(false);

    //清除预定入住日期in的最小值
    ui->dateEdit_in->setMinimumDate(index.sibling(index.row(),8).data().toDate());

    //回填
    //订单状态
    ui->com_billState->setCurrentText(index.sibling(index.row(),4).data().toString());
    //订单编号
    ui->label_billNum->setText(index.sibling(index.row(),5).data().toString());
    //房间号
    ui->label_roomNum->setText(index.sibling(index.row(),7).data().toString());
    //预定入住日期in
    ui->dateEdit_in->setDate(index.sibling(index.row(),8).data().toDate());
    //预定入住日期out
    ui->dateEdit_out->setDate(index.sibling(index.row(),8).data().toDate().addDays(index.sibling(index.row(),9).data().toInt()));
    //房间ID
    m_room_id = index.sibling(index.row(),3).data().toInt();
    //客史ID
    m_clientHistoryID = index.sibling(index.row(),2).data().toInt();
    //订单ID
    m_billID = index.sibling(index.row(),0).data().toInt();
}

/**
 * @brief ReserveRoom::onUpdateBtnClicked
 * 修改按钮被按下
 */
void ReserveRoom::onUpdateBtnClicked()
{
    if(!m_isUpdate)
    {
        QMessageBox::information(this,"提示","请选中一行后再进行修改");
        return;
    }

    //获取数据
    QDate inDate = ui->dateEdit_in->date();
    QDate outDate = ui->dateEdit_out->date();

    int day = inDate.daysTo(outDate);

    int billStateID = ui->com_billState->currentData().toInt();

    if(day <= 0 || m_room_id <= 0 || billStateID <= 0 || m_billID <= 0 || m_clientHistoryID <= 0 || m_operator_id <= 0)
    {
        QMessageBox::warning(this,"警告","数据错误，请从新选择一行重试！");
        onUnLockBtnClicked();
        return;
    }

    //开始修改
    ReserveInfo info;
    info.appointment_date = inDate;
    info.appointment_day = day;
    info.room_id = m_room_id;
    info.bill_state_type_id = billStateID;
    info.client_history_id = m_clientHistoryID;
    info.main_bill_id = m_billID;
    info.operator_id = m_operator_id;

    if(m_service->update(info))
    {
        //修改成功
        QMessageBox::information(this,"修改提示","修改成功!");
        //刷新数据
        select();
    }
    else
    {
        QString str = m_service->getLastError().trimmed();

        if(str.isEmpty())
        {
            str = "修改失败，请稍后重试！";
        }
        QMessageBox::critical(this,"修改错误",str);
    }

    onUnLockBtnClicked();
}

/**
 * @brief ReserveRoom::onUnLockBtnClicked
 * 取消修改/解锁
 */
void ReserveRoom::onUnLockBtnClicked()
{
    ui->edit_name->setEnabled(true);
    ui->edit_idcard->setEnabled(true);
    ui->edit_phone->setEnabled(true);
    ui->com_sex->setEnabled(true);
    ui->dateEdit_clientDay->setEnabled(true);

    ui->btn_select->setEnabled(true);
    ui->btn_reserve->setEnabled(true);

    //设置预定入住日期in的最小值
    ui->dateEdit_in->setMinimumDate(QDate::currentDate());

    //修改房间ID
    m_room_id = 0;

    //房间号码
    ui->label_roomNum->setText("无");

    //当前订单编号
    ui->label_billNum->setText(m_service->selectBillNum());

    m_isUpdate = false;

    //隐藏
    ui->btn_unLock->hide();
}

/**
 * @brief ReserveRoom::onClearBtnClicked
 * 清空按钮被按下
 */
void ReserveRoom::onClearBtnClicked()
{
    ui->edit_name->clear();
    ui->edit_idcard->clear();
    ui->edit_phone->clear();
    ui->com_sex->setCurrentIndex(0);
    ui->com_billState->setCurrentIndex(0);
}



