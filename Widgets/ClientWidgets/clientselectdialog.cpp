#include "clientselectdialog.h"
#include "ui_clientselectdialog.h"

ClientSelectDialog::ClientSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientSelectDialog)
{
    ui->setupUi(this);

    connect(ui->btn_select,&QPushButton::clicked,this,&ClientSelectDialog::onSelectBtnClicked);
    connect(ui->btn_clear,&QPushButton::clicked,this,&ClientSelectDialog::onClearBtnClicked);
    connect(ui->btn_cancel,&QPushButton::clicked,this,&ClientSelectDialog::onCancelBtnClicked);
}

ClientSelectDialog::~ClientSelectDialog()
{
    delete ui;
}

/**
 * @brief ClientSelectDialog::onSelectBtnClicked
 * 查询按钮被按下，发送数据
 */
void ClientSelectDialog::onSelectBtnClicked()
{

}

/**
 * @brief ClientSelectDialog::onClearBtnClicked
 * 清空按钮被按下
 */
void ClientSelectDialog::onClearBtnClicked()
{
    ui->edit_name->clear();
    ui->edit_phone->clear();
    ui->edit_certificateNum->clear();
}

/**
 * @brief ClientSelectDialog::onCancelBtnClicked
 *取消按钮被按下
 */
void ClientSelectDialog::onCancelBtnClicked()
{
    close();
}


