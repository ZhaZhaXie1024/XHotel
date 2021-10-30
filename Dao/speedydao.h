#ifndef SPEEDYDAO_H
#define SPEEDYDAO_H

#include <QObject>
#include <QVariant>

class SqlInterface;
class QSqlRecord;
class QSqlDatabase;

/**
 * @brief The SQLSymbol enum
 * 功能：连接符号
 */
enum SQLLinkSymbol
{
    LinkEqual,
    LinkEqualOR,
    LinkEqualAND,
    LinkUnequal,
    LinkUnequalOR,
    LinkUnequalAND,
    LinkLIKE,
    LinkNotLIKE,
    LinkLIKEOR,
    LinkLIKEAND,
    LinkNotLIKEOR,
    LinkNotLIKEAND
};

/**
 * @brief The SpeedyDao class
 *
 * 功能：
 * 1.用于调用QSqlInterface类操作数据库
 * 2.拼接SQL语句
 */

class SpeedyDao : public QObject
{
    Q_OBJECT
public:
    explicit SpeedyDao(QObject *parent = nullptr);

    //获取最后一个异常
    QString getLastError();

    //拼接查询语句的头部
    QString joinSelectSQL(QString tableName,QStringList keys = QStringList());

    //拼接新增语句的头部
    QString joinInsertSQL(QString tableName,QStringList keys);

    //拼接修改语句的头部
    QString joinUpdateSQL(QString tableName,QStringList keys);

    //拼接删除语句的头部
    QString joinDeleteSQL(QString tableName);

    //拼接查询表的数据条数的头部
    QString joinNumSQL(QString tableName);

    //拼接LEFT JOIN
    QString joinLeftJoinSQL(QString tableName,QString joinTableName,QStringList keys);

    //拼接WHERE条件（单表,且只有=）
    QString joinWhereSQL(QString tableName,QStringList keys);
    //拼接WHERE条件(简单)
    QString joinWhereSQL(QStringList tableNames,QStringList keys,QList<SQLLinkSymbol> sqlLinkSymbol);

    //拼接GROUP BY
    QString joinGroupBySQL(QStringList tableNames,QStringList keys);

    //拼接HAVING子句
    QString joinHavingSQL(QStringList tableNames,QStringList keys,QList<SQLLinkSymbol> sqlLinkSymbol);

    //拼接ORDER BY
    QString joinOrderSQL(QStringList tableNames,QStringList keys,QStringList Desc = QStringList());

    //拼接LIMIT
    QString joinLimit(int offset,int limitNum);

    /*-----------------*/

    //查询
    QList<QVariantMap> selectSql(QString sql,QVariantList values = QVariantList());
    QList<QSqlRecord> selectSqlRecord(QString sql,QVariantList values = QVariantList());
    QList<QSqlRecord> selectSqlRecord(QString sql,QVariantList values,QList<int> like);

    //新增
    bool insertSql(QString sql,QVariantList values);
    //新增并返回主键ID
    QVariant insertReturnID(QString sql,QVariantList values);

    //修改
    bool ordUpdateSql(QString sql,QVariantList value);      //普通的修改
    bool updateSql(QString sql,QVariantList values);        //修改多条错误(开启了事务)
    bool updateToModel(QString sql,QVariantList values);    //特定model使用

    //删除
    bool deleteSql(QString sql,QVariantList values);


   //-----特殊的SQL语句----------

    //新增，且不重复数据(完整的)
    bool insertNoRepetitionSql(QString tableName,QStringList keys,QStringList whereKeys,
                                   QList<SQLLinkSymbol> sqlLinkSymbol,QVariantList values);

    //新增不重复，且返回新增后ID
    QVariant insertNoRepetitionReturnID(QString tableName,QStringList keys,QStringList whereKeys,
                                   QList<SQLLinkSymbol> sqlLinkSymbol,QVariantList values);

    //拼接，修改且数据不重复（where）
    QString joinNoRepUpdateWhere(QString tableName,QStringList keys,QStringList whereKeys,QList<SQLLinkSymbol> sqlLinkSymbol);

    //获取表中条数
    int selectTableCount(QString sql);
    //获取数据表中的条数
    int selectTableCount(QString sql,QVariantList values);

    //获取数据库DB
    QSqlDatabase getDatabase();

    //开启事务
    bool beginTransaction();

    //结束事务(提交)
    bool endTransaction();

    //回滚
    bool rollback();


private:
    QStringList m_sqlLinkSymbol;

    SqlInterface *m_interface;
};

#endif // SPEEDYDAO_H
