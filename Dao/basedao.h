#ifndef BASEDAO_H
#define BASEDAO_H

#include <QObject>

#include <QVariant>

class SqlInterface;
class QSqlDatabase;

enum SQLSymbol
{
    Equal,
    EqualOR,
    EqualAND,
    Unequal,
    UnequalOR,
    UnequalAND
};


/**
 * @brief The BaseDao class
 * 介绍：
 *      1.用于拼装SQL语句，拼装参数
 */
class BaseDao : public QObject
{
    Q_OBJECT
public:

    explicit BaseDao(QObject *parent = nullptr);

    //拼接SQL头部部分
    QString joinTopSQL(QString tableName,QStringList key = QStringList());

    //查询符合条件全部数据 全部字段都查询
    QList<QVariantMap> selectByInfo(QString tableName,QStringList key = QStringList(),QVariantList value = QVariantList());
    //完全自定义化的拼接Where条件
    QList<QVariantMap> selectData(QString sql,QStringList key = QStringList(),
                                  QList<SQLSymbol> symbol = QList<SQLSymbol>(),
                                  QVariantList values = QVariantList());

    //删除一个表中的
    int deleteByInfo(QString tableName,QStringList key = QStringList(),QVariantList value = QVariantList());

    //新增一行数据（普通的insert语句）
    bool insertData(QString tableName,QStringList key = QStringList(),QVariantList value = QVariantList());
    //新增一行数据，当已有重复的数据，将不在新增
    bool insertDataNotRepeat(QString tableName,QStringList key = QStringList(),
                             QVariantList value = QVariantList(),
                             QHash<QString,QList<QVariant>> hash = QHash<QString,QList<QVariant>>());

    //修改数据
    bool upDataByInfo(QString tableName,QStringList key = QStringList(),QVariantList value = QVariantList());
    //修改一行数据，当已有重复的数据，将不在修改
    bool upDataByInfoRepeat(QString tableName,QStringList key = QStringList(),
                            QVariantList value = QVariantList(),
                            QHash<QString,QList<QVariant>> hash = QHash<QString,QList<QVariant>>());


    //查询指定字段的符合条件的全部数据
    QList<QVariantMap> selectByFieldInfo(QString tableName,
                                         QStringList fields = QStringList(),
                                         QStringList key = QStringList(),
                                         QVariantList value = QVariantList());

    //通过sql语句进行查询
    QList<QVariantMap> selectBySqlStr(QString sql,QStringList keys = QStringList(),QVariantList values = QVariantList());


    //开始事务
    bool openTransaction();

    //结束事务(提交)
    bool submitTransaction();

    //回滚
    bool rollback();

    //获取数据库连接
    QSqlDatabase getQSqlDatabase();

    //获取最后产生的异常
    QString getLastError();

private:
    SqlInterface *m_interface;

    QStringList m_sQLSymbol;

};

#endif // BASEDAO_H
