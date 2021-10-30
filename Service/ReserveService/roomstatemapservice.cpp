#include "roomstatemapservice.h"

#include "Dao/speedydao.h"
#include "Entitys/room.h"
#include <QSqlRecord>
#include <QDebug>

RoomStateMapService::RoomStateMapService(QObject *parent) : QObject(parent),m_tableName("room")
{
    m_speedyDao = new SpeedyDao(this);
}

QList<RoomData> RoomStateMapService::selectRoom(int index,int showNum,QStringList whereKey,QVariantList values)
{
   QList<RoomData> m_data;

    QString sql = "SELECT "
            "room.room_id AS room_id,"
            "room.tower_range_id AS tr_id, "
            "room.tower_seat_id AS ts_id, "
            "room.tower_tier AS tier, "
            "room.room_num AS room_num, "
            "room.room_state_id AS state_id, "
            "room.room_type_id AS type_id, "
            "tower_seat.m_describe AS ts_describe, "
            "tower_range.m_describe AS tr_describe, "
            "room_type.m_describe AS type, "
            "room_type.bed_number AS bed_num, "
            "room_type.room_rate AS rate, "
            "room_state.m_describe AS state"
       " FROM "
           " room "
            "LEFT JOIN "
            "tower_range "
            "ON "
                "room.tower_range_id = tower_range.tower_range_id "
            "LEFT JOIN "
            "tower_seat "
            "ON "
                "room.tower_seat_id = tower_seat.tower_seat_id "
            "LEFT JOIN "
            "room_type "
            "ON "
                "room.room_type_id = room_type.room_type_id "
            "LEFT JOIN "
            "room_state "
            "ON "
                "room.room_state_id = room_state.room_state_id ";

    if(whereKey.size() > 0 && whereKey.size() == values.size())
    {
        QString where = "WHERE ";

        for(int i = 0; i < whereKey.size(); i++)
        {
            if(whereKey.at(i).compare("tower_range_id") == 0)
            {
                where.append(QString("room.tower_range_id = %1 ").arg(values.at(i).toInt()));
            }
            else if(whereKey.at(i).compare("tower_seat_id") == 0)
            {
                where.append(QString("room.tower_seat_id = %1 ").arg(values.at(i).toInt()));
            }
            else if(whereKey.at(i).compare("room_state_id") == 0)
            {
                where.append(QString("room.room_state_id = %1 ").arg(values.at(i).toInt()));
            }
            else if(whereKey.at(i).compare("room_type_id") == 0)
            {
                where.append(QString("room.room_type_id = %1 ").arg(values.at(i).toInt()));
            }
            else if(whereKey.at(i).compare("bed_number") == 0)
            {
                where.append(QString("room_type.bed_number = %1 ").arg(values.at(i).toInt()));
            }
            else if(whereKey.at(i).compare("room_rate") == 0)
            {
                where.append(QString("room_type.room_rate <= %1 ").arg(values.at(i).toInt()));
            }

            if(whereKey.size() >= 2 && whereKey.size()-1 != i)
            {
                where.append("AND ");
            }
        }

        sql.append(where);
    }

    QString orderby = "ORDER BY "
                        "tr_id ASC, "
                        "ts_id ASC, "
                        "tier ASC ";

    QString limit = QString("LIMIT %1, %2").arg(index).arg(showNum);

    sql.append(orderby);
    sql.append(limit);

    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(sql);

    for(int i = 0; i < records.size(); i++)
    {
        QSqlRecord record = records.at(i);

        RoomData room;
        room.room_id = record.value("room_id").toInt();
        room.tower_range_id = record.value("tr_id").toInt();
        room.tower_seat_id = record.value("ts_id").toInt();
        room.tower_tier = record.value("tier").toInt();
        room.room_num = record.value("room_num").toString();
        room.room_state_id = record.value("state_id").toInt();
        room.room_type = record.value("type").toString();
        room.tr_describe = record.value("tr_describe").toString();
        room.ts_describe = record.value("ts_describe").toString();
        room.bed_number = record.value("bed_num").toInt();
        room.room_price = record.value("rate").toInt();
        room.room_state = record.value("state").toString();

        m_data.append(room);
    }

    return m_data;
}
