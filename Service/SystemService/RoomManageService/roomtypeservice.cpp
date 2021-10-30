#include "roomtypeservice.h"

#include <QSqlRecord>

#include "Dao/speedydao.h"
#include "Entitys/SystemRoomManageEntitys.h"

RoomTypeService::RoomTypeService(QObject *parent) : QObject(parent)
{
    m_speedyDao = new SpeedyDao(this);
    m_tableName = "room_type";
}

/**
 * @brief RoomTypeService::selectRoomType
 * @return
 * 查询
 */
QList<RoomTypeInfo> RoomTypeService::selectRoomType()
{
    QString sql = m_speedyDao->joinSelectSQL(m_tableName);
    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(sql);

    QList<RoomTypeInfo> data;

    for(int i = 0; i < records.size(); i++)
    {
        QSqlRecord  record = records.at(i);

        RoomTypeInfo item;

        item.room_type_id = record.value("room_type_id").toInt();
        item.code = record.value("code").toString();
        item.m_describe = record.value("m_describe").toString();
        item.bed_number = record.value("bed_number").toInt();
        item.room_rate = record.value("room_rate").toDouble();

        data.append(item);
    }

    return data;
}

/**
 * @brief RoomTypeService::insertRoomType
 * @param code
 * @param m_describe
 * @param bed_number
 * @param room_rate
 * @param occeupacy_rate
 * @param is_double_room
 * @return
 * 新增
 */
bool RoomTypeService::insertRoomType(QString code, QString m_describe, int bed_number, double room_rate)
{
    QStringList insertKeys;
    insertKeys << "code" << "m_describe" << "bed_number" << "room_rate";
    QString sql = m_speedyDao->joinInsertSQL(m_tableName,insertKeys);

    QVariantList values;
    values << code << m_describe << bed_number << room_rate;

    return m_speedyDao->insertSql(sql,values);
}

/**
 * @brief RoomTypeService::updateRoomType
 * @param room_type_id
 * @param code
 * @param m_describe
 * @param bed_number
 * @param room_rate
 * @param occeupacy_rate
 * @param is_double_room
 * @return
 * 修改
 */
bool RoomTypeService::updateRoomType(int room_type_id, QString code, QString m_describe, int bed_number,double room_rate)
{
    QStringList insertKeys;
    insertKeys << "code" << "m_describe" << "bed_number" << "room_rate";
    QString sql = m_speedyDao->joinUpdateSQL(m_tableName,insertKeys);

    sql.append(m_speedyDao->joinWhereSQL(m_tableName,QStringList()<<"room_type_id"));

    QVariantList values;
    values << code << m_describe << bed_number << room_rate << room_type_id;

    return m_speedyDao->updateSql(sql,values);
}

/**
 * @brief RoomTypeService::deleteRoomType
 * @param room_type_id
 * @return
 * 删除
 */
bool RoomTypeService::deleteRoomType(int room_type_id)
{
    QString sql = m_speedyDao->joinDeleteSQL(m_tableName);
    sql.append(m_speedyDao->joinWhereSQL(m_tableName,QStringList()<<"room_type_id"));
    return m_speedyDao->deleteSql(sql,QVariantList()<<room_type_id);
}

/**
 * @brief RoomTypeService::getLastError
 * @return
 * 获取最后一个错误
 */
QString RoomTypeService::getLastError()
{
    return m_speedyDao->getLastError();
}
























