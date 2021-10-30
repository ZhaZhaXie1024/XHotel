#ifndef ROOMATTRIBUESERVICE_H
#define ROOMATTRIBUESERVICE_H

#include <QObject>

class RoomAttribueInfo;
class SpeedyDao;

class RoomAttribueService : public QObject
{
    Q_OBJECT
public:
    explicit RoomAttribueService(QObject *parent = nullptr);

    //查询
    QList<RoomAttribueInfo> selectRoomAttribue();

    //新增
    bool insertRoomAttribue(QString code,QString m_describe);

    //修改
    bool updateRoomAttribue(int room_attribue_id,QString code,QString m_describe);

    //删除
    bool deleteRoomAttribue(int  room_attribue_id);

    //获取最后一个错误
    QString getLastError();

private:
    SpeedyDao *m_speedyDao;

    QString m_tableName;

};

#endif // ROOMATTRIBUESERVICE_H
