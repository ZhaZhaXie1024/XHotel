#ifndef ROOMSTATEMAP_H
#define ROOMSTATEMAP_H

#include <QWidget>
#include <QGraphicsScene>
#include "Entitys/room.h"

class RoomStateMapService;
class RoomStateMapDialog;
class RoomStateItemDataDialog;
namespace Ui {
class RoomStateMap;
}

class RoomStateMap : public QWidget
{
    Q_OBJECT

public:
    //openFlag，为1时代表是左边菜单点击打开，其它为 其它地方打开
    explicit RoomStateMap(QWidget *parent = nullptr,int openFlag = 1);
    ~RoomStateMap();

    void init();

    //初始化toolbutton的颜色
    void initColor();

    //获取数据
    bool select();

    //加载项
    void loadItem();

public slots:
    //toolbutton的槽函数
    void onGreenActTriggered();
    void onBlueActTriggered();
    void onYellowActTriggered();
    void onGrayActTriggered();
    void onRedActTriggered();


    //查询按钮被按下
    void onSelectBtnCLicked();

    //当对话框的查询按钮被按下，获取那边传来的数据，进行查询
    void onDiaSelectBtnClicked(int trId,int tsId,int typeId,int stateId,int bedNum,int price);

    //刷新按钮被按下
    void onRefreshBtnClicked();

    //接收从item传来的data，并创建一个无边框对话框显示
    void showItemData(RoomData data,QPointF scenePos,int flage);
    //接收item传来的关闭详情对话信号
    void leaveItem();

    //itemDialog的关闭
    void itemDialogSlot(bool flag);

    //itemDialog的关闭
    void onTimerOutCloseDialog();

    //预定房间，发送房间ID
    void reserveRoom(int room_id,QString room_num,int room_state_id);

signals:
    //菜单栏(centrewidget)接收房间ID的信号
    void centreWidgetRoom(int room_id,QString room_num,int room_state_id);

    //预定界面接收房间ID的信号
    void reserveWidgetRoom(int room_id,QString room_num,int room_state_id);


protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    Ui::RoomStateMap *ui;

    RoomStateMapService* m_service;
    int m_openFlag;

    QGraphicsScene m_scene;
    RoomStateMapDialog *m_dialog;
    RoomStateItemDataDialog *m_itemDialog;

    QList<RoomData> m_room; //数据的容器

    int m_index;
    int m_showNum;
    QStringList m_key;
    QVariantList m_value;

    int m_sceneHeight;  //当前高度

    int m_viewWidth;
    int m_viewHeight;

    int m_itemWidth;    //项宽度
    int m_itemHeight;   //项高度
    int m_itemMargin;   //项间距

    int m_colNum;       //一行显示多少列
    int m_rowNum;       //显示多少行

    int m_appendIndex;  //开始插入索引
    int m_col;          //当前显示到第几列
    int m_row;          //当前显示到第几行

    int m_overplus;     //超出场景的元素个数

    bool m_diaFlag;     //true：显示，false隐藏
    QTimer *m_timer;
    int m_diaflage;     //1,时间限制，其它,无

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // ROOMSTATEMAP_H
