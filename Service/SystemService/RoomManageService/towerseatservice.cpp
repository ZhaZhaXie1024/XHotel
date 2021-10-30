#include "towerseatservice.h"

#include <QSqlRecord>

#include "Dao/speedydao.h"
#include "Entitys/SystemRoomManageEntitys.h"

TowerSeatService::TowerSeatService(QObject *parent) : QObject(parent)
{
    m_speedyDao = new SpeedyDao(this);
    m_tableName = "tower_seat";
}

/**
 * @brief TowerSeatService::selectTowerSeat
 * @return
 * 查询
 */
QList<TowerSeatInfo> TowerSeatService::selectTowerSeat()
{
    QStringList selectKey;
    selectKey << m_tableName + ".tower_seat_id AS id" << m_tableName + ".code" << m_tableName + ".m_describe sm";
    QString sql = m_speedyDao->joinSelectSQL(m_tableName,selectKey);

    QList<TowerSeatInfo> data;

    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(sql);

    for(int i = 0; i < records.size(); i++)
    {
        QSqlRecord record = records.at(i);

        TowerSeatInfo item;
        item.tower_seat_id = record.value("id").toInt();
        item.code = record.value("code").toString();
        item.m_describe = record.value("sm").toString();

        data.append(item);
    }

    return data;
}

/**
 * @brief TowerSeatService::insertTowerSeat
 * @param towerRangeID
 * @param code
 * @param m_describe
 * @return
 * 新增
 */
bool TowerSeatService::insertTowerSeat(QString code, QString m_describe)
{
    QString sql = m_speedyDao->joinInsertSQL(m_tableName,QStringList() << "code" << "m_describe");

    QVariantList values;
    values << code << m_describe;

    return m_speedyDao->insertSql(sql,values);
}

/**
 * @brief TowerSeatService::updateTowerSeat
 * @param towerSeatID   楼座ID
 * @param towerRangeID  楼区ID
 * @param code          代码
 * @param m_describe    描述
 * @return
 * 修改
 */
bool TowerSeatService::updateTowerSeat(int towerSeatID, QString code, QString m_describe)
{
    QString sql = m_speedyDao->joinUpdateSQL(m_tableName,QStringList() << "code" << "m_describe");
    sql.append(m_speedyDao->joinWhereSQL(QStringList() << m_tableName,QStringList() << "tower_seat_id",QList<SQLLinkSymbol>()<<LinkEqual));

    QVariantList values;
    values << code << m_describe << towerSeatID;

    return m_speedyDao->updateSql(sql,values);
}

/**
 * @brief TowerSeatService::deleteTowerSeat
 * @param towerSeatID
 * @return
 * 删除
 */
bool TowerSeatService::deleteTowerSeat(int towerSeatID)
{
    QString sql = m_speedyDao->joinDeleteSQL(m_tableName);
    sql.append(m_speedyDao->joinWhereSQL(m_tableName,QStringList()<<"tower_seat_id"));

    QVariantList values;
    values << towerSeatID;

    return m_speedyDao->deleteSql(sql,values);
}

/**
 * @brief TowerSeatService::getLastError
 * @return
 * 获取最后的错误
 */
QString TowerSeatService::getLastError()
{
    return m_speedyDao->getLastError();
}

