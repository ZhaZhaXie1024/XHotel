#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>

class ClientService;
class QSqlQueryModel;

class QDialog;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QLineEdit;
class QComboBox;
class QDateEdit;

class ClientSelectDialog;

namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    //为1说明是正常打开，为2是预定中选择客户中打开对话框的形式
    explicit Client(QWidget *parent = nullptr,int openFlag = 1);
    ~Client();

    //初始化
    void init();

    //初始化Model样式
    void initModelStyle();

    //初始化全局连接
    void initConnect();

    //查询
    void select();

    //设置Limit
    void setLimit(int index,int showNum);

    //创建新增/修改对话框
    void createIUDialog(QString title);


public slots:
    //查询按钮被按下，打开查询对话框
    void onSelectBtnClicked();

private:
    Ui::Client *ui;

    ClientService *m_service;
    QSqlQueryModel *m_model;

    //查询对话框
    ClientSelectDialog *m_selectDialog;

    //打开形式
    int m_openFlag;

    //sql
    QString m_sql;

    //limit
    int m_index;
    int m_showNum;

    //新增/修改对话框
private:
    //主控件
    QDialog *m_dialog;

    //主布局（垂直）
    QVBoxLayout *m_diaMLayout;    //两个子布局
    //第二层（水平）
    QHBoxLayout *m_diaElmLayout;    //两个子布局
    QHBoxLayout *m_diaBLayout;
    //第三层（垂直）
    QVBoxLayout *m_diaLLayout;    //装label控件
    QVBoxLayout *m_diaEdiLayout;  //装lineedit控件

    //控件
    QLabel *m_label_name;           //姓名L
    QLineEdit *m_edit_name;         //姓名E

    QLabel *m_label_sex;            //性别L
    QComboBox *m_com_sex;           //性别C

    QLabel *m_label_time;           //出生年月L
    QDateEdit *m_de_time;           //出生年月DE

    QLabel *m_label_phone;          //手机号L
    QLineEdit *m_edit_phone;        //手机号E

    QLabel *m_label_certificate;    //证件号L
    QLineEdit *m_edit_certificate;  //证件号E

    QPushButton *m_btn_insert;      //新增按钮
    QPushButton *m_btn_update;      //修改按钮
    QPushButton *m_btn_clear;       //清空按钮
    QPushButton *m_btn_cancel;      //取消按钮
};

#endif // CLIENT_H
