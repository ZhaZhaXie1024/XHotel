#ifndef SYSTOWERRANGE_H
#define SYSTOWERRANGE_H

#include <QWidget>

class TowerRangeServie;

class QHBoxLayout;
class QVBoxLayout;

class QDialog;
class QLabel;
class QLineEdit;
class QPushButton;

namespace Ui {
class SysTowerRange;
}

class SysTowerRange : public QWidget
{
    Q_OBJECT

public:
    explicit SysTowerRange(QWidget *parent = nullptr);
    ~SysTowerRange();

    //初始化
    void init();

    //加载表格数据/刷新
    void loadTableData();

    //创建对话框
    void createDialog(QString name = "新增");

public slots:
    /*-----------新增--------------*/
    //新建按钮被按下
    void onInsertBtnOnClicked();
    //对话框中，新增按钮被按下(真正的保存)
    void onInsDiaInsBtnClicked();

    /*-----------修改--------------*/
    //修改按钮被按下
    void onUpdateBtnClicked();
    //对话框中，修改按钮被按下
    void onUpDiaUpdateClicked();


    /*---------- 其它--------------*/

    //对话框中，取消按钮被按下
    void onDiaCanBtnClicked();

    //删除按钮被按下
    void onDelBtnOnClicked();

private:
    Ui::SysTowerRange *ui;
    TowerRangeServie *m_service;

    //id
    int m_towerRangeID;

    //对话框控件
private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_contentLayout;
    QVBoxLayout *m_labelLayout;
    QVBoxLayout *m_editLayout;
    QHBoxLayout *m_btnLayout;

    QDialog *m_dialog;
    QLabel *m_dMLabel;
    QLabel *m_describeLabel;
    QLabel *m_describeLabel2;
    QLineEdit *m_dMEdit;
    QLineEdit *m_describeEdit;
    QLineEdit *m_describeEdit2;

    QPushButton *m_cancel;

    QPushButton *m_insertOKBtn;
    QPushButton *m_updateOKBtn;
};

#endif // SYSTOWERRANGE_H
