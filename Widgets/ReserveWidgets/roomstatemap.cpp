#include "roomstatemap.h"
#include "ui_roomstatemap.h"

#include <QIcon>
#include "Service/ReserveService/roomstatemapservice.h"
#include "Custom/roomstateitem.h"
#include "Widgets/ReserveWidgets/roomstatemapdialog.h"
#include "Widgets/ReserveWidgets/roomstateitemdatadialog.h"

#include <QDebug>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>


RoomStateMap::RoomStateMap(QWidget *parent,int openFlag) :
    QWidget(parent),
    ui(new Ui::RoomStateMap),
    m_openFlag(openFlag),
    m_index(0),
    m_showNum(40),
    m_sceneHeight(0)
{
    ui->setupUi(this);

    //初始化
    init();

    //弹出查询对话框
    connect(ui->btn_select,&QPushButton::clicked,this,&RoomStateMap::onSelectBtnCLicked);
    //进行刷新
    connect(ui->btn_refresh,&QPushButton::clicked,this,&RoomStateMap::onRefreshBtnClicked);
}

RoomStateMap::~RoomStateMap()
{
    delete ui;
}

/**
 * @brief RoomStateMap::init
 * 初始化
 */
void RoomStateMap::init()
{
    m_service = nullptr;
    m_dialog = nullptr;
    m_itemDialog = new RoomStateItemDataDialog(this);
    connect(m_itemDialog,&RoomStateItemDataDialog::dialogSignal,this,&RoomStateMap::itemDialogSlot);
    m_itemWidth = 76;
    m_itemHeight = 100;
    m_itemMargin = 12;
    m_appendIndex = 0;
    m_col = 0;
    m_row = 0;
    m_key.clear();
    m_value.clear();
    m_diaFlag = false;

    m_timer = new QTimer(this);
    connect(m_timer,&QTimer::timeout,this,&RoomStateMap::onTimerOutCloseDialog);

    //初始化颜色
    initColor();

    m_service = new RoomStateMapService(this);
    //设置鼠标跟踪，但是事件无法触发，就此注释
    //ui->graphicsView->setMouseTracking(true);
    //安装事件过滤器，让this先处理view的事件
    ui->graphicsView->installEventFilter(this);
    //设置场景
    ui->graphicsView->setScene(&m_scene);
}

/**
 * @brief RoomStateMap::initColor
 * 初始化toolbutton的颜色
 */
void RoomStateMap::initColor()
{
    QPixmap greenPix(16,16);
    greenPix.fill(QColor("#67C23A"));
    QIcon green(greenPix);
    ui->tbtn_green->setIcon(green);
    ui->tbtn_green->setToolButtonStyle(Qt::ToolButtonIconOnly);

    QPixmap bluePix(16,16);
    bluePix.fill(QColor("#409EFF"));
    QIcon blue(bluePix);
    ui->tbtn_blue->setIcon(blue);
    ui->tbtn_blue->setToolButtonStyle(Qt::ToolButtonIconOnly);

    QPixmap yellowPix(16,16);
    yellowPix.fill(QColor("#FCD913"));
    QIcon yellow(yellowPix);
    ui->tbtn_yellow->setIcon(yellow);
    ui->tbtn_yellow->setToolButtonStyle(Qt::ToolButtonIconOnly);

    QPixmap grayPix(16,16);
    grayPix.fill(QColor("#909399"));
    QIcon gray(grayPix);
    ui->tbtn_gray->setIcon(gray);
    ui->tbtn_gray->setToolButtonStyle(Qt::ToolButtonIconOnly);

    QPixmap redPix(16,16);
    redPix.fill(QColor("#F56C6C"));
    QIcon red(redPix);
    ui->tbtn_red->setIcon(red);
    ui->tbtn_red->setToolButtonStyle(Qt::ToolButtonIconOnly);

    connect(ui->tbtn_green,&QToolButton::clicked,this,&RoomStateMap::onGreenActTriggered);
    connect(ui->tbtn_blue,&QToolButton::clicked,this,&RoomStateMap::onBlueActTriggered);
    connect(ui->tbtn_yellow,&QToolButton::clicked,this,&RoomStateMap::onYellowActTriggered);
    connect(ui->tbtn_gray,&QToolButton::clicked,this,&RoomStateMap::onGrayActTriggered);
    connect(ui->tbtn_red,&QToolButton::clicked,this,&RoomStateMap::onRedActTriggered);
}

/**
 * @brief RoomStateMap::select
 * 获取数据
 */
bool RoomStateMap::select()
{
    //校对索引
    m_index = m_room.size() < 0 ? 0 : m_room.size();

    if(m_service == nullptr)
    {
        return false;
    }

    QList<RoomData> data = m_service->selectRoom(m_index,m_showNum,m_key,m_value);

    if(data.size() > 0)
    {
       for(int i = 0; i < data.size(); i++)
       {
           m_room.append(data.at(i));
       }
       return true;
    }

    //更新房间数量
    ui->room_num->setText(QString::number(m_room.size()));

    return false;
}

/**
 * @brief RoomStateMap::loadItem
 * 加载项
 */
