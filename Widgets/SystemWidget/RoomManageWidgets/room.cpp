#include "room.h"
#include "ui_room.h"

#include <QDebug>
#include <QListView>
#include <QMessageBox>
#include <QVariant>

#include "Service/SystemService/RoomManageService/roomservice.h"
#include "Widgets/SystemWidget/RoomManageWidgets/roomfilterdialog.h"
#include "Custom/customtableviewmodel.h"

#include "Delegate/usercomboboxdeletage.h"
#include "Delegate/spinboxdelegate.h"
#include "Delegate/lineeditdelegate.h"

Room::Room(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Room)
{
    ui->setupUi(this);
    init();
}

Room::~Room()
{
    delete ui;
}

void Room::init()
{
    m_service = new RoomService(this);
    m_dialog = new RoomFilterDialog(this);
    m_model = new CustomTableViewModel(this);

    QString sql = m_service->selectSql();
    m_model->setSql(sql,QStringList()<<"room_id"<<"房号"<<"楼区"<<"楼座"<<"楼层"<<"房间类型"<<"房间状态");
    QString insertSql = m_service->insertSql();
    m_model->setInsertSql(insertSql);

    //初始化样式
    ui->com_showCount->setView(new QListView());        //使QSS设置QComboBox每一项的高度生效
    //设置当前页的输入范围 QIntValidator整形的过滤器
    QIntValidator *intValidator = new QIntValidator;
    intValidator->setRange(0, 1000);
    ui->edit_thisPage->setValidator(intValidator);

    initComboBox();
    initConnect();
    createDelegate();
    btnSubDispaly(false);
}

/**
 * @brief Room::initComboBox
 * 初始化显示页数下拉框
 */
void Room::initComboBox()
{
    ui->com_showCount->clear();

    ui->com_showCount->addItems(QStringList()<<"10"<<"15");
}

/**
 * @brief Room::initConnect
 * 初始化连接
 */
void Room::initConnect()
{
    //总页数槽
    connect(m_model,&CustomTableViewModel::pageCountChanged,this,&Room::onPageChanged);
    //当显示条数下拉框改变
    connect(ui->com_showCount,&QComboBox::currentTextChanged,this,&Room::onComShowCountChanged);

    //查询按钮被按下
    connect(ui->btn_select,&QPushButton::clicked,this,&Room::onSelectBtnClicked);
    //查询对话框中查询按钮被按下
    connect(m_dialog,&RoomFilterDialog::select,this,&Room::onDiaSelectBtnClicked);
    //在service中拼接sql语句中出现like，触发该槽
    connect(m_service,&RoomService::onlikeIndex,this,&Room::onJoinSqlInLike);
    //刷新按钮被按下
    connect(ui->btn_refresh,&QPushButton::clicked,this,&Room::onRefreshBtnClicked);

    //翻页
    //首页
    connect(ui->btn_first,&QPushButton::clicked,this,&Room::firstPage);
    //上一页
    connect(ui->btn_previous,&QPushButton::clicked,this,&Room::previousPage);
    //下一页
    connect(ui->btn_next,&QPushButton::clicked,this,&Room::nextPage);
    //尾页
    connect(ui->btn_last,&QPushButton::clicked,this,&Room::lastPage);
    //跳转
    connect(ui->edit_thisPage,&QLineEdit::editingFinished,this,&Room::skipPage);
    connect(ui->edit_thisPage,&QLineEdit::returnPressed,this,&Room::skipPage);

    //新增一行
    connect(ui->btn_insert,&QPushButton::clicked,this,&Room::onInsertBtnClicked);
    //双击修改
    connect(ui->tableView,&QTableView::doubleClicked,this,&Room::onTableViewDoubleClicked);
    //model中新增/修改发生的错误提示
    connect(m_model,&CustomTableViewModel::sqlError,this,&Room::sqlError);
    //提交
    connect(ui->btn_commit,&QPushButton::clicked,this,&Room::onCommitBtnClicked);
    //回滚
    connect(ui->btn_rollBack,&QPushButton::clicked,this,&Room::onrollBackBtnClicked);
}

/**
 * @brief Room::insertToComboBox
 * @param row
 * 插入数据到 显示数据 下拉框
 */
void Room::insertToComboBox(int row)
{
    QString rowStr = QString::number(row);

    int currentIndex = ui->com_showCount->findText(rowStr);

    if(currentIndex >= 0)
    {
        ui->com_showCount->setCurrentIndex(currentIndex);
    }
    else
    {
        int count = ui->com_showCount->count();

        for(int i = 0; i < count; i++)
        {
            int num = ui->com_showCount->itemText(i).toInt();

            if(num > row)
            {
                ui->com_showCount->insertItem(i,rowStr,row);
                ui->com_showCount->setCurrentText(rowStr);
                break;
            }
            else
            {
                if(i == count)
                {
                    ui->com_showCount->insertItem(i,rowStr,row);
                    ui->com_showCount->setCurrentText(rowStr);
                }
            }
        }
    }
}

/**
 * @brief Room::btnComDisplay
 * 普通按钮的显示
 */
