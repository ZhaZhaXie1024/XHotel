#ifndef SYSSTAFFPOSSERVICE_H
#define SYSSTAFFPOSSERVICE_H

#include <QObject>

class SpeedyDao;
struct StaffPosition;

class SysStaffPosService : public QObject
{
    Q_OBJECT
public:
    explicit SysStaffPosService(QObject *parent = nullptr);

    //获取最后一个错误
    QString getLastError();

    //查询
    QList<StaffPosition> selectStaffPos();

    //新增
    bool insertData(QString code,QString describe);

    //修改
    bool updateData(int posId,QString code,QString describe);

    //删除
    bool deleteData(int posId);


private:
    SpeedyDao *m_speedyDao;

    QString m_tableName;
};

#endif // SYSSTAFFPOSSERVICE_H
