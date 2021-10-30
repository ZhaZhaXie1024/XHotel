#ifndef STAFF_H
#define STAFF_H

#include <QWidget>
#include <QDate>

class StaffDialog;
class StaffService;
class UserComboBoxDeletage;
class UserLineEditDelegate;
class UserCalendarDelegate;

namespace Ui {
class Staff;
}

class Staff : public QWidget
{
    Q_OBJECT

private:
    Ui::Staff *ui;
    StaffDialog * m_dialog;
    StaffService * m_service;

    int m_row;
    int m_col;
    int m_thisPage;
    int m_staffID;

    //委托(部门)
    UserComboBoxDeletage *m_depComboBoxDeletage;
    //委托（性别）
    UserComboBoxDeletage *m_sexComboBoxDelegate;
    //委托（职业）
    UserComboBoxDeletage *m_posComboBoxDelegate;

    //委托(电话)
    UserLineEditDelegate *m_phoneEditDelegate;
    //委托（身份证）
    UserLineEditDelegate *m_idCardEditDelegate;

    //委托（入职日期）
    UserCalendarDelegate *m_dateEditDelegate;

    //事件函数------------------------------------------------------------
private:
    //绑定view数据
    void resizeEvent(QResizeEvent *event) override;

    //普通成员函数--------------------------------------------------------
public:
    explicit Staff(QWidget *parent = nullptr);
    ~Staff();

    void init();

    void initConnect();

    void initDalegate();

    void createrDialog(QString name = "新增");

    //普通槽函数--------------------------------------------------------
public slots:
    //设置总页数
    void setPageCount(int num);

    //显示异常
    void showPageError(QString errorStr);

    //获取当前页
    void onThisPageEditOk();

    //刷新
    void onRefreshBtnClicked();


    //新增/修改槽函数-----------------------------------------
public slots:
    //新增,对话框
    void insertBtnClicked();
    //新增
    void insertData(QString staffName,QString phone,
                    QString idCard,QString sex,
                    int depID,int posID,QDate date);

    //修改,对话框
    void updateBtnClicked();
    //修改
    void updateData(QString staffName,
                    QString phone,QString idCard,
                    QString sex,int depID,int posID,QDate date);

    //删除
    void deleteData();

    //表格开始编辑，启动按钮
    void onTableViewChanged(const QModelIndex &index);

    //提交
    void submitData();

    //回滚
    void rollBackData();


    //分页函数----------------------------
public:

    //上一页
    void previousPage();

    //下一页
    void nextPage();

    //首页
    void firstPage();

    //尾页
    void lastPage();


signals:
    //发送信号根据当前页查询
    void thisPageChanged(int page);

    //发送点击修改回填信号
    void upDateBackfillData(QString name, QString sex, QString iphone, QString dep, QString pos,QString idCard, QDate date);


};

#endif // STAFF_H
