#include "staffdialog.h"
#include "ui_staffdialog.h"

#include <QMessageBox>
#include <QCalendarWidget>
#include <QToolButton>


#include <QDebug>

#include "Entitys/comboboxentity.h"
#include "Service/comboboxitemservice.h"

StaffDialog::StaffDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StaffDialog),
    m_name("新增")
{
    ui->setupUi(this);
    init();
}

StaffDialog::StaffDialog(QString name,QWidget *parent):
    QDialog(parent),
    ui(new Ui::StaffDialog),
    m_name(name)
{
    ui->setupUi(this);
    init();
}

StaffDialog::~StaffDialog()
{
    delete ui;
}

void StaffDialog::init()
{
    initMemberValue();

    initStyle();

    m_serive = new ComboboxItemService(this);
    initComboBox();


    //取消按钮
    connect(ui->btn_cancel,&QPushButton::clicked,this,&StaffDialog::onCancelBtnClicked);
    //新增按钮
    connect(ui->btn_insert,&QPushButton::clicked,this,&StaffDialog::onInsertBtnClicked);
    //修改按钮
    connect(ui->btn_update,&QPushButton::clicked,this,&StaffDialog::onUpdateBtnClicked);
}

/**
 * @brief StaffDialog::initStyle
 * 初始化对话框样式
 */
void StaffDialog::initStyle()
{
    setName(m_name);

    this->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowCloseButtonHint);
    this->setModal(true);

    //设置当前时间给日历
    ui->dateEdit->setDate(QDate::currentDate());

    //日历弹出层
    QCalendarWidget *calendar = ui->dateEdit->calendarWidget();
    //开启网格
    calendar->setGridVisible(true);

    //添加匹配手机号的正则表达式
    QRegExp rx("^1([358][0-9]|4[579]|66|7[0135678]|9[89])[0-9]{8}$");
    QRegExpValidator *validator = new QRegExpValidator(rx, this);
    ui->line_phoneEdit->setValidator(validator);
}

/**
 * @brief StaffDialog::setName
 * @param name
 * 设置名,清空表单,隐藏指定按钮
 */
void StaffDialog::setName(QString name)
{
    initMemberValue();

    QString tempName = "新增";
    ui->line_nameEdit->clear();
    ui->line_phoneEdit->clear();
    ui->line_idCardEdit->clear();

    ui->com_dep->setCurrentIndex(0);
    ui->com_pos->setCurrentIndex(0);
    ui->com_sex->setCurrentIndex(0);

    if(name.trimmed().isEmpty())
    {
        name = tempName;
    }
    else
    {
        m_name = name;
    }

    setWindowTitle(name);

    if(m_name.compare("修改") == 0)
    {
        ui->btn_insert->hide();
        ui->i1->hide();
        ui->i2->hide();
        ui->i3->hide();

        ui->btn_update->show();
        ui->u1->show();
        ui->u2->show();
        ui->u3->show();
    }
    else
    {
        ui->btn_update->hide();
        ui->u1->hide();
        ui->u2->hide();
        ui->u3->hide();

        ui->btn_insert->show();
        ui->i1->show();
        ui->i2->show();
        ui->i3->show();
    }
}

/**
 * @brief StaffDialog::initComboBox
 * 初始化所有下拉框
 */
void StaffDialog::initComboBox()
{
    initSexComboBox();
    initDepComboBox();
    initPosComboBox();
}

/**
 * @brief StaffDialog::initMemberValue
 * 初始化成员变量
 */
void StaffDialog::initMemberValue()
{
    m_staffName = "";
    m_idCard = "";
    m_phone = "";

    m_depID = 0;
    m_posID = 0;
    m_sex = "";
}

/**
 * @brief StaffDialog::initSexComboBox
 * 初始化性别下拉框
 */
void StaffDialog::initSexComboBox()
{
    QList<QVariantMap> mapList;
    QVariantMap map;
    map.insert("男","男");
    QVariantMap map1;
    map1.insert("女","女");
    mapList.append(map);
    mapList.append(map1);
    m_serive->boundData(ui->com_sex,mapList);
}

/**
 * @brief StaffDialog::initDepComboBox
 * 初始化部门下拉框
 */
void StaffDialog::initDepComboBox()
{
    m_serive->init("department",QStringList() << "department_id" << "m_describe");
    m_serive->boundData(ui->com_dep,m_serive->select());
}

/**
 * @brief StaffDialog::initPosComboBox
 * 初始化职业下拉框
 */
void StaffDialog::initPosComboBox()
{
    m_serive->init("staff_position",QStringList() << "staff_position_id" << "m_describe");
    m_serive->boundData(ui->com_pos,m_serive->select());
}

/**
 * @brief StaffDialog::getData
 * 获取数据
 */
void StaffDialog::getData()
{
    m_staffName = ui->line_nameEdit->text();
    m_phone = ui->line_phoneEdit->text();
    m_idCard = ui->line_idCardEdit->text();
    m_sex = ui->com_sex->currentText();

    m_depID = ui->com_dep->currentData().toInt();
    m_posID = ui->com_pos->currentData().toInt();

    m_date = ui->dateEdit->date();
}

/**
 * @brief StaffDialog::isValuesEmpty
 * @return true:空,false:不空
 * 判断需要的数据是否为空
 */
bool StaffDialog::isValuesEmpty()
{
    if(m_staffName.trimmed().isEmpty() || m_phone.trimmed().isEmpty() ||
            m_idCard.trimmed().isEmpty() || (m_sex.trimmed().isEmpty() && m_sex.compare("-- 请选择 --") != 0))
    {
        return true;
    }

    if(m_depID <= 0 || m_posID <= 0)
    {
        return true;
    }

    return false;
}

/**
 * @brief StaffDialog::BackfillData
 * @param name
 * @param sex
 * @param iphone
 * @param dep
 * @param pos
 * @param date
 * 当主页面点击修改，回填数据
 */
void StaffDialog::backfillData(QString name, QString sex, QString iphone, QString dep, QString pos,QString idCard, QDate date)
{
    ui->line_nameEdit->setText(name);
    ui->com_sex->setCurrentText(sex);
    ui->line_phoneEdit->setText(iphone);
    ui->com_dep->setCurrentText(dep);
    ui->com_pos->setCurrentText(pos);
    ui->line_idCardEdit->setText(idCard);
    ui->dateEdit->setDate(date);
}

/**
 * @brief StaffDialog::onCancelBtnClicked
 * 关闭对话框
 */
void StaffDialog::onCancelBtnClicked()
{
    this->close();
}

/**
 * @brief StaffDialog::onInsertBtnClicked
 * 新增按钮被按下
 */
void StaffDialog::onInsertBtnClicked()
{
    //获取数据
    getData();

    //判断
    if(isValuesEmpty())
    {
        QMessageBox::critical(this,"错误","请填写完整!");
        return;
    }

    emit insertBtnOnClicked(m_staffName,m_phone,m_idCard,m_sex,m_depID,m_posID,m_date);
}

/**
 * @brief StaffDialog::onUpdateBtnClicked
 * 修改按钮被按下
 */
void StaffDialog::onUpdateBtnClicked()
{
    getData();

    //判断
    if(isValuesEmpty())
    {
        QMessageBox::critical(this,"错误","请填写完整!");
        return;
    }

    emit updateBtnOnClicked(m_staffName,m_phone,m_idCard,m_sex,m_depID,m_posID,m_date);
}

