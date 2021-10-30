#ifndef ROOMDATA_H
#define ROOMDATA_H

#include <QString>

typedef struct RoomData{

    int room_id;            //房间ID
    int room_state_id;      //房间状态ID

    int tower_range_id;     //楼区ID
    int tower_seat_id;      //楼座ID
    int tower_tier;         //楼层

    QString tr_describe;    //楼区
    QString ts_describe;    //楼座
    QString room_num;       //房间号
    QString room_type;      //房间类型
    double room_price;      //房间价格
    QString room_state;     //房间状态
    int bed_number;         //床位数

} RoomData;

#endif // ROOMDATA_H
