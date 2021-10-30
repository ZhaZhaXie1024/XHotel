#ifndef SYSCHARGEITEM_H
#define SYSCHARGEITEM_H

#include <QWidget>

class QHBoxLayout;
class QVBoxLayout;

class QDialog;
class QLabel;
class QLineEdit;
class QPushButton;

class ChargeItemService;

namespace Ui {
class SysChargeItem;
}

class SysChargeItem : public QWidget
{
    Q_OBJECT

public:
    explicit SysChargeItem(QWidget *parent = nullptr);
    ~SysChargeItem();

    void init();

    //创建对话框
    void createDialog(QString name = "新增");

    //加载数据
    void loadTableData();

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
    Ui::SysChargeItem *ui;

    ChargeItemService *m_service;

    int charge_item_id;

    //对话框控件
private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_contentLayout;
    QVBoxLayout *m_labelLayout;
    QVBoxLayout *m_editLayout;
    QHBoxLayout *m_btnLayout;

    QDialog *m_dialog;
    QLabel *m_jeLabel;
    QLabel *m_desLabel;

    QLineEdit *m_jeEdit;
    QLineEdit *m_desEdit;

    QPushButton *m_cancel;

    QPushButton *m_insertOKBtn;
    QPushButton *m_updateOKBtn;
};

#endif // SYSCHARGEITEM_H