void Room::btnComDisplay(bool visible)
{
    ui->btn_insert->setVisible(visible);
    ui->btn_delete->setVisible(visible);
    ui->btn_select->setVisible(visible);

    ui->btn_first->setVisible(visible);
    ui->btn_previous->setVisible(visible);
    ui->edit_thisPage->setVisible(visible);
    ui->label->setVisible(visible);
    ui->label_pageCount->setVisible(visible);
    ui->btn_next->setVisible(visible);
    ui->btn_last->setVisible(visible);
    ui->label_2->setVisible(visible);
    ui->com_showCount->setVisible(visible);
}

/**
 * @brief Room::btnSubDispaly
 * @param visible
 * 提交/回滚按钮的显示
 */
void Room::btnSubDispaly(bool visible)
{
    ui->btn_commit->setVisible(visible);
    ui->btn_rollBack->setVisible(visible);
}

/**
 * @brief Room::createDelegate
 * 创建委托
 */
void Room::createDelegate()
{
    m_towerRangeDeletage = new UserComboBoxDeletage(m_service->comboBoxData("tower_range",QStringList()<<"tower_range_id" << "m_describe"),this);
    m_towerSeatDeletage = new UserComboBoxDeletage(m_service->comboBoxData("tower_seat",QStringList()<<"tower_seat_id" << "m_describe"),this);

    m_roomTypeDeletage = new UserComboBoxDeletage(m_service->comboBoxData("room_type",QStringList()<<"room_type_id"<<"m_describe"),this);
    m_roomStateDeletage = new UserComboBoxDeletage(m_service->comboBoxData("room_state",QStringList()<<"room_state_id"<<"m_describe"),this);

    m_towerTierDelegate = new SpinBoxDelegate(this);
    m_room_numDelegate = new LineEditDelegate(this);
}

/**
 * @brief Room::boundDelegate
 * 绑定委托
 */
void Room::boundDelegate()
{
    ui->tableView->setItemDelegateForColumn(1,m_room_numDelegate);
    ui->tableView->setItemDelegateForColumn(2,m_towerRangeDeletage);
    ui->tableView->setItemDelegateForColumn(3,m_towerSeatDeletage);
    ui->tableView->setItemDelegateForColumn(4,m_towerTierDelegate);
    ui->tableView->setItemDelegateForColumn(5,m_roomTypeDeletage);
    ui->tableView->setItemDelegateForColumn(6,m_roomStateDeletage);
}

/**
 * @brief Room::getThisPage
 * @return         thisPage
 * 获取当前页
 */
int Room::getThisPage()
{
    int thisPage = ui->edit_thisPage->text().toInt();
    return thisPage;
}

/**
 * @brief Room::select
 * select()
 */
void Room::select()
{
    m_model->setShowCount(ui->com_showCount->currentText().toInt());
    m_model->setThisPage(1);
    m_model->select(m_whereValue,m_likeIndex);
    ui->tableView->setModel(nullptr);
    ui->tableView->setModel(m_model);
    //隐藏
    ui->tableView->hideColumn(0);
    //绑定委托
    boundDelegate();
    ui->tableView->update();
}

/**
 * @brief Room::select
 * @param thisPage
 * 查询，上一页，下一页，尾页等
 */
void Room::select(int thisPage)
{
    m_model->setThisPage(thisPage);
    m_model->select(m_whereValue,m_likeIndex);
    ui->edit_thisPage->setText(QString::number(thisPage));
    ui->tableView->update();
}

/**
 * @brief Room::firstPage
 * 首页
 */
void Room::firstPage()
{
    if(ui->edit_thisPage->text().toInt() == 1)
    {
        QMessageBox::information(this,"提示","这已经是第一页啦！");
        return;
    }
    select(1);
}

/**
 * @brief Room::previousPage
 * 上一页
 */
void Room::previousPage()
{
    int page = getThisPage() - 1;
    if(page <= 0)
    {
        QMessageBox::information(this,"提示","这已经是第一页啦！");
        return;
    }
    select(page);
}

/**
 * @brief Room::nextPage
 * 下一页
 */
void Room::nextPage()
{
    int page = getThisPage() + 1;
    int lastPage = ui->label_pageCount->text().toInt();
    if(lastPage <= 0)
    {
        QMessageBox::information(this,"提示","数据错误！");
        return;
    }
    if(page > lastPage)
    {
        QMessageBox::information(this,"提示","这已经是最后一页啦！");
        return;
    }
    select(page);
}

/**
 * @brief Room::lastPage
 * 尾页
 */
void Room::lastPage()
{
    int lastPage = ui->label_pageCount->text().toInt();
    if(lastPage <= 0)
    {
        QMessageBox::information(this,"提示","数据错误！");
        return;
    }

    if(ui->edit_thisPage->text().toInt() == lastPage)
    {
        QMessageBox::information(this,"提示","这已经是第一页啦！");
        return;
    }

    select(lastPage);
}

/**
 * @brief Room::skipPage
 * 跳转页
 */
