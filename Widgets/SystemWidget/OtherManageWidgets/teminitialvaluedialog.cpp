#include "teminitialvaluedialog.h"
#include "ui_teminitialvaluedialog.h"

#include <QMessageBox>
#include "Service/comboboxitemservice.h"
#include "Entitys/comboboxentity.h"

TeminitialValueDialog::TeminitialValueDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TeminitialValueDialog)
{
    ui->setupUi(this);

    setWindowTitle("新增");
    setWindowFlags(Qt::Dialog|Qt::MSWindowsFixedSizeDialogHint|Qt::WindowCloseButtonHint);

    //初始化类型下拉框
    ComboboxItemService comService;
    comService.init("init_value_type",QStringList()<<"init_value_type_id"<<"state");
    comService.boundData(ui->com_type,comService.select());

    //信号和槽
    connect(ui->btn_insert,&QPushButton::clicked,this,&TeminitialValueDialog::insertBtnClicked);
    connect(ui->btn_cancel,&QPushButton::clicked,this,&TeminitialValueDialog::close);
}

TeminitialValueDialog::~TeminitialValueDialog()
{
    delete ui;
}

/**
 * @brief TeminitialValueDialog::clearInput
 * 清理
 */
void TeminitialValueDialog::clearInput()
{
    ui->edit_describe->clear();
    ui->edit_prefix->clear();
    ui->spin_init_value->setValue(0);
}


/**
 * @brief TeminitialValueDialog::insertBtnClicked
 * 新增按钮被按下
 */
void TeminitialValueDialog::insertBtnClicked()
{
    int type = ui->com_type->currentData().toInt();
    QString describe = ui->edit_describe->text().trimmed();
    QString prefix = ui->edit_prefix->text().trimmed();
    int initValue = ui->spin_init_value->value();

    if(type <= 0 || describe.isEmpty() || prefix.isEmpty())
    {
        QMessageBox::warning(this,"提示","数据不能为空,请填写完整！");
        return;
    }

    QVariantList values;

    values << type << describe<<prefix<<initValue<<initValue;

    emit insertData(values);
}
