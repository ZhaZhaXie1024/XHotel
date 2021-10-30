#ifndef ROOMFILTERDIALOG_H
#define ROOMFILTERDIALOG_H

#include <QDialog>
#include <QVariant>

struct ComboBoxSeleteItem;
struct TowerRangInfo;
struct TowerSeatInfo;
struct RoomTypeInfo;
struct RoomStateInfo;

class ComboboxItemService;

namespace Ui {
class RoomFilterDialog;
}

class RoomFilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RoomFilterDialog(QWidget *parent = nullptr);
    ~RoomFilterDialog();

    //初始化
    void init();

    //初始化下拉框
    void initComboBox();

public slots:

    //查询按钮被按下
    void onSelectBtnClicked();

    //清空按钮被按下
    void onClearBtnClicked();

    //取消按钮被按下
    void onCancelBtnClicked();


private:
    Ui::RoomFilterDialog *ui;

    ComboboxItemService *m_comboBoxService;

signals:
    void select(QVariantMap maps);
};

#endif // ROOMFILTERDIALOG_H
