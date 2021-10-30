#include "roomfilterdialog.h"
#include "ui_roomfilterdialog.h"

#include <QMessageBox>
#include <QDebug>

#include "Entitys/SystemRoomManageEntitys.h"
#include "Entitys/comboboxentity.h"
#include "Service/comboboxitemservice.h"

RoomFilterDialog::RoomFilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomFilterDialog)
{
    ui->setupUi(this);
    init();
}

RoomFilterDialog::~RoomFilterDialog()
{
    delete ui;
}

/**
 * @brief RoomFilterDialog::init
 * 初始化
 */
void RoomFilterDialog::init()
{
    m_comboBoxService = new ComboboxItemService(this);

    setWindowTitle("房间查询");
    setModal(true);
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::MSWindowsFixedSizeDialogHint);

    initComboBox();

    //查询按钮被按下
    connect(ui->btn_select,&QPushButton::clicked,this,&RoomFilterDialog::onSelectBtnClicked);
    //清空按钮被按下
    connect(ui->btn_clear,&QPushButton::clicked,this,&RoomFilterDialog::onClearBtnClicked);
    //取消按钮被按下
    connect(ui->btn_cancel,&QPushButton::clicked,this,&RoomFilterDialog::onCancelBtnClicked);
}

/**
 * @brief RoomFilterDialog::initComboBox
 * 初始化下拉框
 */
void RoomFilterDialog::initComboBox()
{
    //楼区下拉框
    m_comboBoxService->init("tower_range",QStringList()<< "tower_range_id" << "m_describe");
    QList<ComboBoxSeleteItem> rangeItems = m_comboBoxService->select();
    m_comboBoxService->boundData(ui->com_tower_range,rangeItems);

    //绑定楼座下拉框
    m_comboBoxService->init("tower_seat",QStringList() << "tower_seat_id" << "m_describe",QStringList()<<"tower_range_id");
    QList<ComboBoxSeleteItem> seatItems = m_comboBoxService->select();
    m_comboBoxService->boundData(ui->com_tower_seat,seatItems);

    //房间类型
    m_comboBoxService->init("room_type",QStringList() << "room_type_id" << "m_describe");
    QList<ComboBoxSeleteItem> roomTypeItems = m_comboBoxService->select();
    m_comboBoxService->boundData(ui->com_room_type,roomTypeItems);

    //房间状态
    m_comboBoxService->init("room_state",QStringList() << "room_state_id" << "m_describe");
    QList<ComboBoxSeleteItem> roomStateItems = m_comboBoxService->select();
    m_comboBoxService->boundData(ui->com_room_state,roomStateItems);
}

/**
 * @brief RoomFilterDialog::onSelectBtnClicked
 * 查询按钮被按下,发送信号通知room查询数据
 */
void RoomFilterDialog::onSelectBtnClicked()
{
    //获取数据
    int tower_range_id = ui->com_tower_range->currentData().toInt();
    int tower_seat_id = ui->com_tower_seat->currentData().toInt();
    int tower_tier = ui->spin_tier->value();
    QString room_num = ui->edit_room_num->text().trimmed();
    int room_type_id = ui->com_room_type->currentData().toInt();
    int room_state_id = ui->com_room_state->currentData().toInt();

    QVariantMap values;
    values.insert("tower_range_id",tower_range_id);
    values.insert("tower_seat_id",tower_seat_id);
    values.insert("tower_tier",tower_tier);
    values.insert("room_num",room_num);
    values.insert("room_type_id",room_type_id);
    values.insert("room_state_id",room_state_id);

    //将数据为空的剔除
    QMap<QString,QVariant>::iterator ite;
    for(ite = values.begin(); ite != values.end(); ite++)
    {
        QVariant value = ite.value();

        QVariant::Type type = value.type();

        if((type == QVariant::Int && value.toInt() <= 0) ||
           (type == QVariant::String && value.toString().isEmpty()))
        {
            values.erase(ite);
            continue;
        }
    }

    //发送
    emit select(values);

    onCancelBtnClicked();
}

/**
 * @brief RoomFilterDialog::onClearBtnClicked
 * 清空按钮被按下
 */
void RoomFilterDialog::onClearBtnClicked()
{
    ui->com_tower_range->setCurrentIndex(0);
    ui->com_tower_seat->setCurrentIndex(0);
    ui->com_room_type->setCurrentIndex(0);
    ui->com_room_state->setCurrentIndex(0);

    ui->spin_tier->setValue(0);
    ui->edit_room_num->clear();
}

/**
 * @brief RoomFilterDialog::onCancelBtnClicked
 * 关闭Dialog
 */
void RoomFilterDialog::onCancelBtnClicked()
{
    hide();
}
