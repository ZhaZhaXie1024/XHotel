#ifndef ROOM_H
#define ROOM_H

#include <QWidget>
#include <QVariant>

class RoomService;
class RoomFilterDialog;
class CustomTableViewModel;

class UserComboBoxDeletage;
class SpinBoxDelegate;
class LineEditDelegate;

namespace Ui {
class Room;
}

class Room : public QWidget
{
    Q_OBJECT

public:
    explicit Room(QWidget *parent = nullptr);
    ~Room();

    void init();

    //初始化 显示数据 下拉框
    void initComboBox();

    //初始化连接
    void initConnect();

    //获取当前页
    int getThisPage();

    //select
    void select();
    void select(int thisPage);

    //首页
    void firstPage();
    //上一页
    void previousPage();
    //下一页
    void nextPage();
    //尾页
    void lastPage();
    //跳转页
    void skipPage();

    //---------------查询部分槽函数
public slots:
    //model中总页数改变槽函数
    void onPageChanged(int pageCount);
    //当页面的显示条数改变
    void onComShowCountChanged(const QString text);

    //查询按钮点击时
    void onSelectBtnClicked();
    //当查询对话框中 “查询” 按钮被按下，接收他发出的信号
    void onDiaSelectBtnClicked(QVariantMap maps);
    //在service中拼接sql语句中出现like，触发该槽
    void onJoinSqlInLike(int index);
    //刷新按钮被按下
    void onRefreshBtnClicked();

    //---------------新增部分槽函数
public slots:
    //在tableView中新增一行,新增按钮被按下
    void onInsertBtnClicked();

    //新增/修改发生错误
    void sqlError(QString error);
    //双击进行修改，同时进行事务处理
    void onTableViewDoubleClicked(const QModelIndex &index);

    //提交
    void onCommitBtnClicked();

    //回滚
    void onrollBackBtnClicked();

protected:
    //窗体大小改变事件
    void resizeEvent(QResizeEvent *event) override;

    //插入数据到 显示数据 下拉框
    void insertToComboBox(int row);

    //---------------新增和修改的样式控制-------------------
    //普通按钮的显示
    void btnComDisplay(bool visible);
    //提交/回滚按钮的显示
    void btnSubDispaly(bool visible);

    //创建委托
    void createDelegate();
    //绑定委托
    void boundDelegate();

private:
    Ui::Room *ui;

    RoomService *m_service;

    RoomFilterDialog *m_dialog;

    CustomTableViewModel *m_model;

    //查询筛选数据 容器
    QStringList m_whereField;
    QVariantList m_whereValue;

    //标记like参数的index
    QList<int> m_likeIndex;

    //委托
    UserComboBoxDeletage *m_towerRangeDeletage;
    UserComboBoxDeletage *m_towerSeatDeletage;
    UserComboBoxDeletage *m_roomTypeDeletage;
    UserComboBoxDeletage *m_roomStateDeletage;
    SpinBoxDelegate *m_towerTierDelegate;
    LineEditDelegate *m_room_numDelegate;

};

#endif // ROOM_H
