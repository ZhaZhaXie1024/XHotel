#ifndef SYSTEMROOMMANAGEENTITYS_H
#define SYSTEMROOMMANAGEENTITYS_H

#include <QString>

/*
    楼区
*/
typedef struct TowerRangInfo{

    int tower_range_id;     //id
    QString code;           //代码
    QString m_describe;     //描述
    QString attribute;      //详细描述
} TowerRangInfo;

/*
    楼座
*/
typedef struct TowerSeatInfo{

    int tower_seat_id;      //楼座id
    QString tower_range;    //楼区描述
    QString code;           //代码
    QString m_describe;     //描述

}TowerSeatInfo;

/*
    房间类型
*/
typedef struct RoomTypeInfo{

    int room_type_id;       //房间类型id
    QString code;           //房间代码
    QString m_describe;     //房间描述
    int bed_number;         //床数
    double room_rate;       //房价
}RoomTypeInfo;

/*
    房间状态
*/
typedef struct RoomStateInfo{

    int room_state_id;      //房间状态id
    QString code;           //房间状态代码
    QString m_describe;     //房间状态描述

} RoomStateInfo;

/*
    房间属性
*/
typedef struct RoomAttribueInfo{

    int room_attribute_id;  //房间属性ID
    QString code;           //房间属性代码
    QString m_describe;     //房间属性描述
} RoomAttribueInfo;

/*
    房间拥有的属性
*/
typedef struct RoomOwnAttribueInfo{
    int room_own_attribute_id;  //房间拥有的属性ID
    QString code;               //属性代码
    QString attribute;          //属性描述

    QString room_name;          //房间名称
    int room_id;                //房间ID
    int room_attribute_id;      //房间的属性ID
} RoomOwnAttribueInfo;

#endif // SYSTEMROOMMANAGEENTITYS_H
