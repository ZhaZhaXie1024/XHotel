#ifndef ROOMSTATEITEMDATADIALOG_H
#define ROOMSTATEITEMDATADIALOG_H

#include <QDialog>

namespace Ui {
class RoomStateItemDataDialog;
}

class RoomStateItemDataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RoomStateItemDataDialog(QWidget *parent = nullptr);
    ~RoomStateItemDataDialog();

    void init();

    //设置位置
    void setPos(QPoint scenePos);

    //设置数据
    void setData(QString room_num,QString tower_rang,QString tower_seat,QString tower_tier,
                 QString bed_num,QString room_state,QString room_type,QString room_price,int flage);


signals:
    void dialogSignal(bool flag);

private:
    Ui::RoomStateItemDataDialog *ui;

    int m_flage;


    // QWidget interface
protected:
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
};

#endif // ROOMSTATEITEMDATADIALOG_H
