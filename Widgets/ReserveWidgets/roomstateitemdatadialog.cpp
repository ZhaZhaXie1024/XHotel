#include "roomstateitemdatadialog.h"
#include "ui_roomstateitemdatadialog.h"

#include <QMouseEvent>
#include <QDebug>

RoomStateItemDataDialog::RoomStateItemDataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoomStateItemDataDialog)
{
    ui->setupUi(this);
    init();
}

RoomStateItemDataDialog::~RoomStateItemDataDialog()
{
    delete ui;
}

/**
 * @brief RoomStateItemDataDialog::init
 * 初始化
 */
void RoomStateItemDataDialog::init()
{
    setWindowTitle("房间详细信息");
    setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    setMouseTracking(true);
}

/**
 * @brief RoomStateItemDataDialog::setPos
 * @param scenePos
 * 设置位置
 */
void RoomStateItemDataDialog::setPos(QPoint scenePos)
{
    move(scenePos);
}

/**
 * @brief RoomStateItemDataDialog::setData
 * @param room_num
 * @param tower_rang
 * @param tower_seat
 * @param tower_tier
 * @param bed_num
 * @param room_state
 * @param room_type
 * @param room_price
 * 设置数据
 */
void RoomStateItemDataDialog::setData(QString room_num, QString tower_rang, QString tower_seat, QString tower_tier,
                                      QString bed_num, QString room_state, QString room_type, QString room_price,int flage)
{
    ui->room_num->setText(room_num);
    ui->tower_rang->setText(tower_rang);
    ui->tower_seat->setText(tower_seat);
    ui->tower_trie->setText(tower_tier);
    ui->bed_unm->setText(bed_num);
    ui->room_state->setText(room_state);
    ui->room_type->setText(room_type);
    ui->room_pirce->setText(room_price);

    m_flage = flage;

    if(flage == 1)
    {
        setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    }
    else
    {
        setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint|Qt::MSWindowsFixedSizeDialogHint);
    }
}

void RoomStateItemDataDialog::enterEvent(QEvent *)
{
    emit dialogSignal(true);
}

void RoomStateItemDataDialog::leaveEvent(QEvent *)
{
    emit dialogSignal(false);
    if(m_flage == 1)
    {
        this->hide();
    }
}

