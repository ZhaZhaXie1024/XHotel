#include "modeltreewidget.h"
#include "ui_modeltreewidget.h"

#include "Dao/basedao.h"

#include <QTreeWidgetItem>
#include <QDebug>

ModelTreeWidget::ModelTreeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModelTreeWidget)
{
    init();

}

ModelTreeWidget::ModelTreeWidget(int operator_id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModelTreeWidget)
{
    init();
    m_operator_id = operator_id;
}

ModelTreeWidget::~ModelTreeWidget()
{
    delete ui;
}

/**
 * @brief ModelTreeWidget::init
 * 功能：初始化
 */
void ModelTreeWidget::init()
{
    ui->setupUi(this);

    //修改背景颜色
    setAutoFillBackground(true);                //允许填充背景
    setPalette(QPalette(QColor(255,255,255)));  //填充为白色

    //为treewidget添加动画，默认的
    ui->treeWidget->setAnimated(true);

    m_baseDao = new BaseDao(this);

    //查询模块
    selectModel();

    //连接槽
    connect(ui->treeWidget,&QTreeWidget::itemClicked,this,&ModelTreeWidget::onTreeWidgetItemClicked);
}

/**
 * @brief ModelTreeWidget::selectModel
 * 功能：查询管理员能够操作的模块
 */
void ModelTreeWidget::selectModel()
{
    QString selectSql = "SELECT "
                        "sys_module.* "
                        "FROM "
                        "sys_module ";

    QList<QVariantMap> model = m_baseDao->selectBySqlStr(selectSql);

    for(int i = 0; i < model.size(); i++)
    {
        QVariantMap map = model.at(i);

        //获取数据
        int moduleID = map.value("sys_module_id").toInt();
        QString moduleName = map.value("sys_module_name").toString();
        int submodelID = map.value("sys_sub_module_id").isNull() ? 0 : map.value("sys_sub_module_id").toInt();

        QTreeWidgetItem *item = new QTreeWidgetItem;
        item->setText(0,moduleName);
        item->setData(0,Qt::UserRole,moduleID);

        if(submodelID > 0)
        {
            QTreeWidgetItemIterator it(ui->treeWidget);
            while (*it) {

                if((*it)->data(0,Qt::UserRole).toInt() == submodelID)
                {
                    (*it)->addChild(item);
                    break;
                }
                ++it;
            }
        }
        else
        {
            ui->treeWidget->addTopLevelItem(item);
        }
    }
}

/**
 * @brief ModelTreeWidget::setCurrentItem
 * @param itemStr
 * 设置选中项
 */
void ModelTreeWidget::setCurrentItem(QString itemStr)
{
    //MatchFixedString基于字符串匹配，MatchCaseSensitive区分大小写，MatchRecursive遍历整个节点
    QList<QTreeWidgetItem *> items = ui->treeWidget->findItems(itemStr.trimmed(),Qt::MatchFixedString|Qt::MatchCaseSensitive|Qt::MatchRecursive);;

    if(items.size() > 0)
    {
        ui->treeWidget->setCurrentItem(items.at(0));
    }
}

/**
 * @brief ModelTreeWidget::onTreeWidgetItemClicked
 * @param item      item
 * @param column    item所在的列
 * 功能：点击item后获取item的text，然后发送到centrewidget中的槽
 */
void ModelTreeWidget::onTreeWidgetItemClicked(QTreeWidgetItem *item, int column)
{
    if(item->childCount() == 0)
    {
        emit treeWidgetItemClicked(item->text(column));
    }

}

/**
 * @brief ModelTreeWidget::cancelSelectedItem
 * 通过widget.cpp中转，在centrewidget中发出;取消选中项
 */
void ModelTreeWidget::cancelSelectedItem()
{
    ui->treeWidget->clearSelection();
}

