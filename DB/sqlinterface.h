#ifndef SQLINTERFACE_H
#define SQLINTERFACE_H

#include <QObject>
#include <QVariant>

class QSqlDatabase;
class QSqlQuery;
class QSqlRecord;

class SqlInterface : public QObject
{
    Q_OBJECT
public:
    explicit SqlInterface(QObject *parent = nullptr);
    ~SqlInterface();

    /*--------------旧方法----------------*/
    //查询
    QList<QVariantMap> queryList(QString sql,QVariantList params = QVariantList());

    //删除一个表中的数据
    int deleteData(QString sql,QVariantList params = QVariantList());

    //插入一行数据
    bool insertData(QString sql,QVariantList params = QVariantList());
    //插入一行数据，若指定的params2与数据库中的值对应，那么该数据将不会被插入
    bool insertNotRepeatData(QString sql,QVariantList params = QVariantList(),QVariantList params2 = QVariantList());

    //修改数据,最后一个参数是条件，只有一个条件
    bool upDateLastWhere(QString sql,QVariantList params = QVariantList());
    //修改一行数据，若指定的params2与数据库中的值对应，那么该数据将不会被修改
    bool upDateNotRepeat(QString sql,QVariantList params = QVariantList(),QVariantList params2 = QVariantList());



    /*----------------新方法--------------------------*/

    //创建连接
    bool createDb();

    //开始事务
    bool beginTransaction();

    //结束事务(提交)
    bool endTransaction();

    //回滚
    bool rollback();

    //获取数据库
    QSqlDatabase getQSqlDatabase();

    //获取最后产生的异常
    QString getLastError();

    //m_query
    void deleteQuery();

    //查询,绑定参数（会造成key排序，而不是我想要的顺序）
    QList<QVariantMap> selectList(QString sql,QVariantList params = QVariantList());

    //查询,绑定参数
    QList<QList<QVariant>> selectForList(QString sql,QVariantList params = QVariantList());

    //查询SqlRecord
    QList<QSqlRecord> selectSqlRecord(QString sql,QVariantList params = QVariantList());
    //查询SqlRecord,带有模糊查询的，使用一个QList<int>来判断
    QList<QSqlRecord> selectSqlRecord(QString sql,QVariantList params,QList<int> likeIndex);

    //其它(新增/修改/删除)，绑定参数，进行查询
    int otherOperation(QString sql,QVariantList params = QVariantList());
    //新增成功返回它的ID
    QVariant returnInsertRecordID(QString sql,QVariantList params = QVariantList());

    //查询表的条数
    int selectTableCount(QString sql);
    //查询表的条数
    int selectTableCount(QString sql,QVariantList params);


private:
    QSqlQuery *m_query;
};


#endif // SQLINTERFACE_H
