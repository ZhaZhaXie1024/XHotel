#include "roomownattribute.h"
#include "ui_roomownattribute.h"

#include <QDebug>
#include <QMessageBox>

#include "Service/SystemService/RoomManageService/roomownattributeservice.h"
#include "Custom/customtableviewmodel.h"
#include "Widgets/SystemWidget/RoomManageWidgets/roomfilterdialog.h"
#include "Entitys/SystemRoomManageEntitys.h"

RoomOwnAttribute::RoomOwnAttribute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoomOwnAttribute)
{
    ui->setupUi(this);

    m_service = new RoomOwnAttributeService(this);
    m_model = new CustomTableViewModel(this);
    m_dialog = new RoomFilterDialog(this);

    initConnect();
    initRoomTableView();
    initRoomAttribueWidget();
}

RoomOwnAttribute::~RoomOwnAttribute()
{
    delete ui;
}

/**
 * @brief RoomOwnAttribute::initConnect
 * 初始化连接
 */
void RoomOwnAttribute::initConnect()
{
    //接收likeindex
    connect(m_service,&RoomOwnAttributeService::sendIndex,this,&RoomOwnAttribute::onLikeIndex);
    //查询按钮被按下
    connect(ui->btn_select,&QPushButton::clicked,this,&RoomOwnAttribute::onSelectBtnClicked);
    //当查询对话框中 “查询” 按钮被按下，接收他发出的信号
    connect(m_dialog,&RoomFilterDialog::select,this,&RoomOwnAttribute::onDiaSelectBtnClicked);

    //当roomTableView项被激活时,查询当前房间拥有的属性
    connect(ui->room_tableview,&QTableView::pressed,this,&RoomOwnAttribute::onRoomTableViewActivated);
    //当添加 左箭头 按钮被按下，新增一条选中房间的房间属性
    connect(ui->btn_add,&QPushButton::clicked,this,&RoomOwnAttribute::onAddBtnClicked);
    //删除
    connect(ui->btn_remove,&QPushButton::clicked,this,&RoomOwnAttribute::onDeleteBtnClicked);
}

/**
 * @brief RoomOwnAttribute::initRoomTableView
 * 初始化房间表格
 */
void RoomOwnAttribute::initRoomTableView()
{
    QString sql = m_service->selectRoomSql();

    m_model->setEditable(false);    //设置不可编辑
    m_model->setOpenPaging(false);  //设置不分页
    m_model->setSql(sql,QStringList()<<"room_id"<<"房号"<<"楼区"<<"楼座"<<"楼层"<<"房间类型"<<"房间状态");
    m_model->select(m_whereValue,m_likeIndex);
    ui->room_tableview->setModel(m_model);
    ui->room_tableview->hideColumn(0);
}

/**
 * @brief RoomOwnAttribute::initRoomAttribueWidget
 * 初始化房间属性表格
 */
void RoomOwnAttribute::initRoomAttribueWidget()
{
    ui->tablewidget->setColumnCount(3);
    ui->tablewidget->setHorizontalHeaderLabels(QStringList() << "id" << "代码" << "属性描述");

    QList<RoomAttribueInfo> data = m_service->selectRoomAttribue();

    ui->tablewidget->setRowCount(data.size());

    for(int i = 0; i < data.size(); i++)
    {
        RoomAttribueInfo info = data.at(i);

        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(info.room_attribute_id));
        item0->setData(Qt::UserRole,info.room_attribute_id);
        item0->setTextAlignment(Qt::AlignCenter);
        ui->tablewidget->setItem(i,0,item0);

        QTableWidgetItem *item1 = new QTableWidgetItem(info.code);
        item1->setData(Qt::UserRole,info.room_attribute_id);
        item1->setTextAlignment(Qt::AlignCenter);
        ui->tablewidget->setItem(i,1,item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(info.m_describe);
        item2->setData(Qt::UserRole,info.room_attribute_id);
        item2->setTextAlignment(Qt::AlignCenter);
        ui->tablewidget->setItem(i,2,item2);
    }

    ui->tablewidget->hideColumn(0);
}

/**
 * @brief RoomOwnAttribute::refreshOwnTable
 * 刷新房间拥有属性表格
 */
void RoomOwnAttribute::refreshOwnTable(int roomID)
{
    ui->tablewidget->clearSelection();
    ui->own_tablewidget->clearSelection();
    initRoomOwnAttribue(roomID);
}

/**
 * @brief RoomOwnAttribute::onLikeIndex
 * @param index
 * 接收likeIndex
 */
void RoomOwnAttribute::onLikeIndex(int index)
{
    m_likeIndex.append(index);
}

/**
 * @brief RoomOwnAttribute::onSelectBtnClicked
 * 查询按钮被按下
 */
void RoomOwnAttribute::onSelectBtnClicked()
{
    m_dialog->show();
}

/**
 * @brief RoomOwnAttribute::onDiaSelectBtnClicked
 * @param maps where的值
 * 当查询对话框中 “查询” 按钮被按下，接收他发出的信号
 */
