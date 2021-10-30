#ifndef CLIENTSELECTDIALOG_H
#define CLIENTSELECTDIALOG_H

#include <QDialog>

namespace Ui {
class ClientSelectDialog;
}

class ClientSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientSelectDialog(QWidget *parent = nullptr);
    ~ClientSelectDialog();

public slots:
    //查询按钮被按下，发送数据
    void onSelectBtnClicked();

    //清空按钮被按下
    void onClearBtnClicked();

    //取消按钮被按下
    void onCancelBtnClicked();

private:
    Ui::ClientSelectDialog *ui;
};

#endif // CLIENTSELECTDIALOG_H
