#ifndef TOWERSEATSERVICE_H
#define TOWERSEATSERVICE_H

#include <QObject>

class SpeedyDao;
class TowerSeatInfo;

class TowerSeatService : public QObject
{
    Q_OBJECT
public:
    explicit TowerSeatService(QObject *parent = nullptr);

    //查询
    QList<TowerSeatInfo> selectTowerSeat();

    //新增
    bool insertTowerSeat(QString code,QString m_describe);

    //修改
    bool updateTowerSeat(int towerSeatID,QString code,QString m_describe);

    //删除
    bool deleteTowerSeat(int towerSeatID);

    //获取最后的错误
    QString getLastError();

private:
    SpeedyDao *m_speedyDao;

    QString m_tableName;

};

#endif // TOWERSEATSERVICE_H
