#ifndef SYSDEPARTMENTSERVICE_H
#define SYSDEPARTMENTSERVICE_H

#include <QObject>


class BaseDao;
class QSqlTableModel;
class SpeedyDao;

class SysDepartmentService : public QObject
{
    Q_OBJECT
public:
    explicit SysDepartmentService(QObject *parent = nullptr);

    QSqlTableModel* selectDepAll();

    //查询部门是否拥有，该数据
    int selectDepOwn(int depId,QString fieldName,QVariant value);

    //test
    void sqlTest();

    void refreshTable();

    bool insertDepData(QString code,QString depName);
    bool updateDepData(int depId,QString code,QString depName);
    int deleteDepData(int depId);

    QString getLastError();

private:
    BaseDao *m_baseDao;
    SpeedyDao *m_speedyDao;
    QSqlTableModel *m_model;
    QString m_tableName;
};

#endif // SYSDEPARTMENTSERVICE_H
