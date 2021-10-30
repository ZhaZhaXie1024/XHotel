#include "roomownattributeservice.h"

#include <QSqlRecord>

#include "Dao/speedydao.h"
#include "Service/SystemService/RoomManageService/roomservice.h"
#include "Service/SystemService/RoomManageService/roomattribueservice.h"
#include "Entitys/SystemRoomManageEntitys.h"

RoomOwnAttributeService::RoomOwnAttributeService(QObject *parent) : QObject(parent),m_tableName("room_own_attribute")
{
    m_speedyDao = new SpeedyDao(this);
    m_roomService = new RoomService(this);
    m_roomAttrService = new RoomAttribueService(this);
}

/**
 * @brief RoomOwnAttributeService::selectRoomSql
 * @param whereKeys
 * @return SQL
 * 拼接 查询房间 SQL
 */
QString RoomOwnAttributeService::selectRoomSql(QStringList whereKeys)
{
    return m_roomService->selectSql(whereKeys);
}

/**
 * @brief RoomOwnAttributeService::selectRoomAttribue
 * @return  房间属性实体列表
 * 查询房间属性
 */
QList<RoomAttribueInfo> RoomOwnAttributeService::selectRoomAttribue()
{
    return m_roomAttrService->selectRoomAttribue();
}

/**
 * @brief RoomOwnAttributeService::selectRoomOwnAttribue
 * @param roomID 房间ID
 * @return
 * 查询房间拥有的属性
 */
QList<RoomOwnAttribueInfo> RoomOwnAttributeService::selectRoomOwnAttribue(int roomID)
{
    QStringList keys;
    keys << "room_own_attribute_id" << "room_attribute.code" << "room_attribute.m_describe";
    QString sql = m_speedyDao->joinSelectSQL(m_tableName,keys);
    sql.append(m_speedyDao->joinLeftJoinSQL(m_tableName,"room",QStringList() << "room_id"));
    sql.append(m_speedyDao->joinLeftJoinSQL(m_tableName,"room_attribute",QStringList() << "room_attribute_id"));
    sql.append(m_speedyDao->joinWhereSQL(m_tableName,QStringList()<<"room_id"));
    QVariantList values;
    values << roomID;

    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(sql,values);

    QList<RoomOwnAttribueInfo> data;

    for(int i = 0; i < records.size(); i++)
    {
        QSqlRecord record = records.at(i);

        RoomOwnAttribueInfo item;
        item.room_own_attribute_id = record.value("room_own_attribute_id").toInt();
        item.code = record.value("code").toString();
        item.attribute = record.value("m_describe").toString();

        data.append(item);
    }

    return data;
}

/**
 * @brief RoomOwnAttributeService::insert
 * @param roomID        房间ID
 * @param attribueID    属性ID
 * @return true成功
 * 新增一条房间拥有的属性
 */
bool RoomOwnAttributeService::insert(int roomID, int attribueID)
{
    QString sql = m_speedyDao->joinInsertSQL(m_tableName,QStringList()<<"room_id"<<"room_attribute_id");
    QVariantList values;
    values << roomID << attribueID;

    return m_speedyDao->insertSql(sql,values);
}

/**
 * @brief RoomOwnAttributeService::insertCheck
 * @param roomID
 * @param attribueID
 * @return
 * 检查是否重复新增
 */
bool RoomOwnAttributeService::insertCheck(int roomID, int attribueID)
{
    QString sql = m_speedyDao->joinSelectSQL(m_tableName,QStringList()<<"count(*)");
    sql.append(m_speedyDao->joinWhereSQL(m_tableName,QStringList()<<"room_id"<<"room_attribute_id"));

    return m_speedyDao->selectTableCount(sql,QVariantList()<<roomID<<attribueID);
}

/**
 * @brief RoomOwnAttributeService::deleteOwnAttribue
 * @param room_own_attribute_id
 * @return
 * 删除一条房间拥有的属性
 */
bool RoomOwnAttributeService::deleteOwnAttribue(int room_own_attribute_id)
{
    QString sql = m_speedyDao->joinDeleteSQL(m_tableName);
    sql.append(m_speedyDao->joinWhereSQL(m_tableName,QStringList()<<"room_own_attribute_id"));

    return m_speedyDao->deleteSql(sql,QVariantList()<<room_own_attribute_id);
}

/**
 * @brief RoomOwnAttributeService::sendLikeIndex
 * @param index
 * roomService中发送onlikeIndex信号触发
 */
void RoomOwnAttributeService::sendLikeIndex(int index)
{
    emit sendIndex(index);
}

