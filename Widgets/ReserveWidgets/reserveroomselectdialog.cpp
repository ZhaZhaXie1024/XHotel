#include "reserveroomselectdialog.h"
#include "ui_reserveroomselectdialog.h"

//#include <QDate>
#include <QDebug>
#include "Service/comboboxitemservice.h"
#include "Entitys/comboboxentity.h"

ReserveRoomSelectDialog::ReserveRoomSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReserveRoomSelectDialog)
{
    ui->setupUi(this);
    init();
}

ReserveRoomSelectDialog::~ReserveRoomSelectDialog()
{
    delete ui;
}

void ReserveRoomSelectDialog::init()
{
    setWindowTitle("查询预订单信息对话框");

    //初始化入住时间
    QDate thisDate = QDate::currentDate();
    ui->dateEdit->setDate(thisDate);

    //初始化下拉框
    ComboboxItemService comService;
    QList<int> sqlLinkSymbol;
    sqlLinkSymbol << 1 << 0;
    comService.init("bill_state_type",QStringList()<<"bill_state_type_id"<<"bill_state",QStringList()<<"code"<<"code",sqlLinkSymbol);
    comService.boundData(ui->com_billState,comService.select(QVariantList()<<"YY"<<"QX"));

    //添加提示
    ui->checkBox->setToolTip("选中启用");

    //全局连接
    connect(ui->btn_select,&QPushButton::clicked,this,&ReserveRoomSelectDialog::onSelectBtnClicked);
    connect(ui->btn_clear,&QPushButton::clicked,this,&ReserveRoomSelectDialog::onClearBtnClicked);
    connect(ui->btn_cancel,&QPushButton::clicked,this,&ReserveRoomSelectDialog::onCancelClicked);
}

/**
 * @brief ReserveRoomSelectDialog::onSelectBtnClicked
 * 查询按钮被按下
 */
void ReserveRoomSelectDialog::onSelectBtnClicked()
{
    //获取数据
    QString name = ui->edit_name->text().trimmed();
    QString phone = ui->edit_phone->text().trimmed();
    QString billNum = ui->edit_billNum->text().trimmed();

    QDate date;

    if(ui->checkBox->isChecked())
    {
        date = ui->dateEdit->date();
    }
    int stateID = ui->com_billState->currentData().toInt();

    emit select(name,phone,billNum,date,stateID);
}

/**
 * @brief ReserveRoomSelectDialog::onClearBtnClicked
 * 清空按钮被按下
 */
void ReserveRoomSelectDialog::onClearBtnClicked()
{
    ui->edit_name->clear();
    ui->edit_phone->clear();
    ui->edit_billNum->clear();
    ui->com_billState->setCurrentIndex(0);
    ui->dateEdit->clear();
}

/**
 * @brief ReserveRoomSelectDialog::onCancelClicked
 * 取消按钮被按下
 */
void ReserveRoomSelectDialog::onCancelClicked()
{
    this->close();
}
