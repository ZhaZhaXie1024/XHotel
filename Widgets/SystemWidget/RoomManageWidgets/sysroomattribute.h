#ifndef SYSROOMATTRIBUTE_H
#define SYSROOMATTRIBUTE_H

#include <QWidget>

class RoomAttribueService;

class QHBoxLayout;
class QVBoxLayout;

class QDialog;
class QLabel;
class QLineEdit;
class QPushButton;

namespace Ui {
class SysRoomAttribute;
}

class SysRoomAttribute : public QWidget
{
    Q_OBJECT

public:
    explicit SysRoomAttribute(QWidget *parent = nullptr);
    ~SysRoomAttribute();

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
    Ui::SysRoomAttribute *ui;

    RoomAttribueService *m_service;

    int m_room_attribute_id;

    //对话框控件
private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_contentLayout;
    QVBoxLayout *m_labelLayout;
    QVBoxLayout *m_editLayout;
    QHBoxLayout *m_btnLayout;

    QDialog *m_dialog;
    QLabel *m_dmLabel;
    QLabel *m_desLabel;

    QLineEdit *m_dmEdit;
    QLineEdit *m_desEdit;

    QPushButton *m_cancel;

    QPushButton *m_insertOKBtn;
    QPushButton *m_updateOKBtn;

};

#endif // SYSROOMATTRIBUTE_H
