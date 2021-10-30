#ifndef SYSDEPARTMENT_H
#define SYSDEPARTMENT_H

#include <QWidget>

//提供一个可修改的Model
class QSqlTableModel;

class QHBoxLayout;
class QVBoxLayout;

class QDialog;
class QLabel;
class QLineEdit;
class QPushButton;


//服务层
class SysDepartmentService;

namespace Ui {
class SysDepartment;
}

class SysDepartment : public QWidget
{
    Q_OBJECT

public:
    explicit SysDepartment(QWidget *parent = nullptr);
    ~SysDepartment();

    void init();

    //创建一个对话框
    void createDialog(QString name = "新增");

    //启用/关闭提交和回滚按钮
    void setEnabledInButton(bool enable);

public slots:
    //新建按钮被按下
    void onInsertBtnOnClicked();
    //修改按钮被按下
    void onUpdateBtnClicked();
    //对话框中，新增按钮被按下
    void onInsDiaInsBtnClicked();
    //对话框中，修改按钮被按下
    void onUpDiaUpdateClicked();
    //对话框中，取消按钮被按下
    void onDiaCanBtnClicked();

    //删除按钮被按下
    void onDelBtnOnClicked();
    //刷新按钮被按下
    void onRefBtnOnClicked();

    //提交按钮被按下时
    void onSubmitBtnClicked();

    //回滚按钮被按下时
    void onRollbackBtnClicked();

    //当model数据被修改时
    void onModelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());



private:
    Ui::SysDepartment *ui;

    QSqlTableModel *m_model;

    SysDepartmentService *m_service;

    //控件
private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_btnLayout;
    QDialog *m_Dialog;
    QLabel *m_DMLabel;
    QLabel *m_DescribeLabel;
    QLineEdit *m_DMEdit;
    QLineEdit *m_DescribeEdit;
    QPushButton *m_Cancel;

    QPushButton *m_insertOKBtn;
    QPushButton *m_updateOKBtn;

    //数据
private:
    int m_depID;
};

#endif // SYSDEPARTMENT_H
