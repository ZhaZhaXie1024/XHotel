#ifndef SYSOPERATOR_H
#define SYSOPERATOR_H

#include <QWidget>

class SysOperatorService;

class QHBoxLayout;
class QVBoxLayout;

class QDialog;
class QLabel;
class QLineEdit;
class QPushButton;
class QTableWidget;

namespace Ui {
class SysOperator;
}

class SysOperator : public QWidget
{
    Q_OBJECT

public:
    explicit SysOperator(QWidget *parent = nullptr);
    ~SysOperator();

    //初始化
    void init();

    //加载表格数据/刷新
    void loadTableData();

    //创建对话框
    void createDialog(QString name = "新增");

    //初始化选择员工表格
    void initStaffTableWidget();


public slots:

    /*-----------新增--------------*/
    //新建按钮被按下
    void onInsertBtnOnClicked();
    //新增对话框中，选择员工按钮被按下
    void onDiaSelectStaffBtnClicked();
    //新增对话框中选择员工对话框中的查询按钮被按下
    void onInsDiaDiaSelBtnClicked();
    //选择员工对话框表格被双击
    void onStaffWidgetDClicked(const QModelIndex &index);
    //对话框中，新增按钮被按下(真正的保存)
    void onInsDiaInsBtnClicked();


    /*-----------修改--------------*/
    //修改按钮被按下
    void onUpdateBtnClicked();
    //对话框中，修改按钮被按下
    void onUpDiaUpdateClicked();
    //对话框中，取消按钮被按下
    void onDiaCanBtnClicked();

    //删除按钮被按下
    void onDelBtnOnClicked();

private:
    Ui::SysOperator *ui;

    SysOperatorService *m_service;

    int m_operatorID;
    int m_staffID;

    //控件
private:
    QVBoxLayout *m_mainLayout;      //主布局
    QHBoxLayout *m_pasLayout;       //密码布局
    QVBoxLayout *m_pasLabelLayout;  //密码Lable布局
    QVBoxLayout *m_pasEditLayout;   //密码Edit布局
    QHBoxLayout *m_btnLayout;       //按钮布局
    QVBoxLayout *m_staffLayout;     //staff布局
    QHBoxLayout *m_staffEditLayout; //staffEdit布局
    QDialog *m_dialog;              //新增/修改对话框
    QDialog *m_staffDialog;         //员工对话框

    QLabel *m_nullLabel;                //空l
    QLabel *m_passwordLabel;            //密码l
    QLabel *m_verifyPasswordbeLabel;    //确认密码l
    QPushButton *m_selectStaff;         //选择员工信息按钮
    QLineEdit *m_passwordEdit;          //密码e
    QLineEdit *m_verifyPasswordbeEdit;  //确认密码e


    QPushButton *m_insertOKBtn;         //确定新增按钮
    QPushButton *m_updateOKBtn;         //确定修改按钮
    QPushButton *m_cancel;              //取消，关闭对话框按钮

    QLabel *m_staffNameLabel;           //员工名称l
    QLineEdit *m_staffNameEdit;         //员工名称e
    QLabel *m_staffIdCardLabel;         //身份证l
    QLineEdit *m_staffIdCardEdit;       //身份证e
    QLabel *m_staffPhoneLabel;          //电话l
    QLineEdit *m_staffPhoneEdit;        //电话e
    QPushButton *m_staffDiaSelect;      //员工查询按钮
    QTableWidget *m_staffWidget;        //选择员工表格
};


#endif // SYSOPERATOR_H
