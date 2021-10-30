#ifndef TOWERRANGESERVIE_H
#define TOWERRANGESERVIE_H

#include <QObject>

class SpeedyDao;
struct TowerRangInfo;

class TowerRangeServie : public QObject
{
    Q_OBJECT
public:
    explicit TowerRangeServie(QObject *parent = nullptr);

    //查询
    QList<TowerRangInfo> selectTowerRang();

    //新增
    bool insertTowerRang(QString code,QString describe,QString attribue);

    //修改
    bool updateTowerRang(int towerRangID,QString code,QString describe,QString attribue);

    //删除
    bool deleteTowerRang(int towerRangID);

    //获取最后发生的错误
    QString getLastError();

private:
    SpeedyDao *m_speedyDao;

    QString m_tableName;

};

#endif // TOWERRANGESERVIE_H
