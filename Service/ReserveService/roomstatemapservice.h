#ifndef ROOMSTATEMAPSERVICE_H
#define ROOMSTATEMAPSERVICE_H

#include <QObject>
#include <QVariant>

class SpeedyDao;
struct RoomData;

class RoomStateMapService : public QObject
{
    Q_OBJECT
public:
    explicit RoomStateMapService(QObject *parent = nullptr);

    //查询
    QList<RoomData> selectRoom(int index,int showNum,QStringList whereKey = QStringList(),QVariantList values = QVariantList());

private:
    SpeedyDao *m_speedyDao;

    QString m_tableName;
};

#endif // ROOMSTATEMAPSERVICE_H
