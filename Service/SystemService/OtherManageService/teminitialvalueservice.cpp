#include "teminitialvalueservice.h"

#include <QSqlRecord>
#include <QDebug>

#include "Dao/speedydao.h"
#include "Entitys/SysOtherEntitys.h"

TeminitialValueService::TeminitialValueService(QObject *parent) : QObject(parent),m_tableName("systeminitial_value")
{
    m_speedyDao = new SpeedyDao(this);

}

/**
 * @brief TeminitialValueService::select
 * @return
 * 查询
 */
QList<TeminitialValueInfo> TeminitialValueService::select()
{
    QStringList keys;
    keys << "systemInitial_value_id" << "state" << "m_describe" << "prefix" << "init_value" << "this_value";
    QString sql = m_speedyDao->joinSelectSQL(m_tableName);
    sql.append(m_speedyDao->joinLeftJoinSQL(m_tableName,"init_value_type",QStringList()<<"init_value_type_id"));

    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(sql);

    QList<TeminitialValueInfo> data;

    for(int i = 0; i < records.size(); i++)
    {
        QSqlRecord record = records.at(i);

        TeminitialValueInfo item;
        item.systemInitial_value_id = record.value("systemInitial_value_id").toInt();
        item.state = record.value("state").toString();
        item.m_describe = record.value("m_describe").toString();
        item.prefix = record.value("prefix").toString();
        item.init_value = record.value("init_value").toInt();
        item.this_value = record.value("this_value").toInt();

        data.append(item);
    }

    return data;
}

/**
 * @brief TeminitialValueService::insertTeminital
 * @param values
 * @return
 * 新增
 */
bool TeminitialValueService::insertTeminital(QVariantList values)
{
    QString sql = m_speedyDao->joinInsertSQL(m_tableName,QStringList()<<"init_value_type_id"<<"m_describe"<<"prefix"<<"init_value"<<"this_value");
    return m_speedyDao->insertSql(sql,values);
}

/**
 * @brief TeminitialValueService::deleteTeminital
 * @param systemInitial_value_id
 * @return
 * 删除
 */
bool TeminitialValueService::deleteTeminital(int systemInitial_value_id)
{
    QString sql = m_speedyDao->joinDeleteSQL(m_tableName);
    sql.append(m_speedyDao->joinWhereSQL(m_tableName,QStringList()<<"systemInitial_value_id"));
    return m_speedyDao->deleteSql(sql,QVariantList()<<systemInitial_value_id);
}
