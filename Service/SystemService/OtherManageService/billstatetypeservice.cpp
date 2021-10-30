#include "billstatetypeservice.h"

#include "Dao/speedydao.h"
#include "Entitys/SysOtherEntitys.h"

#include <QSqlRecord>

BillStateTypeService::BillStateTypeService(QObject *parent) : QObject(parent)
{
    m_speedyDao = new SpeedyDao(this);
    m_tableName = "bill_state_type";
}

/**
 * @brief BillStateTypeService::select
 * @return
 * 查询
 */
QList<BillStateTypeInfo> BillStateTypeService::select()
{
    QString sql = m_speedyDao->joinSelectSQL(m_tableName);

    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(sql);

    QList<BillStateTypeInfo> data;

    for(int i = 0; i < records.size(); i++)
    {
        QSqlRecord record = records.at(i);

        BillStateTypeInfo item;
        item.bill_state_type_id = record.value("bill_state_type_id").toInt();
        item.code = record.value("code").toString();
        item.bill_state = record.value("bill_state").toString();

        data.append(item);
    }

    return data;
}

/**
 * @brief BillStateTypeService::insertBillStateType
 * @param bill_state
 * @return
 * 新增
 */
bool BillStateTypeService::insertBillStateType(QString code,QString bill_state)
{
    QVariantList values;
    values << code << bill_state;

    QString sql = m_speedyDao->joinInsertSQL(m_tableName,QStringList()<< "code" <<"bill_state");
    return m_speedyDao->insertSql(sql,values);
}

/**
 * @brief BillStateTypeService::updateBillStateType
 * @param bill_state_type_id
 * @param bill_state
 * @return
 * 修改
 */
bool BillStateTypeService::updateBillStateType(int bill_state_type_id,QString code, QString bill_state)
{
    QVariantList values;
    values << code << bill_state << bill_state_type_id;
    QString sql = m_speedyDao->joinUpdateSQL(m_tableName,QStringList()<< "code" <<"bill_state");
    sql.append(m_speedyDao->joinWhereSQL(m_tableName,QStringList()<<"bill_state_type_id"));
    return m_speedyDao->insertSql(sql,values);
}

/**
 * @brief BillStateTypeService::deleteBillStateType
 * @param bill_state_type_id
 * @return
 * 删除
 */
bool BillStateTypeService::deleteBillStateType(int bill_state_type_id)
{
    QString sql = m_speedyDao->joinDeleteSQL(m_tableName);
    sql.append(m_speedyDao->joinWhereSQL(m_tableName,QStringList()<<"bill_state_type_id"));
    return m_speedyDao->deleteSql(sql,QVariantList()<<bill_state_type_id);
}

/**
 * @brief BillStateTypeService::getLastError
 * @return
 * 获取最后的错误
 */
QString BillStateTypeService::getLastError()
{
    return m_speedyDao->getLastError();
}