void RoomOwnAttribute::onDiaSelectBtnClicked(QVariantMap maps)
{
    m_whereField.clear();
    m_whereValue.clear();
    m_likeIndex.clear();

    //遍历
    QMap<QString,QVariant>::const_iterator ite;
    for(ite = maps.begin(); ite != maps.end(); ite++)
    {
        m_whereField.append(ite.key());
        m_whereValue.append(ite.value());
    }

    QString sql = m_service->selectRoomSql(m_whereField);
    m_model->setSql(sql);
    m_model->select(m_whereValue,m_likeIndex);
}

/**
 * @brief RoomOwnAttribute::onRoomTableViewActivated
 * @param index
 * 当roomTableView项被激活时,查询当前房间拥有的属性
 */
void RoomOwnAttribute::onRoomTableViewActivated(const QModelIndex &index)
{
    if(!index.isValid())
    {
        return;
    }

    int roomId = index.sibling(index.row(),0).data().toInt();

    if(roomId > 0)
    {
        m_roomID = roomId;
        //初始化房间拥有属性表
        refreshOwnTable(m_roomID);
    }
    else
    {
        QMessageBox::information(this,"提示","当前房间ID错误，请稍后重试");
        return;
    }
}

/**
 * @brief RoomOwnAttribute::onAddBtnClicked
 * 当添加 左箭头 按钮被按下，新增一条选中房间的房间属性
 */
void RoomOwnAttribute::onAddBtnClicked()
{
    if(m_roomID <= 0)
    {
        QMessageBox::information(this,"提示","当前并没有选中房间信息");
        return;
    }

    if(!ui->tablewidget->currentIndex().isValid())
    {
        QMessageBox::information(this,"提示","当前并没有选中属性信息");
        return;
    }

    int attribueID = ui->tablewidget->currentIndex().data(Qt::UserRole).toInt();

    if(attribueID <= 0)
    {
        QMessageBox::information(this,"提示","当前选中的属性信息错误，请刷新页面后重试");
        return;
    }

    if(m_service->insertCheck(m_roomID,attribueID)<=0)
    {
        if(m_service->insert(m_roomID,attribueID))
        {

            QMessageBox::information(this,"提示","新增成功");
            refreshOwnTable(m_roomID);
        }
        else
        {
            QMessageBox::information(this,"提示","新增失败");
        }
    }
    else
    {
        QMessageBox::information(this,"提示","当前属性已存在，请选择其它属性");
    }
}

/**
 * @brief RoomOwnAttribute::onDeleteBtnClicked
 * 当删除按钮被按下，删除
 */
void RoomOwnAttribute::onDeleteBtnClicked()
{
    int btn = QMessageBox::question(this,"提示","确定要删除吗？");
    if(btn == QMessageBox::Yes)
    {
        if(m_roomID <= 0)
        {
            QMessageBox::information(this,"提示","当前并没有选中房间信息");
            return;
        }

        if(!ui->own_tablewidget->currentIndex().isValid())
        {
            QMessageBox::information(this,"提示","当前并没有选中房间拥有的属性信息");
            return;
        }

        int OwnAttribueID = ui->own_tablewidget->currentIndex().data(Qt::UserRole).toInt();

        qDebug() << ui->own_tablewidget->currentIndex().data() << ui->own_tablewidget->currentIndex().data(Qt::UserRole);

        if(OwnAttribueID <= 0)
        {
            QMessageBox::information(this,"提示","当前选中的属性信息错误，请刷新页面后重试");
            return;
        }

        if(m_service->deleteOwnAttribue(OwnAttribueID))
        {
            QMessageBox::information(this,"提示","删除成功");
            refreshOwnTable(m_roomID);
        }
        else
        {
            QMessageBox::information(this,"提示","删除失败");
        }
    }
}

/**
 * @brief RoomOwnAttribute::initRoomOwnAttribue
 * @param roomID
 * 初始化房间拥有属性表
 */
void RoomOwnAttribute::initRoomOwnAttribue(int roomID)
{
    ui->own_tablewidget->setColumnCount(3);
    ui->own_tablewidget->setHorizontalHeaderLabels(QStringList() << "id" << "代码" << "房间拥有的属性描述");

    QList<RoomOwnAttribueInfo> data = m_service->selectRoomOwnAttribue(roomID);

    ui->own_tablewidget->setRowCount(data.size());

    for(int i = 0; i < data.size(); i++)
    {
        RoomOwnAttribueInfo info = data.at(i);

        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(info.room_own_attribute_id));
        item0->setData(Qt::UserRole,info.room_own_attribute_id);
        item0->setTextAlignment(Qt::AlignCenter);
        ui->own_tablewidget->setItem(i,0,item0);

        QTableWidgetItem *item1 = new QTableWidgetItem(info.code);
        item1->setData(Qt::UserRole,info.room_own_attribute_id);
        item1->setTextAlignment(Qt::AlignCenter);
        ui->own_tablewidget->setItem(i,1,item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(info.attribute);
        item2->setData(Qt::UserRole,info.room_own_attribute_id);
        item2->setTextAlignment(Qt::AlignCenter);
        ui->own_tablewidget->setItem(i,2,item2);
    }

    ui->own_tablewidget->hideColumn(0);
}