void Room::skipPage()
{
    int page = ui->edit_thisPage->text().toInt();

    if(page <= 0 || page > ui->label_pageCount->text().toInt())
    {
        QMessageBox::information(this,"提示","数据错误！");
        return;
    }

    select(page);
}

/**
 * @brief Room::onPageChanged
 * @param pageCount
 * model中总页数改变槽函数
 */
void Room::onPageChanged(int pageCount)
{
//    qDebug() << pageCount;
    ui->label_pageCount->setNum(pageCount);
}

/**
 * @brief Room::onComShowCountChanged
 * @param text
 * 当页面的显示条数改变
 */
void Room::onComShowCountChanged(const QString text)
{
    if(text.isEmpty()||text.toInt() <= 0)
    {
        return;
    }
    m_model->setShowCount(text.toInt());
    m_model->setThisPage(1);
    m_model->select(m_whereValue,m_likeIndex);
    ui->tableView->setModel(nullptr);
    ui->tableView->setModel(m_model);
    //隐藏
    ui->tableView->hideColumn(0);
    //绑定委托
    boundDelegate();
    ui->tableView->update();
}

/**
 * @brief Room::onSelectBtnClicked
 * 查询按钮点击时,打开查询对话框
 */
void Room::onSelectBtnClicked()
{
    m_dialog->show();
}

/**
 * @brief Room::onDiaSelectBtnClicked
 * 当查询对话框中 “查询” 按钮被按下，接收他发出的信号
 */
void Room::onDiaSelectBtnClicked(QVariantMap maps)
{
    m_whereField.clear();
    m_whereValue.clear();
    m_likeIndex.clear();

    //遍历
    QMap<QString,QVariant>::const_iterator ite;
    for(ite = maps.begin(); ite != maps.end(); ite++)
    {
        m_whereField.append(ite.key());
        m_whereValue.append(ite.value());
    }

    QString sql = m_service->selectSql(m_whereField);
    m_model->setSql(sql);
    //查询
    select();
    //将当前页改为1
    ui->edit_thisPage->setText(QString::number(1));
}

/**
 * @brief Room::onJoinSqlInLike
 * @param index Like出现的index位置
 *在service中拼接sql语句中出现like，触发该槽
 */
void Room::onJoinSqlInLike(int index)
{
    m_likeIndex.append(index);
}

/**
 * @brief Room::onRefreshBtnClicked
 * 刷新按钮被按下，刷新数据
 */
void Room::onRefreshBtnClicked()
{
    select();
}

/**
 * @brief Room::sqlError
 * @param error
 * 新增/修改发生错误
 */
void Room::sqlError(QString error)
{
    QMessageBox::warning(this,"提示",error);
    //显示普通按钮/隐藏提交回滚按钮
    btnSubDispaly(false);
    btnComDisplay(true);
    //查询数据
    select();
}

/**
 * @brief Room::onTableViewDoubleClicked
 * @param index
 * 双击进行修改，同时进行事务处理
 */
void Room::onTableViewDoubleClicked(const QModelIndex &)
{
    bool is = m_model->openSqlTransaction();

    if(is && !ui->btn_commit->isVisible())
    {
        //隐藏普通按钮/显示回滚按钮
        btnComDisplay(false);
        btnSubDispaly(true);
    }

    if(!is)
    {
        QMessageBox::warning(this,"提示","事务无法开启，请关闭页面后重试");
    }
}


/**
 * @brief Room::onInsertBtnClicked
 * 在tableView中新增一行,新增按钮被按下
 */
void Room::onInsertBtnClicked()
{
    if(m_model->openSqlTransaction())
    {
        m_model->insertRow(1);
        btnComDisplay(false);
        btnSubDispaly(true);
    }
    else
    {
        QMessageBox::warning(this,"提示","事务无法开启，请关闭页面后重试");
    }
}

/**
 * @brief Room::onCommitBtnClicked
 * 提交
 */
void Room::onCommitBtnClicked()
{
    if(m_model->submitAll())
    {
        select();
        QMessageBox::information(this,"提交成功","提交成功了");
        btnSubDispaly(false);
        btnComDisplay(true);
    }
    else
    {
        QMessageBox::information(this,"提交失败","提交失败");
    }
}

/**
 * @brief Room::onrollBackBtnClicked
 * 回滚
 */
void Room::onrollBackBtnClicked()
{
    if(m_model->backAll())
    {
        select();
        QMessageBox::information(this,"回滚成功","回滚成功了");
        btnSubDispaly(false);
        btnComDisplay(true);
    }
    else
    {
        QMessageBox::information(this,"回滚失败","回滚失败");
    }
}

/**
 * @brief Room::resizeEvent
 * @param event
 * 窗体大小改变事件 用于初始化操作
 */
void Room::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);
    ui->retranslateUi(this);

    int width = ui->tableView->width();
    int height = ui->tableView->height()-40;

    //显示条数
    int row = height/40;
    //初始化显示条数下拉框
    initComboBox();
    insertToComboBox(row);

    //查询
    select();

    //每列宽度
    int col = m_model->columnCount()-1;
    if(col > 0)
    {
        ui->tableView->horizontalHeader()->setDefaultSectionSize(width/col-1);
    }
}
