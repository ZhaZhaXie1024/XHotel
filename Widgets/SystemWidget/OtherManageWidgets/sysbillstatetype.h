#ifndef SYSBILLSTATETYPE_H
#define SYSBILLSTATETYPE_H

#include <QWidget>

class BillStateTypeService;

namespace Ui {
class SysBillStateType;
}

class SysBillStateType : public QWidget
{
    Q_OBJECT

public:
    explicit SysBillStateType(QWidget *parent = nullptr);
    ~SysBillStateType();

    void init();

    //加载数据
    void loadTableData();

public slots:
    //单击表格中任意一
    void onCellClicked(int row,int col);

    //新增按钮被按下
    void oninsertBtnClicked();

    //修改按钮被按下
    void onupdateBtnClicked();

    //删除按钮被按下
    void ondeleteBtnClicked();

private:
    Ui::SysBillStateType *ui;

    BillStateTypeService *m_service;

    int bill_state_type_id;
};

#endif // SYSBILLSTATETYPE_H
