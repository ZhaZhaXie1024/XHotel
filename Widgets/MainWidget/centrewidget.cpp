#include "centrewidget.h"
#include "ui_centrewidget.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>

#include "Widgets/SystemWidget/StaffManageWidgets/sysdepartment.h"
#include "Widgets/SystemWidget/StaffManageWidgets/sysstaffposition.h"
#include "Widgets/SystemWidget/StaffManageWidgets/staff.h"
#include "Widgets/SystemWidget/StaffManageWidgets/sysoperator.h"

#include "Widgets/SystemWidget/RoomManageWidgets/systowerrange.h"
#include "Widgets/SystemWidget/RoomManageWidgets/systowerseat.h"
#include "Widgets/SystemWidget/RoomManageWidgets/sysroomtype.h"
#include "Widgets/SystemWidget/RoomManageWidgets/sysroomstate.h"
#include "Widgets/SystemWidget/RoomManageWidgets/sysroomattribute.h"
#include "Widgets/SystemWidget/RoomManageWidgets/room.h"
#include "Widgets/SystemWidget/RoomManageWidgets/roomownattribute.h"

#include "Widgets/SystemWidget/OtherManageWidgets/systeminitialvalue.h"
#include "Widgets/SystemWidget/OtherManageWidgets/syschargeitem.h"
#include "Widgets/SystemWidget/OtherManageWidgets/sysbillstatetype.h"

#include "Widgets/ReserveWidgets/roomstatemap.h"
#include "Widgets/ReserveWidgets/reserveroom.h"

#include "Widgets/ClientWidgets/client.h"

CentreWidget::CentreWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CentreWidget)
{
    ui->setupUi(this);
    init();
}

CentreWidget::~CentreWidget()
{
    delete ui;
}

void CentreWidget::init()
{
    //修改背景颜色
    setAutoFillBackground(true);                //允许填充背景
    setPalette(QPalette(QColor(255,255,255)));  //填充为白色

    //初始化
    m_depWidget = nullptr;
    m_staffPosWidget = nullptr;
    m_staffWidget = nullptr;
    m_operatorWidget = nullptr;

    m_towWidget = nullptr;
    m_towerSeatWidget = nullptr;
    m_roomTypeWidget = nullptr;
    m_roomStateWidget = nullptr;
    m_roomAttributeWidget = nullptr;
    m_roomWidget = nullptr;
    m_roomOwnAttributeWidget = nullptr;

    m_initaValueWidget = nullptr;
    m_chargeItemWidget = nullptr;
    m_billStateTypeWidget = nullptr;

    m_roomStateMapWidget = nullptr;
    m_reserveRoomWidget = nullptr;

    m_clientWidget = nullptr;

    addTabWidgetInName("房态");

    //初始化连接

    //tab CurrentChanged槽函数
    connect(ui->tabWidget,&QTabWidget::currentChanged,this,&CentreWidget::onTabCurrentChanged);

    //tab点击关闭按钮
    connect(ui->tabWidget,&QTabWidget::tabCloseRequested,this,&CentreWidget::onTabCloseRequested);

    //获取操作员ID
    operatorID();
}

/**
 * @brief CentreWidget::getOperatorID
 * @return 获取操作员ID
 */
int CentreWidget::getOperatorID()
{
    return m_operator_id;
}

/**
 * @brief CentreWidget::getOperatorID
 * 读取文件中的操作员ID
 */
void CentreWidget::operatorID()
{
    QFile file("../operatorInfo.txt");

    int operatorID = 0;

    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);

        QString str = stream.readAll();

        QStringList list = str.split(" ");

        operatorID = list[0].toInt();

        file.close();
    }

    if(operatorID > 0)
    {
        m_operator_id = operatorID;
    }
}

/**
 * @brief CentreWidget::addTabWidgetInName
 * @param widgetName treewidget中获取到的text文本
 * 功能：通过文本来选中或创建对应的widget添加到tabwidget中
 */
