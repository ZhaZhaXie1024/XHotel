#ifndef ROOMSTATESERVICE_H
#define ROOMSTATESERVICE_H

#include <QObject>

class SpeedyDao;

struct RoomStateInfo;

class RoomStateService : public QObject
{
    Q_OBJECT
public:
    explicit RoomStateService(QObject *parent = nullptr);

    //查询
    QList<RoomStateInfo> selectRoomState();

    //新增
    bool insertRoomState(QString code,QString m_describe);

    //修改
    bool updateRoomState(int room_state_id,QString code,QString m_describe);

    //删除
    bool deleteRoomState(int  room_state_id);

    //获取最后一个错误
    QString getLastError();

private:
    SpeedyDao *m_speedyDao;

    QString m_tableName;

};

#endif // ROOMSTATESERVICE_H
