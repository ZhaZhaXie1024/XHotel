#ifndef RESERVEROOM_H
#define RESERVEROOM_H

#include <QWidget>

class ReserveService;
class QSqlQueryModel;

//房态图对话框
class QDialog;
class QHBoxLayout;
class RoomStateMap;
class ReserveRoomSelectDialog;

#include <QDate>

namespace Ui {
class ReserveRoom;
}

class ReserveRoom : public QWidget
{
    Q_OBJECT

public:
    explicit ReserveRoom(QWidget *parent = nullptr);
    ~ReserveRoom();

    //初始化
    void init();

    //初始化model样式
    void initModel();

    //初始化房态对话框
    void initRoomStateMapDialog();

    //初始化样式
    void initStyle();

    //初始化全局信号和槽
    void initConnect();

    //设置房间ID
    void setRoom(int room_id,QString room_num,int room_state_id);
    //设置操作员ID
    void setOperatorID(int operator_id);

    //查询
    void select();

public slots:


    //查询使用的槽函数-----------

    //查询按钮被按下
    void onSelectBtnClicked();
    //查询对话框中查询按钮被按下
    void onSelectDialogSelectBtnClicked(QString name,QString phone,QString billNum,QDate date,int stateID);

    //新增使用到的槽函数-----------

    //选择房间被按下
    void onSelectRoomBtnClicked();
    //新增按钮被按下
    void onInsertBtnClicked();

    //修改用到的槽函数------------

    //点击表格某一行，进行回填
    void onTableViewDouClicked(const QModelIndex &index);
    //修改按钮被按下
    void onUpdateBtnClicked();
    //取消修改/解锁
    void onUnLockBtnClicked();

    //其它槽函数---------------

    //单击，清空按钮被按下（清空表单域）
    void onClearBtnClicked();
    //双击，清空表单域的同时清空选中的房间和房间ID-----Qpushbutton没有双击信号
    //void onClearBtnDouClicked();


private:
    Ui::ReserveRoom *ui;

    ReserveService* m_service;
    QSqlQueryModel *m_model;

    //房态图
    QDialog *m_dialog;
    QHBoxLayout *m_mianLayout;
    RoomStateMap *m_roomStateMap;

    //查询对话框
    ReserveRoomSelectDialog *m_selectDialog;

    QString m_sql;

    bool m_isUpdate;        //双击表格一行后，开始修改
    int m_room_id;
    int m_operator_id;

    //修改--
    int m_clientHistoryID;  //客史ID
    int m_billID;           //订单ID

    int this_index;
    int this_showNum;
};

#endif // RESERVEROOM_H