void CentreWidget::addTabWidgetInName(QString widgetName)
{
    if(widgetName.compare("部门管理") == 0)
    {
        if(m_depWidget!=nullptr)
        {
            ui->tabWidget->setCurrentWidget(m_depWidget);
        }
        else
        {
            m_depWidget = new SysDepartment(ui->tabWidget);
            ui->tabWidget->addTab(m_depWidget,"部门管理");
            ui->tabWidget->setCurrentWidget(m_depWidget);
        }
    }
    else if(widgetName.compare("员工管理") == 0)
    {
        if(m_staffWidget!=nullptr)
        {
            ui->tabWidget->setCurrentWidget(m_staffWidget);
        }
        else
        {
            m_staffWidget = new Staff(ui->tabWidget);
            ui->tabWidget->addTab(m_staffWidget,"员工管理");
            ui->tabWidget->setCurrentWidget(m_staffWidget);
        }
    }
    else if(widgetName.compare("操作员管理") == 0)
    {
        if(m_operatorWidget!=nullptr)
        {
            ui->tabWidget->setCurrentWidget(m_operatorWidget);
        }
        else
        {
            m_operatorWidget = new SysOperator(ui->tabWidget);
            ui->tabWidget->addTab(m_operatorWidget,"操作员管理");
            ui->tabWidget->setCurrentWidget(m_operatorWidget);
        }
    }
    else if(widgetName.compare("员工职位管理") == 0)
    {
        if(m_staffPosWidget!=nullptr)
        {
            ui->tabWidget->setCurrentWidget(m_staffPosWidget);
        }
        else
        {
            m_staffPosWidget = new SysStaffPosition(ui->tabWidget);
            ui->tabWidget->addTab(m_staffPosWidget,"员工职位管理");
            ui->tabWidget->setCurrentWidget(m_staffPosWidget);
        }
    }
    else if(widgetName.compare("楼区管理") == 0)
    {
        if(m_towWidget!=nullptr)
        {
            ui->tabWidget->setCurrentWidget(m_towWidget);
        }
        else
        {
            m_towWidget = new SysTowerRange(ui->tabWidget);
            ui->tabWidget->addTab(m_towWidget,"楼区管理");
            ui->tabWidget->setCurrentWidget(m_towWidget);
        }
    }
    else if(widgetName.compare("楼座管理") == 0)
    {
        if(m_towerSeatWidget!=nullptr)
        {
            ui->tabWidget->setCurrentWidget(m_towerSeatWidget);
        }
        else
        {
            m_towerSeatWidget = new SysTowerSeat(ui->tabWidget);
            ui->tabWidget->addTab(m_towerSeatWidget,"楼座管理");
            ui->tabWidget->setCurrentWidget(m_towerSeatWidget);
        }
    }
    else if(widgetName.compare("房间类型管理") == 0)
    {
        if(m_roomTypeWidget!=nullptr)
        {
            ui->tabWidget->setCurrentWidget(m_roomTypeWidget);
        }
        else
        {
            m_roomTypeWidget = new SysRoomType(ui->tabWidget);
            ui->tabWidget->addTab(m_roomTypeWidget,"房间类型管理");
            ui->tabWidget->setCurrentWidget(m_roomTypeWidget);
        }
    }
    else if(widgetName.compare("房间状态管理") == 0)
    {
        if(m_roomStateWidget!=nullptr)
        {
            ui->tabWidget->setCurrentWidget(m_roomStateWidget);
        }
        else
        {
            m_roomStateWidget = new SysRoomState(ui->tabWidget);
            ui->tabWidget->addTab(m_roomStateWidget,"房间状态管理");
            ui->tabWidget->setCurrentWidget(m_roomStateWidget);
        }
    }
    else if(widgetName.compare("房间属性管理") == 0)
    {
        if(m_roomAttributeWidget!=nullptr)
        {
            ui->tabWidget->setCurrentWidget(m_roomStateWidget);
        }
        else
        {
            m_roomAttributeWidget = new SysRoomAttribute(ui->tabWidget);
            ui->tabWidget->addTab(m_roomAttributeWidget,"房间属性管理");
            ui->tabWidget->setCurrentWidget(m_roomAttributeWidget);
        }
    }
    else if(widgetName.compare("房间管理") == 0)
    {
        if(m_roomWidget!=nullptr)
        {
            ui->tabWidget->setCurrentWidget(m_roomWidget);
        }
        else
        {
            m_roomWidget = new Room(ui->tabWidget);
            ui->tabWidget->addTab(m_roomWidget,"房间管理");
            ui->tabWidget->setCurrentWidget(m_roomWidget);
        }
    }
    else if(widgetName.compare("设置房间属性") == 0)
    {
        if(m_roomOwnAttributeWidget!=nullptr)
        {
            ui->tabWidget->setCurrentWidget(m_roomOwnAttributeWidget);
        }
        else
        {
            m_roomOwnAttributeWidget = new RoomOwnAttribute(ui->tabWidget);
            ui->tabWidget->addTab(m_roomOwnAttributeWidget,"设置房间属性");
            ui->tabWidget->setCurrentWidget(m_roomOwnAttributeWidget);
        }
    }
    else if(widgetName.compare("流水号管理") == 0)
    {
        if(m_initaValueWidget!=nullptr)
        {
            ui->tabWidget->setCurrentWidget(m_initaValueWidget);
        }
        else
        {
            m_initaValueWidget = new SystemInitialValue(ui->tabWidget);
            ui->tabWidget->addTab(m_initaValueWidget,"流水号管理");
            ui->tabWidget->setCurrentWidget(m_initaValueWidget);
        }
    }
    else if(widgetName.compare("收费项目") == 0)
    {
        if(m_chargeItemWidget != nullptr)
        {
            ui->tabWidget->setCurrentWidget(m_chargeItemWidget);
        }
        else
        {
            m_chargeItemWidget = new SysChargeItem(ui->tabWidget);
            ui->tabWidget->addTab(m_chargeItemWidget,"收费项目");
            ui->tabWidget->setCurrentWidget(m_chargeItemWidget);
        }
    }
    else if(widgetName.compare("账单状态") == 0)
    {
        if(m_billStateTypeWidget != nullptr)
        {
            ui->tabWidget->setCurrentWidget(m_billStateTypeWidget);
        }
        else
        {
            m_billStateTypeWidget = new SysBillStateType(ui->tabWidget);
            ui->tabWidget->addTab(m_billStateTypeWidget,"账单状态");
            ui->tabWidget->setCurrentWidget(m_billStateTypeWidget);
        }
    }
    else if(widgetName.compare("房态") == 0)
    {
        if(m_roomStateMapWidget!=nullptr)
        {
            ui->tabWidget->setCurrentWidget(m_roomStateMapWidget);
        }
        else
        {
            m_roomStateMapWidget = new RoomStateMap(ui->tabWidget);
            ui->tabWidget->addTab(m_roomStateMapWidget,"房态");
            ui->tabWidget->setCurrentWidget(m_roomStateMapWidget);
            //打开预定，并设置房间
            connect(m_roomStateMapWidget,&RoomStateMap::centreWidgetRoom,this,&CentreWidget::openReserveWidget);
        }
    }
    else if(widgetName.compare("预约") == 0)
    {
        if(m_reserveRoomWidget!=nullptr)
        {
            ui->tabWidget->setCurrentWidget(m_reserveRoomWidget);
        }
        else
        {
            m_reserveRoomWidget = new ReserveRoom(ui->tabWidget);
            m_reserveRoomWidget->setOperatorID(m_operator_id);
            ui->tabWidget->addTab(m_reserveRoomWidget,"预约");
            ui->tabWidget->setCurrentWidget(m_reserveRoomWidget);            
        }
    }
    else if(widgetName.compare("客人列表") == 0)
    {
        if(m_clientWidget!=nullptr)
        {
            ui->tabWidget->setCurrentWidget(m_clientWidget);
        }
        else
        {
            m_clientWidget = new Client(ui->tabWidget);
            ui->tabWidget->addTab(m_clientWidget,"客人列表");
            ui->tabWidget->setCurrentWidget(m_clientWidget);
        }
    }


}

