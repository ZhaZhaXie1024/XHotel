#include "roomstateitem.h"

#include <QPainter>
#include <QFont>
#include <QDebug>
#include <QTimer>
#include <QMenu>
#include <QAction>

#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>

RoomStateItem::RoomStateItem(RoomData data,int width,int height,int openFlag,QGraphicsItem *parent)
    : QGraphicsObject(parent),
      m_data(data),
      m_width(width),
      m_height(height),
      m_openFlag(openFlag)
{
    init();
}

RoomStateItem::~RoomStateItem()
{
    delete m_menu;
    m_menu = nullptr;
}

void RoomStateItem::init()
{
    //初始化颜色
    switch (m_data.room_state_id) {
    case 1:
        m_color.setNamedColor("#FCD913");       //黄色，清理中
        break;
    case 2:
        m_color.setNamedColor("#F56C6C");       //红色，维修
        break;
    case 3:
        m_color.setNamedColor("#409EFF");       //蓝色，预约
        break;
    case 4:
        m_color.setNamedColor("#909399");       //灰色，入住
        break;
    case 5:
        m_color.setNamedColor("#FCD913");       //黄色，待清理
        break;
    case 6:
        m_color.setNamedColor("#67C23A");       //绿色，空闲
        break;
    }

    //初始化
    m_isEnter = false;
    m_hoverColorStr = "#cccccc";
    //相当于开启鼠标跟踪
    setAcceptHoverEvents(true);
    //创建定时器
    m_timer = new QTimer(this);
    connect(m_timer,&QTimer::timeout,this,&RoomStateItem::onTimeOutEmitData);

    m_menu = nullptr;
    m_actReserve = nullptr;
    m_actOpenRoom = nullptr;
    m_actSettlement = nullptr;
    m_actDetail = nullptr;
}

/**
 * @brief RoomStateItem::onTimeOutEmitData
 * 定时器溢出，发送数据
 */
void RoomStateItem::onTimeOutEmitData()
{
    emit itemData(m_data,m_scenePos,1);
    m_timer->stop();
}

/**
 * @brief RoomStateItem::onDetailActClicked
 * act显示详情槽函数
 */
void RoomStateItem::onDetailActClicked()
{
    emit itemData(m_data,m_scenePos,2);
    m_timer->stop();
}

/**
 * @brief RoomStateItem::onReserveActClicked
 * act预定槽函数
 */
void RoomStateItem::onReserveActClicked()
{
    emit reserveRoom(m_data.room_id,m_data.room_num,m_data.room_state_id);
}

/**
 * @brief RoomStateItem::boundingRect
 * @return 绘制区域
 * 绘制区域
 */
QRectF RoomStateItem::boundingRect() const
{
    int penWidth = 2;
    int w_radius = m_width/2;
    int h_radius = m_height/2;
    return QRectF(-w_radius / penWidth/2,-h_radius / penWidth/2,
                  m_width+penWidth,m_height+penWidth);
}

/**
 * @brief RoomStateItem::paint
 * @param painter
 * @param option
 * @param widget
 * 绘制
 */
void RoomStateItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //旧笔和笔刷
    QPen oldPen = painter->pen();
    QBrush oldBrush = painter->brush();
    QFont oldFont = painter->font();

    //画笔：透明
    QPen pen(Qt::transparent);
    //画刷：
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    //字体
    QFont font = painter->font();

    if(m_isEnter)
    {
        /*----鼠标进入后样式----*/
        pen.setWidth(2);
        pen.setColor(QColor(m_hoverColorStr));
        brush.setColor(m_color);
    }

    //画笔和画刷给到画家
    painter->setPen(pen);
    brush.setColor(m_color);
    painter->setBrush(brush);

    //绘制底层方形(带圆角)
    painter->drawRoundedRect(boundingRect(),4,4);

    //绘制文字-----
    if(m_isEnter)
    {
        pen.setWidth(1);
        pen.setColor(QColor("#fff"));
        painter->setPen(pen);
    }
    else
    {
        painter->setPen(oldPen);
    }

    painter->setBrush(oldBrush);
    font.setPixelSize(12);
    painter->setFont(font);

    qreal boundTopLX = boundingRect().topLeft().x();
    qreal boundTopLY = boundingRect().topLeft().y();

    int width = boundingRect().width();
    int height = 20;

    //距离
    int distance = 10;

    //楼区
    QRectF rang(boundTopLX,boundTopLY+distance,width,height);
    painter->drawText(rang,Qt::AlignCenter,m_data.tr_describe);

    distance = height+10;

    //绘制房号
    font.setPixelSize(16);
    painter->setFont(font);
    QRectF num(boundTopLX,boundTopLY+distance,width,height);
    painter->drawText(num,Qt::AlignCenter,m_data.room_num);

    distance = height*2+10;

    //房间类型
    font.setPixelSize(12);
    painter->setFont(font);
    QRectF type(boundTopLX,boundTopLY+distance,width,height);
    painter->drawText(type,Qt::AlignCenter,m_data.room_type);

    distance = height*3+10;

    //绘制价格
    font.setPixelSize(16);
    painter->setFont(font);
    QRectF pirce(boundTopLX,boundTopLY+distance,width,height);
    painter->drawText(pirce,Qt::AlignCenter,QString("￥%1").arg(m_data.room_price));

    //恢复旧画笔和笔刷
    painter->setPen(oldPen);
    painter->setBrush(oldBrush);
    painter->setFont(oldFont);
}

/**
 * @brief RoomStateItem::hoverEnterEvent
 * @param event
 * 悬浮进入事件,改变样式
 */
void RoomStateItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    if(!m_isEnter)
    {
        m_timer->start(1000);
    }
    m_isEnter = true;

    m_scenePos = event->screenPos();

    QGraphicsObject::hoverEnterEvent(event);
}

/**
 * @brief RoomStateItem::hoverLeaveEvent
 * @param event
 * 悬浮离开事件,改变样式
 */
void RoomStateItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_isEnter = false;
    m_timer->stop();
    emit leaveItem();
    QGraphicsObject::hoverLeaveEvent(event);
}

/**
 * @brief RoomStateItem::contextMenuEvent
 * @param event
 * 鼠标右键，打开菜单栏
 */
void RoomStateItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if(m_openFlag != 1)
    {
        QGraphicsObject::contextMenuEvent(event);
        return;
    }

    QPoint pos = event->screenPos();

    if(m_menu == nullptr)
    {
        m_menu = new QMenu;
    }

    if(m_data.room_state_id == 6 || m_data.room_state_id == 5)
    {
        if(m_actReserve == nullptr)
        {
            m_actReserve = new QAction(QIcon(":/images/reserve.png"),"预定",m_menu);
            connect(m_actReserve,&QAction::triggered,this,&RoomStateItem::onReserveActClicked);
        }

        m_menu->addAction(m_actReserve);
    }


    if(m_data.room_state_id == 6)
    {
        if(m_actOpenRoom == nullptr)
        {
            m_actOpenRoom = new QAction(QIcon(":/images/openRoom.png"),"开房",m_menu);
        }

        m_menu->addAction(m_actOpenRoom);
    }

    if(m_data.room_state_id == 4)
    {
        if(m_actSettlement == nullptr)
        {
            m_actSettlement = new QAction(QIcon(":/images/settlement.png"),"结算",m_menu);
        }

        m_menu->addAction(m_actSettlement);
    }

    if(m_actDetail == nullptr)
    {
        m_actDetail = new QAction(QIcon(":/images/detail.png"),"详情",m_menu);
        connect(m_actDetail,&QAction::triggered,this,&RoomStateItem::onDetailActClicked);
    }
    m_menu->addAction(m_actDetail);

    m_menu->exec(pos);

    QGraphicsObject::contextMenuEvent(event);
}

/**
 * @brief RoomStateItem::mouseDoubleClickEvent
 * @param event
 * 鼠标双击事件
 */
void RoomStateItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        //发送(房间ID、房间号、房间状态)
        onReserveActClicked();
    }

    QGraphicsObject::mouseDoubleClickEvent(event);
}


