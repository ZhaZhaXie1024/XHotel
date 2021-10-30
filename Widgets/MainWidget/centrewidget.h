#ifndef CENTREWIDGET_H
#define CENTREWIDGET_H

#include <QWidget>

class SysDepartment;
class SysStaffPosition;
class Staff;
class SysOperator;

class SysTowerRange;
class SysTowerSeat;
class SysRoomType;
class SysRoomState;
class SysRoomAttribute;
class Room;
class RoomOwnAttribute;

class SystemInitialValue;
class SysChargeItem;
class SysBillStateType;

class RoomStateMap;
class ReserveRoom;

class Client;

namespace Ui {
class CentreWidget;
}

class CentreWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CentreWidget(QWidget *parent = nullptr);
    ~CentreWidget();

    void init();

    //获取操作员ID
    int getOperatorID();

private:
    //读取文件获取操作员ID
    void operatorID();

public slots:
    //通过文本来选中或创建对应的widget添加到tabwidget中
    void addTabWidgetInName(QString widgetName);

    //Tab 的currentChanged信号的槽函数
    void onTabCurrentChanged(int index);
    //Tab的关闭按钮被按下
    void onTabCloseRequested(int index);

    //接收房态图传来的房间ID，打开预定界面
    void openReserveWidget(int room_id,QString room_num,int room_state_id);

signals:
    //发出信号，叫treeview清空选中项
    void cancelSelectedItem();

    //发出信号，叫treewidget选中项
    void setCurrentTreeWidgetItem(QString itemStr);

private:
    Ui::CentreWidget *ui;

    int m_operator_id;    //操作员ID

    //tab的widget
    SysDepartment *m_depWidget;             //部门
    SysStaffPosition *m_staffPosWidget;     //员工职位
    Staff *m_staffWidget;                   //员工
    SysOperator *m_operatorWidget;          //操作员

    SysTowerRange *m_towWidget;                     //楼区
    SysTowerSeat *m_towerSeatWidget;                //楼座
    SysRoomType *m_roomTypeWidget;                  //房间类型
    SysRoomState *m_roomStateWidget;                //房间状态
    SysRoomAttribute *m_roomAttributeWidget;        //房间属性
    Room *m_roomWidget;                             //房间
    RoomOwnAttribute *m_roomOwnAttributeWidget;     //房间拥有的属性

    SystemInitialValue *m_initaValueWidget;         //流水号管理
    SysChargeItem *m_chargeItemWidget;              //收费项目管理
    SysBillStateType *m_billStateTypeWidget;        //账单状态

    RoomStateMap *m_roomStateMapWidget;             //房态
    ReserveRoom *m_reserveRoomWidget;               //预约

    Client *m_clientWidget;                  //客人列表
};

#endif // CENTREWIDGET_H
