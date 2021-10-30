#ifndef TEMINITIALVALUESERVICE_H
#define TEMINITIALVALUESERVICE_H

#include <QObject>

class SpeedyDao;
class TeminitialValueInfo;

class TeminitialValueService : public QObject
{
    Q_OBJECT
public:
    explicit TeminitialValueService(QObject *parent = nullptr);

    //查询
    QList<TeminitialValueInfo> select();

    //新增
    bool insertTeminital(QVariantList values);

    //删除
    bool deleteTeminital(int systemInitial_value_id);

private:
    SpeedyDao *m_speedyDao;

    QString m_tableName;

};

#endif // TEMINITIALVALUESERVICE_H
