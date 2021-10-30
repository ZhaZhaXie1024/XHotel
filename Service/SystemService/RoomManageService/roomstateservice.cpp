#include "roomstateservice.h"

#include <QSqlRecord>

#include "Dao/speedydao.h"
#include "Entitys/SystemRoomManageEntitys.h"

RoomStateService::RoomStateService(QObject *parent) : QObject(parent)
{
    m_speedyDao = new SpeedyDao(this);
    m_tableName = "room_state";
}

/**
 * @brief RoomStateService::selectState
 * @return
 * 查询
 */
QList<RoomStateInfo> RoomStateService::selectRoomState()
{
    QString sql = m_speedyDao->joinSelectSQL(m_tableName);
    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(sql);

    QList<RoomStateInfo> data;

    for(int i = 0; i < records.size(); i++)
    {
        QSqlRecord record = records.at(i);

        RoomStateInfo item;
        item.room_state_id = record.value("room_state_id").toInt();
        item.code = record.value("code").toString();
        item.m_describe = record.value("m_describe").toString();

        data.append(item);
    }

    return data;
}

/**
 * @brief RoomStateService::insertRoomState
 * @param code
 * @param m_describe
 * @return
 * 新增
 */
bool RoomStateService::insertRoomState(QString code, QString m_describe)
{
    QString sql = m_speedyDao->joinInsertSQL(m_tableName,QStringList() << "code" << "m_describe");

    QVariantList values;
    values << code << m_describe;

    return m_speedyDao->insertSql(sql,values);
}

/**
 * @brief RoomStateService::updateRoomState
 * @param room_state_id
 * @param code
 * @param m_describe
 * @return
 * 修改
 */
bool RoomStateService::updateRoomState(int room_state_id, QString code, QString m_describe)
{
    QString sql = m_speedyDao->joinUpdateSQL(m_tableName,QStringList() << "code" << "m_describe");
    sql.append(m_speedyDao->joinWhereSQL(m_tableName,QStringList() << "room_state_id"));

    QVariantList values;
    values << code << m_describe << room_state_id;

    return m_speedyDao->updateSql(sql,values);
}

/**
 * @brief RoomStateService::deleteRoomState
 * @param room_state_id
 * @return
 * 删除
 */
bool RoomStateService::deleteRoomState(int room_state_id)
{
    QString sql = m_speedyDao->joinDeleteSQL(m_tableName);
    sql.append(m_speedyDao->joinWhereSQL(m_tableName,QStringList()<<"room_state_id"));

    QVariantList values;
    values << room_state_id;

    return m_speedyDao->deleteSql(sql,values);
}

/**
 * @brief RoomStateService::getLastError
 * @return
 * 获取最后一个错误
 */
QString RoomStateService::getLastError()
{
    return m_speedyDao->getLastError();
}
