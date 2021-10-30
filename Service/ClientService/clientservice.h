#ifndef CLIENTSERVICE_H
#define CLIENTSERVICE_H

#include <QObject>

class SpeedyDao;
class QSqlDatabase;

class ClientService : public QObject
{
    Q_OBJECT
public:
    explicit ClientService(QObject *parent = nullptr);

    //获取数据库对象
    QSqlDatabase getSqlDatabase();

    //计算总页数
    int pageCount();

    //获取查询语句
    QString select();
    //获取带条件的查询语句
    QString select(QStringList whereKeys,QStringList values);

    //设置分页
    void setLimit(int index,int shownum);


private:
    //获取数据总数
    int selectNum();

private:

    SpeedyDao *m_speedyDao;

    QString m_tableName;

    //分页
    int m_index;
    int m_showNum;

    QString m_sql;

};

#endif // CLIENTSERVICE_H
