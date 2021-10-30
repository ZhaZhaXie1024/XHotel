#include "roomservice.h"

#include <QDebug>

#include "Dao/speedydao.h"
#include "Service/comboboxitemservice.h"
#include "Entitys/comboboxentity.h"

RoomService::RoomService(QObject *parent) : QObject(parent)
{
    m_speedyDao = new SpeedyDao(this);
    m_comboBoxService = new ComboboxItemService(this);

    m_tableName = "room";
}

/**
 * @brief RoomService::selectSql
 * @param keys      where key
 * @return          sql
 * 拼接查询语句
 */
QString RoomService::selectSql(QStringList wherekeys)
{
    QStringList fieldKeys;
    fieldKeys << "room.room_id AS room_id" << "room.room_num AS room_num"
              << "tower_range.m_describe AS tower_range_id" << "tower_seat.m_describe AS tower_seat_id"
              << "room.tower_tier AS tower_tier" << "room_type.m_describe AS room_type_id"
              << "room_state.m_describe AS room_state_id";

    QString sql = m_speedyDao->joinSelectSQL(m_tableName,fieldKeys);

    //拼接leftjoin
    QString tsLeft = m_speedyDao->joinLeftJoinSQL(m_tableName,"tower_range",QStringList()<<"tower_range_id");
    QString trLeft = m_speedyDao->joinLeftJoinSQL(m_tableName,"tower_seat",QStringList()<<"tower_seat_id");    
    QString rtleft = m_speedyDao->joinLeftJoinSQL(m_tableName,"room_type",QStringList()<<"room_type_id");
    QString rsleft = m_speedyDao->joinLeftJoinSQL(m_tableName,"room_state",QStringList()<<"room_state_id");

    sql.append(trLeft);
    sql.append(tsLeft);
    sql.append(rtleft);
    sql.append(rsleft);

    //拼接where
    bool isEmpty = true;

    for(int i = 0; i < wherekeys.size(); i++)
    {
        QString key = wherekeys.at(i);

        QString temp;

        if(isEmpty)
        {
            temp = " WHERE ";
        }
        else
        {
            temp = " AND ";
        }

        if(key.compare("room_num") != 0)
        {
            temp.append(m_tableName + "." + key + " = ? ");
            isEmpty = false;
        }
        else
        {
            temp.append(m_tableName + "." + key + " LIKE ? ");
            emit onlikeIndex(i);
            isEmpty = false;
        }

        sql.append(temp);
    }

    sql.append(" ORDER BY room.room_id DESC ");

//    qDebug() << "RoomService::selectSql:" << sql << endl;

    return sql;
}

/**
 * @brief RoomService::insertSql
 * @param keys
 * @return
 * 新增语句
 */
QString RoomService::insertSql()
{
    QStringList keys;
    keys << "room_num" << "tower_range_id" << "tower_seat_id"
         << "tower_tier" << "room_type_id" << "room_state_id";
    return m_speedyDao->joinInsertSQL(m_tableName,keys);
}

/**
 * @brief RoomService::comboBoxData
 * @param tableName
 * @param field
 * @return
 * 返回下拉框数据
 */
QList<ComboBoxSeleteItem> RoomService::comboBoxData(QString tableName, QStringList field,QStringList whereKey,QVariantList values)
{
    if(whereKey.size() > 0 && (whereKey.size() == values.size()))
    {
        m_comboBoxService->init(tableName,field,whereKey);
        return m_comboBoxService->select(values);
    }
    else
    {
        m_comboBoxService->init(tableName,field);
        return m_comboBoxService->select();
    }
}