void RoomStateMap::loadItem()
{
    int sceneX = m_scene.sceneRect().x();
    int sceneY = m_scene.sceneRect().y();

    m_overplus = 0;

    //更新房间数量
    ui->room_num->setText(QString::number(m_room.size()));

    if(m_room.size() <= 0)
    {
        return;
    }

    QString preRangStr = m_room.at(m_appendIndex).tr_describe;

    for(m_appendIndex; m_appendIndex < m_room.size(); m_appendIndex++)
    {
        if(m_col == m_colNum || preRangStr.compare(m_room.at(m_appendIndex).tr_describe) != 0)
        {
            m_col = 0;
            m_row++;
            preRangStr = m_room.at(m_appendIndex).tr_describe;
        }

        RoomData data = m_room.at(m_appendIndex);

        int itemX = sceneX + (m_itemWidth/2 + m_itemMargin) + (m_itemWidth+m_itemMargin) * m_col;
        int itemY = sceneY + (m_itemHeight/2 + m_itemMargin) + (m_itemHeight+m_itemMargin) * m_row;

        //有元素超出范围
        if(itemY+m_itemHeight > m_scene.sceneRect().bottomRight().ry())
        {
            m_overplus++;
        }

        RoomStateItem* item = new RoomStateItem(data,m_itemWidth,m_itemHeight,m_openFlag);
        //停留三秒，显示详情
        connect(item,&RoomStateItem::itemData,this,&RoomStateMap::showItemData);
        //离开关闭详情
        connect(item,&RoomStateItem::leaveItem,this,&RoomStateMap::leaveItem);
        //发送房间ID
        connect(item,&RoomStateItem::reserveRoom,this,&RoomStateMap::reserveRoom);
        //添加至场景
        m_scene.addItem(item);
        //放置到指定位置
        item->setPos(itemX,itemY);

        m_col++;
    }

//    ui->graphicsView->centerOn(-m_viewWidth/2,-m_viewHeight/2);
}

/**
 * @brief RoomStateMap::onGreenActTriggered
 * 空闲按钮被按下
 */
void RoomStateMap::onGreenActTriggered()
{
    onDiaSelectBtnClicked(0,0,0,6,0,0);
}

/**
 * @brief RoomStateMap::onBlueActTriggered
 * 蓝色按钮被按下
 */
void RoomStateMap::onBlueActTriggered()
{
    onDiaSelectBtnClicked(0,0,0,3,0,0);
}

/**
 * @brief RoomStateMap::onYellowActTriggered
 * 黄色按钮被按下
 */
void RoomStateMap::onYellowActTriggered()
{
    onDiaSelectBtnClicked(0,0,0,1,0,0);
}

/**
 * @brief RoomStateMap::onGrayActTriggered
 * 灰色按钮被按下
 */
void RoomStateMap::onGrayActTriggered()
{
    onDiaSelectBtnClicked(0,0,0,4,0,0);
}

/**
 * @brief RoomStateMap::onRedActTriggered
 * 红色按钮被按下
 */
void RoomStateMap::onRedActTriggered()
{
    onDiaSelectBtnClicked(0,0,0,2,0,0);
}

/**
 * @brief RoomStateMap::onSelectBtnCLicked
 * 查询按钮被按下
 */
void RoomStateMap::onSelectBtnCLicked()
{
    if(m_dialog == nullptr)
    {
        m_dialog = new RoomStateMapDialog(this);
        //关联获取查询信号
        connect(m_dialog,&RoomStateMapDialog::selectKeyValue,this,&RoomStateMap::onDiaSelectBtnClicked);
    }

    m_dialog->show();
}

/**
 * @brief RoomStateMap::onDiaSelectBtnClicked
 * 当对话框的查询按钮被按下，获取那边传来的数据，进行查询
 */
void RoomStateMap::onDiaSelectBtnClicked(int trId,int tsId,int typeId,int stateId,int bedNum,int price)
{
    m_key.clear();
    m_value.clear();

    if(trId > 0)
    {
        m_key.append("tower_range_id");
        m_value.append(trId);
    }
    if(tsId > 0)
    {
        m_key.append("tower_seat_id");
        m_value.append(tsId);
    }
    if(typeId > 0)
    {
        m_key.append("room_type_id");
        m_value.append(typeId);
    }
    if(stateId > 0)
    {
        m_key.append("room_state_id");
        m_value.append(stateId);
    }
    if(bedNum > 0)
    {
        m_key.append("bed_number");
        m_value.append(bedNum);
    }
    if(price > 0)
    {
        m_key.append("room_rate");
        m_value.append(price);
    }

    //调用刷新查询
    onRefreshBtnClicked();
}

/**
 * @brief RoomStateMap::onRefreshBtnClicked
 * 刷新数据，并重新加载item
 */
void RoomStateMap::onRefreshBtnClicked()
{
    m_scene.clear();

    m_room.clear();

    m_index = 0;
    m_appendIndex = 0;
    m_col = 0;
    m_row = 0;

    select();
    loadItem();
}

/**
 * @brief RoomStateMap::showItemData
 * @param data
 * 接收从item传来的data，并创建一个无边框对话框显示
 */
