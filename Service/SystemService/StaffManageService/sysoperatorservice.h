#ifndef SYSOPERATORSERVICE_H
#define SYSOPERATORSERVICE_H

#include <QObject>
#include "Dao/speedydao.h"

class StaffOperator;

class SysOperatorService : public QObject
{
    Q_OBJECT
public:
    explicit SysOperatorService(QObject *parent = nullptr);

    //初始化sql语句
    void initSql();

    //查询数据
    QList<StaffOperator> select();

    //查询员工数据
    QList<StaffOperator> selectStaff(QStringList whereKeys = QStringList(),
                                     QVariantList whereValues = QVariantList());

    //新增管理员
    bool insertOperator(int staffID,QString password);

    //修改一个管理员（密码）
    bool updateOperator(int operatorID,QString password);

    //删除一个管理员
    bool deleteOperator(int operatorID);

    //获取SQL最后的错误
    QString getLastError();

private:
    SpeedyDao *m_speedyDao;

    QString m_selectSql;

    QString m_tableName;
};

#endif // SYSOPERATORSERVICE_H
