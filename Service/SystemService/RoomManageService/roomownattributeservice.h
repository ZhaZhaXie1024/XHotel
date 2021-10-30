#ifndef ROOMOWNATTRIBUTESERVICE_H
#define ROOMOWNATTRIBUTESERVICE_H

#include <QObject>

class SpeedyDao;
class RoomService;
class RoomAttribueService;
struct RoomAttribueInfo;
class RoomOwnAttribueInfo;

class RoomOwnAttributeService : public QObject
{
    Q_OBJECT
public:
    explicit RoomOwnAttributeService(QObject *parent = nullptr);

    //拼接 查询房间 SQL
    QString selectRoomSql(QStringList whereKeys = QStringList());

    //查询房间属性
    QList<RoomAttribueInfo> selectRoomAttribue();

    //查询房间拥有的属性
    QList<RoomOwnAttribueInfo> selectRoomOwnAttribue(int roomID);

    //新增一条房间拥有的属性
    bool insert(int roomID,int attribueID);

    //检查是否重复新增
    bool insertCheck(int roomID,int attribueID);

    //删除一条房间拥有的属性
    bool deleteOwnAttribue(int room_own_attribute_id);



public slots:
    //roomService中发送onlikeIndex信号触发该槽，发送sendIndex信号
    void sendLikeIndex(int index);

private:
    SpeedyDao *m_speedyDao;
    RoomService *m_roomService;
    RoomAttribueService *m_roomAttrService;

    QString m_tableName;

signals:
    void sendIndex(int index);

};

#endif // ROOMOWNATTRIBUTESERVICE_H