void RoomStateMap::showItemData(RoomData data,QPointF scenePos,int flage)
{
    m_diaflage = flage;

    QPoint viewBottomRight = ui->graphicsView->frameGeometry().bottomRight();

    QWidget* temp  = ui->graphicsView;

    while (true) {
        if(temp->parentWidget()!=nullptr)
        {
            viewBottomRight = temp->mapToParent(viewBottomRight);
            temp = temp->parentWidget();
        }
        else
        {
            break;
        }
    }

//    qDebug() << "viewBottomRight" << viewBottomRight << scenePos.x()+m_itemDialog->width() << scenePos.y()+m_itemDialog->height();

    int viewRight = viewBottomRight.x();
    int viewBottom = viewBottomRight.y();

    int xValue = scenePos.x()+m_itemDialog->width() - viewRight;
    int yValue = scenePos.y()+m_itemDialog->height() - viewBottom;

    //超出右边
    if(xValue > 0)
    {
        scenePos.rx() -= xValue;
    }
    //超出下边
    if(yValue > 0)
    {
        scenePos.ry() -= yValue;
    }

    m_itemDialog->setPos(scenePos.toPoint());
    m_itemDialog->setData(data.room_num,data.tr_describe,data.ts_describe,QString::number(data.tower_tier),
                          QString::number(data.bed_number),data.room_state,data.room_type,QString::number(data.room_price),flage);
    m_itemDialog->show();
}

/**
 * @brief RoomStateMap::leaveItem
 * 接收item传来的关闭详情对话信号
 */
void RoomStateMap::leaveItem()
{
    if(m_diaflage != 1)
    {
        return;
    }
    m_timer->start(1000);
}

/**
 * @brief RoomStateMap::itemDialogSlot
 * @param flag
 * 项详情对话框，进入和离开事件的接收槽函数
 */
void RoomStateMap::itemDialogSlot(bool flag)
{
    m_diaFlag = flag;
}

/**
 * @brief RoomStateMap::onTimerOutCloseDialog
 * itemDialog的关闭
 */
void RoomStateMap::onTimerOutCloseDialog()
{
    if(m_diaFlag)
    {
        m_timer->stop();
        return;
    }

    m_timer->stop();
    m_itemDialog->close();
}

/**
 * @brief RoomStateMap::reserveRoom
 * @param room_id
 * 预定房间，发送房间ID
 */
void RoomStateMap::reserveRoom(int room_id,QString room_num,int room_state_id)
{
    if(m_openFlag == 1)
    {
        emit centreWidgetRoom(room_id,room_num,room_state_id);
    }
    else
    {
        emit reserveWidgetRoom(room_id,room_num,room_state_id);
    }
}

/**
 * @brief RoomStateMap::resizeEvent
 * @param event
 * 页面大小改变事件
 */
void RoomStateMap::resizeEvent(QResizeEvent *)
{
    m_viewWidth = ui->graphicsView->width();
    m_viewHeight = ui->graphicsView->height();

    m_sceneHeight = m_viewWidth;

    m_scene.setSceneRect(-m_viewWidth/2,-m_viewHeight/2,m_viewWidth-20,m_sceneHeight);
//    m_scene.setBackgroundBrush(QBrush(QColor("#278813")));
    m_scene.clear();
    m_room.clear();

    m_appendIndex = 0;
    m_col = 0;
    m_row = 0;

    //计算可显示的数量
    m_colNum = m_scene.width()/89;
    m_rowNum = m_scene.height()/109;
    m_showNum = m_colNum*m_rowNum;

    select();
    //加载项
    loadItem();

    ui->graphicsView->centerOn(-m_viewWidth/2,-m_viewHeight/2);
}


/**
 * @brief RoomStateMap::eventFilter
 * @param watched
 * @param event
 * @return true,子类不在处理event 。 false，子类还可以继续处理该event
 * 处理子类的事件
 */
bool RoomStateMap::eventFilter(QObject *watched, QEvent *event)
{
    //处理视图事件
    if(ui->graphicsView == watched)
    {
        //处理view的滚轮事件
        if(QEvent::Wheel == event->type())
        {
            QWheelEvent* wheel = (QWheelEvent*)event;

            if(m_overplus)
            {
                //计算高度
                int height = (m_overplus/m_colNum) * (m_itemHeight + m_itemMargin);
                if(m_overplus%m_colNum != 0)
                {
                    height += m_itemHeight+m_itemMargin;
                }
                m_scene.setSceneRect(-m_viewWidth/2,-m_viewHeight/2,m_viewWidth-20,m_sceneHeight+=height);

                m_overplus = 0;
            }

            //到底了还是用滚轮向下滚
            if(wheel->angleDelta().y()/8 < 0)
            {
                //加载新数据
                if(select())
                {
                    m_scene.setSceneRect(-m_viewWidth/2,-m_viewHeight/2,m_viewWidth-20,m_sceneHeight+=m_viewHeight);
                    loadItem();

//                    qDebug() << m_sceneHeight << ui->room_num->text();
                }
            }
        }

    }

    return QWidget::eventFilter(watched,event);
}

