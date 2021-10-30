#ifndef ROOMSTATEITEM_H
#define ROOMSTATEITEM_H

#include <QGraphicsObject>

#include "Entitys/room.h"

class QMenu;
class QAction;

class RoomStateItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit RoomStateItem(RoomData data,int width,int height,int openFlag,QGraphicsItem *parent = nullptr);

    ~RoomStateItem();

    void init();

public slots:
    //定时器的槽函数
    void onTimeOutEmitData();

    //act显示详情槽函数
    void onDetailActClicked();

    //act预定槽函数
    void onReserveActClicked();

signals:
    //发送数据(显示对话框，1，对话框有时间限制，其它没有限制)
    void itemData(RoomData data,QPointF scenePos,int flage = 1);
    //发送离开item信号
    void leaveItem();

    //发送房间ID和名称
    void reserveRoom(int roomID,QString room_num,int room_state_id);

    // QGraphicsItem interface
public:
    //绘制区域
    virtual QRectF boundingRect() const override;
    //绘制
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    //悬浮进入事件
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    //悬浮离开事件
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    //右键弹出菜单项
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

    //双击鼠标事件
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    RoomData m_data;

    QColor m_color;

    int m_width;
    int m_height;
    int m_openFlag;     //1是左边菜单打开，其它是弹窗的形式打开

    //鼠标进入标志
    bool m_isEnter;
    //计时器
    QTimer *m_timer;

    //该项在场景中的坐标
    QPointF m_scenePos;

    //鼠标进入后 显示的颜色，文本
    QString m_hoverColorStr;

    //菜单
    QMenu *m_menu;
    QAction *m_actReserve;
    QAction *m_actOpenRoom;
    QAction *m_actSettlement;
    QAction *m_actDetail;

};

#endif // ROOMSTATEITEM_H
