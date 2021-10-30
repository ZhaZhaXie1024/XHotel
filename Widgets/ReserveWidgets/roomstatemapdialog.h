#ifndef ROOMSTATEMAPDIALOG_H
#define ROOMSTATEMAPDIALOG_H

#include <QDialog>

namespace Ui {
class RoomStateMapDialog;
}

class RoomStateMapDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RoomStateMapDialog(QWidget *parent = nullptr);
    ~RoomStateMapDialog();

    void init();

public slots:
    //查询按钮被按下
    void onSelectBtnClicked();

    //清空按钮被按下
    void onClearBtnClicked();

    //取消按钮被按下
    void onCancelBtnClicked();

private:
    Ui::RoomStateMapDialog *ui;

signals:
    //发送数据
    void selectKeyValue(int trId,int tsId,int typeId,int stateId,int bedNum,int price);
};

#endif // ROOMSTATEMAPDIALOG_H
