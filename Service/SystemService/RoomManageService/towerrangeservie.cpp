#include "towerrangeservie.h"

#include <QSqlRecord>

#include "Dao/speedydao.h"
#include "Entitys/SystemRoomManageEntitys.h"

TowerRangeServie::TowerRangeServie(QObject *parent)
    : QObject(parent),
      m_tableName("tower_range")
{
    m_speedyDao = new SpeedyDao(this);
}

/**
 * @brief TowerRangeServie::selectTowerRang
 * @return
 * 查询
 */
QList<TowerRangInfo> TowerRangeServie::selectTowerRang()
{
    QString sql = m_speedyDao->joinSelectSQL(m_tableName);
    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(sql);

    QList<TowerRangInfo> data;

    for(int i =  0; i < records.size(); i++)
    {
        TowerRangInfo info;

        QSqlRecord record = records.at(i);

        info.tower_range_id = record.value("tower_range_id").toInt();
        info.code = record.value("code").toString();
        info.m_describe = record.value("m_describe").toString();
        info.attribute = record.value("attribute").toString();

        data.append(info);
    }

    return data;
}

/**
 * @brief TowerRangeServie::insertTowerRang
 * @param code
 * @param describe
 * @param attribue
 * @return
 * 新增
 */
bool TowerRangeServie::insertTowerRang(QString code, QString describe, QString attribue)
{
    QString sql = m_speedyDao->joinInsertSQL(m_tableName,QStringList()<<"code"<<"m_describe"<<"attribute");

    QVariantList values;
    values << code << describe << attribue;

    return m_speedyDao->insertSql(sql,values);
}

/**
 * @brief TowerRangeServie::updateTowerRang
 * @param towerRangID
 * @param code
 * @param describe
 * @param attribue
 * @return
 * 修改
 */
bool TowerRangeServie::updateTowerRang(int towerRangID, QString code, QString describe, QString attribue)
{
    QString sql = m_speedyDao->joinUpdateSQL(m_tableName,QStringList()<<"code"<<"m_describe"<<"attribute");
    sql.append(m_speedyDao->joinWhereSQL(QStringList() << m_tableName,QStringList()<<"tower_range_id",QList<SQLLinkSymbol>()<<LinkEqual));
    QVariantList values;
    values << code << describe << attribue << towerRangID;
    return m_speedyDao->updateSql(sql,values);
}

/**
 * @brief TowerRangeServie::deleteTowerRang
 * @param towerRangID
 * @return
 * 删除
 */
bool TowerRangeServie::deleteTowerRang(int towerRangID)
{
    QString sql = m_speedyDao->joinDeleteSQL(m_tableName);
    sql.append(m_speedyDao->joinWhereSQL(QStringList()<<m_tableName,QStringList()<<"tower_range_id",QList<SQLLinkSymbol>()<<LinkEqual));
    return m_speedyDao->deleteSql(sql,QVariantList()<<towerRangID);
}

/**
 * @brief TowerRangeServie::getLastError
 * @return
 * 获取最后发生的错误
 */
QString TowerRangeServie::getLastError()
{
    return m_speedyDao->getLastError();
}


