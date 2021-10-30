#ifndef SYSSTAFFPOSITION_H
#define SYSSTAFFPOSITION_H

#include <QWidget>

namespace Ui {
class SysStaffPosition;
}

class SysStaffPosService;

class QHBoxLayout;
class QVBoxLayout;

class QDialog;
class QLabel;
class QLineEdit;
class QPushButton;

/**
 * @brief The SysStaffPosition class
 * 员工职位管理
 */
class SysStaffPosition : public QWidget
{
    Q_OBJECT

public:
    explicit SysStaffPosition(QWidget *parent = nullptr);
    ~SysStaffPosition();

    //初始化
    void init();

    //加载表格数据/刷新
    void loadTableData();

    //创建对话框
    void createDialog(QString name = "新增");

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


private:
    Ui::SysStaffPosition *ui;

    SysStaffPosService *m_service;

    int m_posId;

    //控件
private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_btnLayout;
    QDialog *m_dialog;
    QLabel *m_DMLabel;
    QLabel *m_DescribeLabel;
    QLineEdit *m_DMEdit;
    QLineEdit *m_DescribeEdit;
    QPushButton *m_Cancel;

    QPushButton *m_insertOKBtn;
    QPushButton *m_updateOKBtn;

};

#endif // SYSSTAFFPOSITION_H
