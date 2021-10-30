#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QHBoxLayout;
class QVBoxLayout;
class HeaderWidget;
class ModelTreeWidget;
class CentreWidget;
class QLabel;
class QTimer;
class QDateTime;


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    Widget(int operator_id,QString operatorName,QWidget *parent = nullptr);
    ~Widget();

    void initLayout();

    void initWidget();

    void initConnect();

public slots:
    //每秒时间溢出执行
    void onTimeOutLabel();

private:
    QVBoxLayout *m_mainLayout;

    QHBoxLayout *m_centreLayout;
    QVBoxLayout *m_modelTreeViewLayout;

    HeaderWidget *m_headerWidget;
    ModelTreeWidget *m_modelTreeWidget;
    CentreWidget *m_centreWidget;
    QLabel *m_timeLabel;

    int m_operatorID;
    QString m_operatorName;

    QTimer *m_timer;
    QDateTime *m_dateTime;
};
#endif // WIDGET_H
