#include "chargeitemservice.h"

#include <Dao/speedydao.h>
#include <Entitys/SysOtherEntitys.h>

#include <QSqlRecord>

ChargeItemService::ChargeItemService(QObject *parent) : QObject(parent)
{
    m_speedyDao = new SpeedyDao(this);
    m_tableName = "charge_item";
}

/**
 * @brief ChargeItemService::select
 * @return
 * 查询
 */
QList<ChargeItemInfo> ChargeItemService::select()
{
    QString sql = m_speedyDao->joinSelectSQL(m_tableName);

    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(sql);

    QList<ChargeItemInfo> data;

    for(int i = 0; i < records.size(); i++)
    {
        QSqlRecord record = records.at(i);

        ChargeItemInfo item;
        item.charge_item_id = record.value("charge_item_id").toInt();
        item.money = record.value("money").toDouble();
        item.m_describe = record.value("m_describe").toString();

        data.append(item);
    }

    return data;
}

/**
 * @brief ChargeItemService::insertChargeItem
 * @param values
 * @return
 * 新增
 */
bool ChargeItemService::insertChargeItem(QVariantList values)
{
    QString sql = m_speedyDao->joinInsertSQL(m_tableName,QStringList()<<"money"<<"m_describe");
    return m_speedyDao->insertSql(sql,values);
}

/**
 * @brief ChargeItemService::updateChargeItem
 * @param charge_item_id
 * @param money
 * @param m_describe
 * @return
 * 修改
 */
bool ChargeItemService::updateChargeItem(int charge_item_id, double money, QString m_describe)
{
    QVariantList values;
    values << money << m_describe << charge_item_id;
    QString sql = m_speedyDao->joinUpdateSQL(m_tableName,QStringList()<<"money"<<"m_describe");
    sql.append(m_speedyDao->joinWhereSQL(m_tableName,QStringList()<<"charge_item_id"));
    return m_speedyDao->insertSql(sql,values);
}

/**
 * @brief ChargeItemService::deleteChargeItem
 * @param charge_item_id
 * @return
 * 删除
 */
bool ChargeItemService::deleteChargeItem(int charge_item_id)
{
    QString sql = m_speedyDao->joinDeleteSQL(m_tableName);
    sql.append(m_speedyDao->joinWhereSQL(m_tableName,QStringList()<<"charge_item_id"));
    return m_speedyDao->deleteSql(sql,QVariantList()<<charge_item_id);
}

/**
 * @brief ChargeItemService::getLastError
 * @return 最后的错误
 *
 */
QString ChargeItemService::getLastError()
{
    return m_speedyDao->getLastError();
}

