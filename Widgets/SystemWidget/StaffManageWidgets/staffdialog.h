#ifndef STAFFDIALOG_H
#define STAFFDIALOG_H

#include <QDialog>
#include <QDate>

class ComboBoxSeleteItem;
class ComboboxItemService;
class QDate;

namespace Ui {
class StaffDialog;
}

class StaffDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StaffDialog(QWidget *parent = nullptr);

    StaffDialog(QString name,QWidget *parent = nullptr);

    ~StaffDialog();

    //初始化
    void init();
    //初始化对话框样式
    void initStyle();
    //初始化下拉框
    void initComboBox();
    //初始化成员变量
    void initMemberValue();

    //设置name,显示或隐藏个别按钮
    void setName(QString name = "新增");



    //性别下拉框
    void initSexComboBox();
    //部门下拉框
    void initDepComboBox();
    //职业下拉框
    void initPosComboBox();

private:
    //获取数据
    void getData();

    //判断需要的数据是否为空
    bool isValuesEmpty();

public slots:
    //当主页面点击修改，回填数据
    void backfillData(QString name,QString sex,QString iphone,QString dep,QString pos,QString idCard,QDate date);

    //关闭对话框
    void onCancelBtnClicked();

    //新增按钮被按下
    void onInsertBtnClicked();

    //修改按钮被按下
    void onUpdateBtnClicked();


private:
    Ui::StaffDialog *ui;

    //对话框名称，判断是新增还是修改
    QString m_name;

    ComboboxItemService *m_serive;

    //数据
    QString m_staffName;    //用户名
    QString m_idCard;       //身份证
    QString m_phone;        //电话号码

    int m_depID;            //部门ID
    int m_posID;            //职业ID
    QString m_sex;          //性别

    QDate m_date;           //入职日期

signals:
    //新增按钮被按下,同时符合条件后,发送该信号
    void insertBtnOnClicked(QString staffName,QString phone,
                            QString idCard,QString sex,
                            int depID,int posID,QDate date);

    //修改按钮被按下,同时符合条件后,发送该信号
    void updateBtnOnClicked(QString staffName,QString phone,
                            QString idCard,QString sex,
                            int depID,int posID,QDate date);
};

#endif // STAFFDIALOG_H
