#ifndef ROOMTYPESERVICE_H
#define ROOMTYPESERVICE_H

#include <QObject>

class SpeedyDao;

struct RoomTypeInfo;

class RoomTypeService : public QObject
{
    Q_OBJECT
public:
    explicit RoomTypeService(QObject *parent = nullptr);

    //查询
    QList<RoomTypeInfo> selectRoomType();

    //新增
    bool insertRoomType(QString code,QString m_describe,int bed_number,double room_rate);

    //修改
    bool updateRoomType(int room_type_id,QString code,QString m_describe,int bed_number,double room_rate);

    //删除
    bool deleteRoomType(int room_type_id);

    //获取最后一个错误
    QString getLastError();

private:
    SpeedyDao *m_speedyDao;
    QString m_tableName;

};

#endif // ROOMTYPESERVICE_H
