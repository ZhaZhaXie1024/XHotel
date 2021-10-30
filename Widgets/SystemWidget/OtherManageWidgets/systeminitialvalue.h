#ifndef SYSTEMINITIALVALUE_H
#define SYSTEMINITIALVALUE_H

#include <QWidget>
#include <QVariant>

class TeminitialValueService;
class TeminitialValueDialog;

namespace Ui {
class SystemInitialValue;
}

class SystemInitialValue : public QWidget
{
    Q_OBJECT

public:
    explicit SystemInitialValue(QWidget *parent = nullptr);
    ~SystemInitialValue();

    void init();

    //加载表格数据
    void loadData();

public slots:
    /*-----------新增--------------*/
    //新建按钮被按下
    void onInsertBtnOnClicked();

    //对话框中，新增按钮被按下(真正的保存)
    void onInsDiaInsBtnClicked(QVariantList values);

    //删除按钮被按下------------
    void onDelBtnOnClicked();

private:
    Ui::SystemInitialValue *ui;

    TeminitialValueService* m_service;
    TeminitialValueDialog *m_dialog;
};

#endif // SYSTEMINITIALVALUE_H
