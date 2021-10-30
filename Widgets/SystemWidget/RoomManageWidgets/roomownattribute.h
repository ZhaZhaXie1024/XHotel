#ifndef ROOMOWNATTRIBUTE_H
#define ROOMOWNATTRIBUTE_H

#include <QWidget>
#include <QVariant>

class RoomOwnAttributeService;
class CustomTableViewModel;
class RoomFilterDialog;

namespace Ui {
class RoomOwnAttribute;
}

class RoomOwnAttribute : public QWidget
{
    Q_OBJECT

public:
    explicit RoomOwnAttribute(QWidget *parent = nullptr);
    ~RoomOwnAttribute();

    //初始化连接
    void initConnect();

    //初始化房间表格
    void initRoomTableView();

    //初始化房间属性表格
    void initRoomAttribueWidget();

    //刷新房间拥有属性表格
    void refreshOwnTable(int roomID);

public slots:
    //接收likeIndex
    void onLikeIndex(int index);

    //查询按钮被按下
    void onSelectBtnClicked();
    //当查询对话框中 “查询” 按钮被按下，接收他发出的信号
    void onDiaSelectBtnClicked(QVariantMap maps);

    //当roomTableView项被激活时,查询当前房间拥有的属性
    void onRoomTableViewActivated(const QModelIndex &index);

    //当添加 左箭头 按钮被按下，新增一条选中房间的房间属性
    void onAddBtnClicked();
    //当删除按钮被按下，删除
    void onDeleteBtnClicked();

private:
    //初始化房间拥有属性表
    void initRoomOwnAttribue(int roomID);

private:
    Ui::RoomOwnAttribute *ui;

    RoomOwnAttributeService *m_service;
    CustomTableViewModel *m_model;
    RoomFilterDialog *m_dialog;

    //条件查询Room的like索引
    QList<int> m_likeIndex;
    QStringList m_whereField;
    QVariantList m_whereValue;

    int m_roomID;       //房间ID
};

#endif // ROOMOWNATTRIBUTE_H
