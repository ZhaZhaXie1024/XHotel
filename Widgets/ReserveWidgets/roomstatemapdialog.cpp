#include "roomstatemapdialog.h"
#include "ui_roomstatemapdialog.h"

#include "Entitys/comboboxentity.h"
#include "Service/comboboxitemservice.h"

RoomStateMapDialog::RoomStateMapDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomStateMapDialog)
{
    ui->setupUi(this);
    init();
    connect(ui->btn_select,&QPushButton::clicked,this,&RoomStateMapDialog::onSelectBtnClicked);
    connect(ui->btn_clear,&QPushButton::clicked,this,&RoomStateMapDialog::onClearBtnClicked);
    connect(ui->btn_cancel,&QPushButton::clicked,this,&RoomStateMapDialog::onCancelBtnClicked);
}

RoomStateMapDialog::~RoomStateMapDialog()
{
    delete ui;
}

/**
 * @brief RoomStateMapDialog::init
 * 为两个文本框添加输入限制,绑定下拉框
 */
void RoomStateMapDialog::init()
{
    setWindowTitle("查询");

    //限制只能输入数字
    QIntValidator intValidator;
    intValidator.setRange(1,10);

    QIntValidator intValidator1;
    intValidator1.setRange(100,100000);

    ui->bed_num->setValidator(&intValidator);
    ui->room_price->setValidator(&intValidator1);
    ui->bed_num->setPlaceholderText("1~10");
    ui->room_price->setPlaceholderText("100~100000,且搜索范围在这个值之下");

    ComboboxItemService comService;

    //初始化房区下拉框
    comService.init("tower_range",QStringList()<<"tower_range_id"<<"m_describe");
    comService.boundData(ui->com_tower_range,comService.select());

    //初始化楼座下拉框
    comService.init("tower_seat",QStringList()<<"tower_seat_id"<<"m_describe");
    comService.boundData(ui->com_tower_seat,comService.select());

    //初始化房间类型下拉框
    comService.init("room_type",QStringList()<<"room_type_id"<<"m_describe");
    comService.boundData(ui->com_room_type,comService.select());

    //初始化房间状态下拉框
    comService.init("room_state",QStringList()<<"room_state_id"<<"m_describe");
    comService.boundData(ui->com_room_state,comService.select());
}

/**
 * @brief RoomStateMapDialog::onSelectBtnClicked
 * 查询按钮被按下
 */
void RoomStateMapDialog::onSelectBtnClicked()
{
    //获取数据
    int trId = ui->com_tower_range->currentData().toInt();
    int tsId = ui->com_tower_seat->currentData().toInt();
    int typeId = ui->com_room_type->currentData().toInt();
    int stateId = ui->com_room_state->currentData().toInt();
    int bedNum = ui->bed_num->text().toInt();
    int price = ui->room_price->text().toInt();

    emit selectKeyValue(trId,tsId,typeId,stateId,bedNum,price);

    close();
}

/**
 * @brief RoomStateMapDialog::onClearBtnClicked
 * 清空按钮被按下
 */
void RoomStateMapDialog::onClearBtnClicked()
{
    ui->bed_num->clear();
    ui->room_price->clear();
    ui->com_tower_range->setCurrentIndex(0);
    ui->com_tower_seat->setCurrentIndex(0);
    ui->com_room_state->setCurrentIndex(0);
    ui->com_room_type->setCurrentIndex(0);
}

/**
 * @brief RoomStateMapDialog::onCancelBtnClicked
 * 取消按钮被按下
 */
void RoomStateMapDialog::onCancelBtnClicked()
{
    this->close();
}