/**
 * @brief CentreWidget::onTabCurrentChanged
 * @param index
 * 的currentChanged信号的槽函数
 */
void CentreWidget::onTabCurrentChanged(int index)
{
    if(index!=-1)
    {
        emit setCurrentTreeWidgetItem(ui->tabWidget->tabText(index));
    }
    else
    {
        //清除选中的项,最后一项都没了
        emit cancelSelectedItem();
    }
}

/**
 * @brief CentreWidget::onTabCloseRequested
 * @param index 关闭的tab的索引值
 * 功能：Tab的关闭按钮被按下，关闭对应的tab页面
 */
void CentreWidget::onTabCloseRequested(int index)
{
    QString objectName = ui->tabWidget->widget(index)->objectName();

    ui->tabWidget->removeTab(index);

    if(objectName.compare("SysDepartment") == 0){
        m_depWidget->close();
        delete m_depWidget;
        m_depWidget = nullptr;
    }
    else if(objectName.compare("Staff") == 0)
    {
        m_staffWidget->close();
        delete m_staffWidget;
        m_staffWidget = nullptr;
    }
    else if(objectName.compare("SysOperator") == 0)
    {
        m_operatorWidget->close();
        delete m_operatorWidget;
        m_operatorWidget = nullptr;
    }
    else if(objectName.compare("SysStaffPosition") == 0)
    {
        m_staffPosWidget->close();
        delete m_staffPosWidget;
        m_staffPosWidget = nullptr;
    }
    else if(objectName.compare("SysTowerRange") == 0)
    {
        m_towWidget->close();
        delete m_towWidget;
        m_towWidget = nullptr;
    }
    else if(objectName.compare("SysTowerSeat") == 0)
    {
        m_towerSeatWidget->close();
        delete m_towerSeatWidget;
        m_towerSeatWidget = nullptr;
    }
    else if(objectName.compare("SysRoomType") == 0)
    {
        m_roomTypeWidget->close();
        delete m_roomTypeWidget;
        m_roomTypeWidget = nullptr;
    }
    else if(objectName.compare("SysRoomState") == 0)
    {
        m_roomStateWidget->close();
        delete m_roomStateWidget;
        m_roomStateWidget = nullptr;
    }
    else if(objectName.compare("SysRoomAttribute") == 0)
    {
        m_roomAttributeWidget->close();
        delete m_roomAttributeWidget;
        m_roomAttributeWidget = nullptr;
    }
    else if(objectName.compare("Room") == 0)
    {
        m_roomWidget->close();
        delete m_roomWidget;
        m_roomWidget = nullptr;
    }
    else if(objectName.compare("RoomOwnAttribute") == 0)
    {
        m_roomOwnAttributeWidget->close();
        delete m_roomOwnAttributeWidget;
        m_roomOwnAttributeWidget = nullptr;
    }
    else if(objectName.compare("SystemInitialValue") == 0)
    {
        m_initaValueWidget->close();
        delete m_initaValueWidget;
        m_initaValueWidget = nullptr;
    }
    else if(objectName.compare("RoomStateMap") == 0)
    {
        m_roomStateMapWidget->close();
        delete m_roomStateMapWidget;
        m_roomStateMapWidget = nullptr;
    }
    else if(objectName.compare("SysChargeItem") == 0)
    {
        m_chargeItemWidget->close();
        delete m_chargeItemWidget;
        m_chargeItemWidget = nullptr;
    }
    else if(objectName.compare("SysBillStateType") == 0)
    {
        m_billStateTypeWidget->close();
        delete m_billStateTypeWidget;
        m_billStateTypeWidget = nullptr;
    }
    else if(objectName.compare("ReserveRoom") == 0)
    {
        m_reserveRoomWidget->close();
        delete m_reserveRoomWidget;
        m_reserveRoomWidget = nullptr;
    }

    //emit setCurrentTreeWidgetItem(ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
}

/**
 * @brief CentreWidget::openReserveWidget
 * @param room_id
 * 接收房态图传来的房间ID，打开预定界面
 */
void CentreWidget::openReserveWidget(int room_id,QString room_num,int room_state_id)
{
    addTabWidgetInName("预约");

    m_reserveRoomWidget->setRoom(room_id,room_num,room_state_id);

    //发出信号，叫treewidget选中项
    emit setCurrentTreeWidgetItem("预约");
}
