#ifndef RESERVEROOMSELECTDIALOG_H
#define RESERVEROOMSELECTDIALOG_H

#include <QDialog>
#include <QDate>

namespace Ui {
class ReserveRoomSelectDialog;
}

class ReserveRoomSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReserveRoomSelectDialog(QWidget *parent = nullptr);
    ~ReserveRoomSelectDialog();

    void init();

public slots:
    //查询按钮被按下
    void onSelectBtnClicked();

    //清空按钮被按下
    void onClearBtnClicked();

    //取消按钮被按下
    void onCancelClicked();

signals:
    void select(QString name,QString phone,QString billNum,QDate date,int stateID);

private:
    Ui::ReserveRoomSelectDialog *ui;
};

#endif // RESERVEROOMSELECTDIALOG_H
