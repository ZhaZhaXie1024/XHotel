#include "comboboxitemservice.h"

#include <QSqlRecord>
#include <QComboBox>
#include <QtDebug>

#include "Dao/speedydao.h"


ComboboxItemService::ComboboxItemService(QObject *parent) : QObject(parent)
{
    m_dao = new SpeedyDao(this);
    m_sql = "";
}

ComboboxItemService::ComboboxItemService(QString tableName, QStringList field, QObject *parent)
    : QObject(parent)
{
    m_dao = new SpeedyDao(this);
    m_sql = "";
    init(tableName,field);
}

/**
 * @brief ComboboxItemService::init
 * 初始化
 */
void ComboboxItemService::init(QString tableName,QStringList field)
{
    QString select = m_dao->joinSelectSQL(tableName,field);

    m_sql = select;
}

/**
 * @brief ComboboxItemService::init
 * @param tableName
 * @param field
 * @param where
 * 带where语句,并且只有and 和 =
 */
void ComboboxItemService::init(QString tableName, QStringList field, QStringList where)
{
    QString select = m_dao->joinSelectSQL(tableName,field);

    select.append(m_dao->joinWhereSQL(tableName,where));

//    qDebug() << select;

    m_sql = select;
}

/**
 * @brief ComboboxItemService::init
 * @param tableName
 * @param field
 * @param where
 * @param sqlLinkSymbol
 * 带where可以多个组合
 */
void ComboboxItemService::init(QString tableName, QStringList field, QStringList where, QList<int> sqlLinkSymbol)
{
    QString select = m_dao->joinSelectSQL(tableName,field);

    QList<SQLLinkSymbol> sqlLines;

    for(int i = 0; i < sqlLinkSymbol.size(); i++)
    {
        sqlLines << SQLLinkSymbol(sqlLinkSymbol.at(i));
    }

    select.append(m_dao->joinWhereSQL(QStringList()<<tableName,where,sqlLines));

    m_sql = select;
}

/**
 * @brief ComboboxItemService::select
 * @return
 * 查询数据
 */
QList<ComboBoxSeleteItem> ComboboxItemService::select(QVariantList values)
{
    if(m_sql.trimmed().isEmpty())
    {
        return QList<ComboBoxSeleteItem>();
    }

    QList<ComboBoxSeleteItem> itemList;

    QList<QSqlRecord> recordList = m_dao->selectSqlRecord(m_sql,values);

    for(int i = 0; i < recordList.size(); i++)
    {
        QSqlRecord record = recordList.at(i);

        int id = record.value(0).toInt();
        QString name = record.value(1).toString();

        ComboBoxSeleteItem item;
        item.itemID = id;
        item.itemName = name;

        itemList.append(item);

    }

    return itemList;
}

/**
 * @brief ComboboxItemService::select
 * @param sql
 * @param values
 * @return
 * 查询数据,直接通过sql和value查询
 */
QList<ComboBoxSeleteItem> ComboboxItemService::select(QString sql, QVariantList values)
{
    if(sql.trimmed().isEmpty())
    {
        return QList<ComboBoxSeleteItem>();
    }

    QList<ComboBoxSeleteItem> itemList;

    QList<QSqlRecord> recordList = m_dao->selectSqlRecord(sql,values);

    for(int i = 0; i < recordList.size(); i++)
    {
        QSqlRecord record = recordList.at(i);

        int id = record.value(0).toInt();
        QString name = record.value(1).toString();

        ComboBoxSeleteItem item;
        item.itemID = id;
        item.itemName = name;

        itemList.append(item);

    }

    return itemList;
}


/**
 * @brief ComboboxItemService::boundData
 * @param box
 * @param data
 * 绑定数据
 */
void ComboboxItemService::boundData(QComboBox *box, QList<ComboBoxSeleteItem> data)
{
    QPixmap pixmap(1, 30);
    pixmap.fill(Qt::transparent);//先弄一个透明的图片
    QIcon icon(pixmap);
    box->setIconSize(QSize(1,30));

    //添加一个 --请选择--
    box->addItem(icon,"-- 请选择 --",0);

    for(int i = 0; i < data.size(); i++)
    {
        ComboBoxSeleteItem item = data.at(i);
        box->addItem(icon,item.itemName,item.itemID);
    }
}

/**
 * @brief ComboboxItemService::boundData
 * @param box
 * @param data
 * 绑定数据
 */
void ComboboxItemService::boundData(QComboBox *box, QList<QVariantMap> data)
{
    QPixmap pixmap(1, 30);
    pixmap.fill(Qt::transparent);//先弄一个透明的图片
    QIcon icon(pixmap);
    box->setIconSize(QSize(1,30));

    //添加一个 --请选择--
    box->addItem(icon,"-- 请选择 --",0);

    for(int i = 0; i < data.size(); i++)
    {
        QVariantMap map = data.at(i);
        QMap<QString,QVariant>::const_iterator ite = map.begin();
        for(;ite != map.end(); ite++)
        {
            box->addItem(icon,ite.key(),ite.value());
        }
    }
}
