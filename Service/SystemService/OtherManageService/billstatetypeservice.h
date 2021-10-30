#ifndef BILLSTATETYPESERVICE_H
#define BILLSTATETYPESERVICE_H

#include <QObject>

class SpeedyDao;
class BillStateTypeInfo;

class BillStateTypeService : public QObject
{
    Q_OBJECT
public:
    explicit BillStateTypeService(QObject *parent = nullptr);

    //查询
    QList<BillStateTypeInfo> select();

    //新增
    bool insertBillStateType(QString code,QString bill_state);

    //修改
    bool updateBillStateType(int bill_state_type_id,QString code,QString bill_state);

    //删除
    bool deleteBillStateType(int bill_state_type_id);

    //获取最后的错误
    QString getLastError();

private:
    SpeedyDao *m_speedyDao;

    QString m_tableName;


};

#endif // BILLSTATETYPESERVICE_H
