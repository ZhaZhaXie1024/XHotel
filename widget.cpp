#include "widget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QTimer>
#include <QDateTime>

#include "Widgets/MainWidget/headerwidget.h"
#include "Widgets/MainWidget/modeltreewidget.h"
#include "Widgets/MainWidget/centrewidget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    initLayout();
    initWidget();
    initConnect();
}

Widget::Widget(int operator_id, QString operatorName, QWidget *parent)
    :QWidget(parent)
{
    m_operatorID = operator_id;
    m_operatorName = operatorName;
    initLayout();
    initWidget();
    initConnect();
}

Widget::~Widget()
{
    if(m_dateTime!=nullptr)
    {
        delete m_dateTime;
        m_dateTime = nullptr;
    }
}

/**
 * @brief Widget::initLayout
 * 功能：初始化布局
 */
void Widget::initLayout()
{
    m_mainLayout = new QVBoxLayout(this);
    setLayout(m_mainLayout);
}

/**
 * @brief Widget::initWidget
 * 功能：初始化 控件（组）
 */
void Widget::initWidget()
{
    //设置标题
    setWindowTitle("九忆酒店管理系统");
    //设置icon
    setWindowIcon(QIcon(":/images/logowhite.png"));

    //修改背景颜色
    setAutoFillBackground(true);                //允许填充背景
    setPalette(QPalette(QColor(255,255,255)));  //填充为白色

    //调整大小(窗口最大化)
    if(!isMaximized())
    {
        setWindowState(Qt::WindowMaximized);
    }

    //设置窗体最小大小
    setMinimumSize(830,520);

    //初始化头部
    m_headerWidget = new HeaderWidget(m_operatorName,this);
    m_mainLayout->addWidget(m_headerWidget);

    //初始化布局
    m_centreLayout = new QHBoxLayout();
    m_modelTreeViewLayout = new QVBoxLayout();

    //初始化模块栏
    m_modelTreeWidget = new ModelTreeWidget(this);
    //初始化时间控件
    m_timeLabel = new QLabel(m_dateTime->currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    m_timeLabel->setAlignment(Qt::AlignCenter);
    m_timeLabel->setMargin(10);
    m_timeLabel->setFont(QFont("印品睿圆体",18,2));

    //添加进入m_modelTreeViewLayout
    m_modelTreeViewLayout->addWidget(m_modelTreeWidget,1);
    m_modelTreeViewLayout->addWidget(m_timeLabel,0);

    //初始化中心控件tabwidget
    m_centreWidget = new CentreWidget(this);

    //添加至m_centreLayout
    m_centreLayout->addLayout(m_modelTreeViewLayout,0);
    m_centreLayout->addWidget(m_centreWidget,1);

    m_mainLayout->addLayout(m_centreLayout);

    //初始化时间
    onTimeOutLabel();

    //初始化定时器
    m_timer = new QTimer(this);
    m_dateTime = new QDateTime;
    m_timer->start(3000);
}

/**
 * @brief Widget::initConnect
 * 功能：初始化链接信号和槽
 */
void Widget::initConnect()
{
    //中转，添加tab
    connect(m_modelTreeWidget,&ModelTreeWidget::treeWidgetItemClicked,m_centreWidget,&CentreWidget::addTabWidgetInName);

    //刷新时间
    connect(m_timer,&QTimer::timeout,this,&Widget::onTimeOutLabel);

    //中转，取消treewidget选中项
    connect(m_centreWidget,&CentreWidget::cancelSelectedItem,m_modelTreeWidget,&ModelTreeWidget::cancelSelectedItem);
    //中转，选中一项
    connect(m_centreWidget,&CentreWidget::setCurrentTreeWidgetItem,m_modelTreeWidget,&ModelTreeWidget::setCurrentItem);
}

/**
 * @brief Widget::onTimeOutLabel
 * 功能：用于刷新时间
 */
void Widget::onTimeOutLabel()
{
    QString timeStr = m_dateTime->currentDateTime().toString("MM-dd HH:mm ddd");

    m_timeLabel->setText(timeStr);
}




