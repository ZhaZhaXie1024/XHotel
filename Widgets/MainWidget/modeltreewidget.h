#ifndef MODELTREEWIDGET_H
#define MODELTREEWIDGET_H

#include <QWidget>

class BaseDao;
class QTreeWidgetItem;

namespace Ui {
class ModelTreeWidget;
}

class ModelTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModelTreeWidget(QWidget *parent = nullptr);
    ModelTreeWidget(int operator_id,QWidget *parent = nullptr);

    ~ModelTreeWidget();

    //初始化
    void init();

    //查询模块
    void selectModel();


public slots:

    //item点击事件
    void onTreeWidgetItemClicked(QTreeWidgetItem *item, int column);

    //通过widget.cpp中转，在centrewidget中发出;取消选中项
    void cancelSelectedItem();

    //widget中转，设置选中项
    void setCurrentItem(QString itemStr);

signals:
    //item点击事件中 发送该信号，CentreWidget中接收
    void treeWidgetItemClicked(QString widgetName);

private:
    Ui::ModelTreeWidget *ui;

    //操作员ID
    int m_operator_id;

    BaseDao *m_baseDao;
};

#endif // MODELTREEWIDGET_H
