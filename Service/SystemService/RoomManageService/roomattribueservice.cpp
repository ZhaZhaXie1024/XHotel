#include "roomattribueservice.h"

#include <QSqlRecord>

#include "Dao/speedydao.h"
#include "Entitys/SystemRoomManageEntitys.h"

RoomAttribueService::RoomAttribueService(QObject *parent) : QObject(parent)
{
    m_speedyDao = new SpeedyDao(this);
    m_tableName = "room_attribute";
}

/**
 * @brief RoomAttribueService::selectRoomAttribue
 * @return
 * 查询
 */
QList<RoomAttribueInfo> RoomAttribueService::selectRoomAttribue()
{
    QString sql = m_speedyDao->joinSelectSQL(m_tableName);
    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(sql);

    QList<RoomAttribueInfo> data;

    for(int i = 0; i < records.size(); i++)
    {
        QSqlRecord record = records.at(i);

        RoomAttribueInfo item;
        item.room_attribute_id = record.value("room_attribute_id").toInt();
        item.code = record.value("code").toString();
        item.m_describe = record.value("m_describe").toString();

        data.append(item);
    }

    return data;
}

/**
 * @brief RoomAttribueService::insertRoomAttribue
 * @param code
 * @param m_describe
 * @return
 * 新增
 */
bool RoomAttribueService::insertRoomAttribue(QString code, QString m_describe)
{
    QString sql = m_speedyDao->joinInsertSQL(m_tableName,QStringList() << "code" << "m_describe");

    QVariantList values;
    values << code << m_describe;

    return m_speedyDao->insertSql(sql,values);
}

/**
 * @brief RoomAttribueService::updateRoomAttribue
 * @param room_attribue_id
 * @param code
 * @param m_describe
 * @return
 * 修改
 */
bool RoomAttribueService::updateRoomAttribue(int room_attribue_id, QString code, QString m_describe)
{
    QString sql = m_speedyDao->joinUpdateSQL(m_tableName,QStringList() << "code" << "m_describe");
    sql.append(m_speedyDao->joinWhereSQL(m_tableName,QStringList() << "room_attribute_id"));

    QVariantList values;
    values << code << m_describe << room_attribue_id;

    return m_speedyDao->updateSql(sql,values);
}

/**
 * @brief RoomAttribueService::deleteRoomAttribue
 * @param room_attribue_id
 * @return
 * 删除
 */
bool RoomAttribueService::deleteRoomAttribue(int room_attribue_id)
{
    QString sql = m_speedyDao->joinDeleteSQL(m_tableName);
    sql.append(m_speedyDao->joinWhereSQL(m_tableName,QStringList()<<"room_attribute_id"));

    QVariantList values;
    values << room_attribue_id;

    return m_speedyDao->deleteSql(sql,values);
}

/**
 * @brief RoomAttribueService::getLastError
 * @return
 * 获取最后一个错误
 */
QString RoomAttribueService::getLastError()
{
    return m_speedyDao->getLastError();
}



