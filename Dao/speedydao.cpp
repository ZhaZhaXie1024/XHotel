#include "speedydao.h"

#include "DB/sqlinterface.h"

#include <QSqlRecord>
#include <QSqlDatabase>
#include <QDebug>

SpeedyDao::SpeedyDao(QObject *parent) : QObject(parent)
{
    m_interface = new SqlInterface(this);

    m_sqlLinkSymbol.append(" = ? ");
    m_sqlLinkSymbol.append(" = ? OR ");
    m_sqlLinkSymbol.append(" = ? AND ");
    m_sqlLinkSymbol.append(" != ? ");
    m_sqlLinkSymbol.append(" != ? OR ");
    m_sqlLinkSymbol.append(" != ? AND ");
    m_sqlLinkSymbol.append(" LIKE ? ");
    m_sqlLinkSymbol.append(" NOT LIKE ? ");
    m_sqlLinkSymbol.append(" LIKE ?  OR ");
    m_sqlLinkSymbol.append(" LIKE ?  AND ");
    m_sqlLinkSymbol.append(" NOT LIKE ?  OR ");
    m_sqlLinkSymbol.append(" NOT LIKE ? AND ");
}

/**
 * @brief SpeedyDao::getLastError
 * @return 异常信息
 * 功能：获取最后一个异常文本信息
 */
QString SpeedyDao::getLastError()
{
    return m_interface->getLastError();
}

/**
 * @brief SpeedyDao::joinSelectSQL
 * @param tableName     表名
 * @param keys          字段
 * @return              SELECT头部语句
 * 功能：拼接查询语句的头部
 */
QString SpeedyDao::joinSelectSQL(QString tableName, QStringList keys)
{
    QString sql = "SELECT ";

    int keySize = keys.size();

    if(keySize <= 0)
    {
        sql.append("* ");
    }
    else
    {
        for(int i = 0; i < keySize; i++)
        {
            sql.append(keys.at(i));
            if(i != keySize - 1)
            {
                sql.append(", ");
            }
        }
    }

    sql.append(" FROM ");
    sql.append(tableName + " ");

    //qDebug() << "SELECT : " << sql;

    return sql;
}

/**
 * @brief SpeedyDao::joinInsertSQL
 * @param tableName     表名
 * @param keys          字段
 * @return              INSERT头部语句
 * 功能：拼接新增语句的头部
 */
QString SpeedyDao::joinInsertSQL(QString tableName, QStringList keys)
{
    QString sql = QString("INSERT INTO %1 ( ").arg(tableName);
    QString valuesSql = ") VALUES ( ";

    int keySize = keys.size();

    for(int i = 0; i < keySize; i++)
    {
        sql.append(keys.at(i));
        valuesSql.append(" ? ");
        if(i != keySize - 1)
        {
            sql.append(", ");
            valuesSql.append(", ");
        }
        else
        {
            valuesSql.append(" ) ");
        }
    }

    sql.append(valuesSql);

    //qDebug() << "INSERT : "<< sql;

    return sql;
}

/**
 * @brief SpeedyDao::joinUpdateSQL
 * @param tableName     表名
 * @param keys          字段
 * @return              UPDATE头部语句
 * 功能：接修改语句的头部
 */
QString SpeedyDao::joinUpdateSQL(QString tableName, QStringList keys)
{
    QString sql = QString("UPDATE %1 SET ").arg(tableName);

    int kyeSize = keys.size();

    for(int i = 0; i < kyeSize; i++)
    {
        sql.append(keys.at(i) + " = ? ");

        if(i != kyeSize - 1)
        {
            sql.append(", ");
        }
    }

    //qDebug() << "UPDATE : " << sql;

    return sql;
}

/**
 * @brief SpeedyDao::joinDeleteSQL
 * @param tableName 表名
 * @return          DELETE头部语句
 * 功能：拼接删除语句的头部
 */
QString SpeedyDao::joinDeleteSQL(QString tableName)
{
    QString sql = QString("DELETE FROM %1 ").arg(tableName);

    //qDebug() << "DELETE : " << sql;

    return sql;
}

/**
 * @brief SpeedyDao::joinNumSQL
 * @param tableName 表名
 * @return          计算条数头部SQL语句
 * 功能：拼接查询表的数据条数的头部
 */
QString SpeedyDao::joinNumSQL(QString tableName)
{
    QString sql = QString("SELECT COUNT(*) AS COUNTNUM FROM %1 ").arg(tableName);

    //qDebug() << "JOIN NUM : "  << sql;

    return sql;
}

/**
 * @brief SpeedyDao::leftJoinSQL
 * @param tableName         左表名
 * @param joinTableName     右表名（被连接）
 * @param keys               第一个值是左表名的keys，第二个值是右表名的keys
 * @return                  LEFT JOIN语句
 * 功能：拼接LEFT JOIN
 */
QString SpeedyDao::joinLeftJoinSQL(QString tableName, QString joinTableName, QStringList keys)
{
    QString sql = QString(" LEFT JOIN %1 ON ").arg(joinTableName);

    int keySize = keys.size();

    if(keySize == 1)
    {
        sql.append(tableName+"."+keys.at(0) + " = ");
        sql.append(joinTableName+"."+keys.at(0) + " ");
    }
    else if(keySize == 2 || keySize > 2)
    {
        sql.append(tableName+"."+keys.at(0) + " = ");
        sql.append(joinTableName+"."+keys.at(1) + " ");
    }
    else if(keySize < 1)
    {
        qDebug() << "SpeedyDao::leftJoinSQL 拼接keys异常" << endl;
        return "";
    }

    //qDebug() << "LEFT JOIN : "   << sql;

    return sql;
}

/**
 * @brief SpeedyDao::joinWhereSQL
 * @param tableName
 * @param keys
 * @return 返回拼接好的where条件
 * 拼接WHERE条件（单表,且只有and =）
 */
QString SpeedyDao::joinWhereSQL(QString tableName, QStringList keys)
{
    QString sql = " WHERE ";

    int keySize = keys.size();

    if(keySize <= 0)
    {
        qDebug() << "SpeedyDao::joinWhereSQL (单表) keys异常 :" << endl;
        return "";
    }

    for(int i = 0; i < keys.size(); i++)
    {
        QString key = keys.at(i);

        sql.append(tableName + "." + key);

        if(i != keySize-1)
        {
            sql.append(" = ? AND ");
        }
        else
        {
            sql.append(" = ? ");
        }
    }

    return sql;
}

/**
 * @brief SpeedyDao::joinWhereSQL
 * @param tableNames        表名（别名）
 * @param keys              字段
 * @param sqlLinkSymbol     连接符号
 * @return                  WHERE语句
 * 功能：拼接WHERE条件(简单)
 */
QString SpeedyDao::joinWhereSQL(QStringList tableNames, QStringList keys, QList<SQLLinkSymbol> sqlLinkSymbol)
{
    QString sql = " WHERE ";

    int tableNameSize = tableNames.size();
    int keySize = keys.size();
    int linkSqlSymbolSize = sqlLinkSymbol.size();


    if(tableNameSize == 1 && keySize == linkSqlSymbolSize)
    {
        for(int i = 0; i < keySize; i++)
        {
            sql.append(tableNames.at(0) + ".");
            sql.append(keys.at(i));
            sql.append(m_sqlLinkSymbol.at(sqlLinkSymbol.at(i)));
        }
    }
    else
    {
        if(tableNameSize != keySize ||
                tableNameSize != linkSqlSymbolSize ||
                keySize != linkSqlSymbolSize)
        {
            qDebug() << "SpeedyDao::joinWhereSQL 拼接时出现异常 :" <<
                        tableNameSize << "," <<
                        keySize << "," <<
                        linkSqlSymbolSize << "," << endl;
            return "";
        }

        for(int i = 0; i < tableNameSize; i++)
        {
            sql.append(tableNames.at(i) + ".");
            sql.append(keys.at(i));
            sql.append(m_sqlLinkSymbol.at(sqlLinkSymbol.at(i)));
        }
    }

    //qDebug() << "WHERE :" << sql;

    return sql;
}

/**
 * @brief SpeedyDao::joinGroupBySQL
 * @param tableNames    表名（表别名）
 * @param keys          字段
 * @return              GroupBy子句
 * 功能：拼接GROUP BY
 */
QString SpeedyDao::joinGroupBySQL(QStringList tableNames, QStringList keys)
{
    QString sql = " GROUP BY ";

    int tableNameSize = tableNames.size();
    int keySize = keys.size();

    if(tableNameSize == 1)
    {
        for(int i = 0; i < keySize; i++)
        {
            sql.append(tableNames.at(0) + ".");
            sql.append(keys.at(i));
            if(i != keySize-1)
            {
                sql.append(",");
            }
        }
    }
    else
    {
        if(tableNameSize <= 0 || tableNameSize != keySize)
        {
            qDebug() << "SpeedyDao::joinGroupBySQL 拼接时出现异常 :" << endl;
            return "";
        }

        for(int i = 0; i < tableNameSize; i++)
        {
            sql.append(tableNames.at(i) + ".");
            sql.append(keys.at(i));
            if(i != keySize-1)
            {
                sql.append(",");
            }
        }
    }

    //qDebug() << "GROUP BY:" << sql;

    return sql;
}

/**
 * @brief SpeedyDao::joinHavingSQL
 * @param tableNames        表名
 * @param keys              字段
 * @param sqlLinkSymbol     连接符
 * @return                  HAVING子句
 * 功能：拼接HAVING子句
 */
QString SpeedyDao::joinHavingSQL(QStringList tableNames, QStringList keys, QList<SQLLinkSymbol> sqlLinkSymbol)
{
    QString sql = " HAVING ";

    int tableNameSize = tableNames.size();
    int keySize = keys.size();
    int linkSqlSymbolSize = sqlLinkSymbol.size();


    if(tableNameSize == 1 && keySize == linkSqlSymbolSize)
    {
        for(int i = 0; i < keySize; i++)
        {
            sql.append(tableNames.at(0) + ".");
            sql.append(keys.at(i));
            sql.append(m_sqlLinkSymbol.at(sqlLinkSymbol.at(i)));
        }
    }
    else
    {
        if(tableNameSize != keySize ||
                tableNameSize != linkSqlSymbolSize ||
                keySize != linkSqlSymbolSize)
        {
            qDebug() << "SpeedyDao::joinHavingSQL 拼接时出现异常 :" <<
                        tableNameSize << "," <<
                        keySize << "," <<
                        linkSqlSymbolSize << "," << endl;
            return "";
        }

        for(int i = 0; i < tableNameSize; i++)
        {
            sql.append(tableNames.at(i) + ".");
            sql.append(keys.at(i));
            sql.append(m_sqlLinkSymbol.at(sqlLinkSymbol.at(i)));
        }
    }

    //qDebug() << "HAVING :" << sql;

    return sql;
}

/**
 * @brief SpeedyDao::joinOrderSQL
 * @param tableNames    表名
 * @param keys          字段名
 * @param DescIndex     排序索引
 * @return              ORDERBY子句
 * 功能：拼接ORDER BY
 */
QString SpeedyDao::joinOrderSQL(QStringList tableNames, QStringList keys, QStringList Desc)
{
    QString sql = " ORDER BY ";

    int tableNameSize = tableNames.size();
    int keySize = keys.size();

    if(tableNameSize == 1)
    {
        for(int i = 0; i < keySize; i++)
        {
            sql.append(tableNames.at(0) + ".");
            sql.append(keys.at(i));
            if(Desc.size() > 0)
            {
                sql.append(" " + Desc.at(i) + " ");
            }
            if(i != keySize-1)
            {
                sql.append(",");
            }
        }
    }
    else
    {
        if(tableNameSize <= 0 || tableNameSize != keySize)
        {
            qDebug() << "SpeedyDao::joinOrderSQL 拼接时出现异常 :" << endl;
            return "";
        }

        for(int i = 0; i < tableNameSize; i++)
        {
            sql.append(tableNames.at(i) + ".");
            sql.append(keys.at(i));
            if(Desc.size() > 0)
            {
                sql.append(" " + Desc.at(i) + " ");
            }
            if(i != keySize-1)
            {
                sql.append(",");
            }
        }
    }

    //qDebug() << "ORDER BY:" << sql;

    return sql;
}

/**
 * @brief SpeedyDao::joinLimit
 * @param offset      偏移量
 * @param limitNum    限制条数
 * @return            LIMIT子句
 * 功能：拼接LIMIT
 */
QString SpeedyDao::joinLimit(int offset, int limitNum)
{
    return QString(" LIMIT %1 , %2 ").arg(offset).arg(limitNum);
}

/**
 * @brief SpeedyDao::selectSql
 * @param sql       完整的SQL语句
 * @param values    值
 * @return          查询出来的值
 * 功能：查询
 */
QList<QVariantMap> SpeedyDao::selectSql(QString sql, QVariantList values)
{
    return m_interface->selectList(sql,values);
}

/**
 * @brief SpeedyDao::selectSqlRecord
 * @param sql
 * @param values
 * @return
 * 功能：查询
 */
QList<QSqlRecord> SpeedyDao::selectSqlRecord(QString sql, QVariantList values)
{
    return  m_interface->selectSqlRecord(sql,values);
}

/**
 * @brief SpeedyDao::selectSqlRecord
 * @param sql
 * @param values
 * @param like
 * @return  查询,带有模糊查询
 */
QList<QSqlRecord> SpeedyDao::selectSqlRecord(QString sql, QVariantList values, QList<int> like)
{
    return  m_interface->selectSqlRecord(sql,values,like);
}

/**
 * @brief SpeedyDao::insertSql
 * @param sql       完整的SQL语句
 * @param values    值
 * @return          是否新增成功
 * 功能：新增
 */
bool SpeedyDao::insertSql(QString sql, QVariantList values)
{
    return m_interface->otherOperation(sql,values);
}

/**
 * @brief SpeedyDao::insertReturnID
 * @param sql
 * @param values
 * @return
 * 新增并返回主键ID
 */
QVariant SpeedyDao::insertReturnID(QString sql, QVariantList values)
{
    return m_interface->returnInsertRecordID(sql,values);
}

/**
 * @brief SpeedyDao::ordUpdateSql
 * @param sql
 * @param value
 * @return
 * 普通的修改,没有带事务提交的功能
 */
bool SpeedyDao::ordUpdateSql(QString sql, QVariantList value)
{
    return m_interface->otherOperation(sql,value);
}

/**
 * @brief SpeedyDao::updateSql
 * @param sql       完整的SQL语句
 * @param values    值
 * @return          是否修改成功
 * 功能：修改多条错误，带事务
 */
bool SpeedyDao::updateSql(QString sql, QVariantList values)
{
    bool isUpdate = false;
    bool isRoolback = false;

    m_interface->beginTransaction();

    int updateNum = m_interface->otherOperation(sql,values);

    if(updateNum == 1)
    {
        isUpdate = true;
    }
    else if(updateNum > 1)
    {
        m_interface->rollback();
        isRoolback = true;
        isUpdate = false;
    }

    if(!isRoolback)
    {
        m_interface->endTransaction();
    }

    return isUpdate;
}

/**
 * @brief SpeedyDao::updateToModel
 * @param sql
 * @param values
 * @return
 * 修改数据没有验证版
 */
bool SpeedyDao::updateToModel(QString sql, QVariantList values)
{
    return m_interface->otherOperation(sql,values);
}

/**
 * @brief SpeedyDao::deleteSql
 * @param sql       完整的SQL语句
 * @param values    值
 * @return          是否删除成功
 * 功能：删除
 */
bool SpeedyDao::deleteSql(QString sql, QVariantList values)
{
    bool isDelete = false;
    bool isRoolback = false;

    m_interface->beginTransaction();

    int deleteNum = m_interface->otherOperation(sql,values);

    if(deleteNum == 1)
    {
        isDelete = true;
    }
    else if(deleteNum > 1)
    {
        m_interface->rollback();
        isRoolback = true;
        isDelete = false;
    }

    if(!isRoolback)
    {
        m_interface->endTransaction();
    }

    return isDelete;
}

/**
 * @brief SpeedyDao::joinNoRepetitionInsertSQL
 * @param tableName 表名
 * @param keys      字段
 * @param values    值
 * @return          是否新增成功
 * 功能：新增不重复数据
 */
bool SpeedyDao::insertNoRepetitionSql(QString tableName, QStringList keys,QStringList whereKeys,QList<SQLLinkSymbol> sqlLinkSymbol, QVariantList values)
{
    int keySize = keys.size();
    int whereKeySize = whereKeys.size();

    if(keySize+whereKeySize != values.size())
    {
        qDebug() << "SpeedyDao::joinNoRepetitionInsertSQL 参数不齐" << endl;
        return false;
    }

    QString sql = QString("INSERT INTO %1 (").arg(tableName);

    QString temp1;

    for(int i = 0; i < keySize; i++)
    {
        sql.append(tableName + "." + keys.at(i));
        temp1.append(" ? ");

        if(i != keySize-1)
        {
            sql.append(",");
            temp1.append(",");
        }
    }

    sql.append(QString(" ) SELECT %1 FROM DUAL WHERE NOT EXISTS ( SELECT * FROM %2 AS tb1 WHERE ")
               .arg(temp1).arg(tableName));


    for(int i = 0; i < whereKeySize; i++)
    {
        sql.append("tb1." + whereKeys.at(i) + m_sqlLinkSymbol.at(sqlLinkSymbol.at(i)));
    }

    sql.append(")");

    //qDebug() << "NoRepetitionInsert ：" << sql;

    return m_interface->insertData(sql,values);
}

/**
 * @brief SpeedyDao::insertNoRepetitionReturnID
 * @param tableName
 * @param keys
 * @param whereKeys         设置不重复字段
 * @param sqlLinkSymbol
 * @param values
 * @return
 * 新增不重复，且返回新增后ID
 */
QVariant SpeedyDao::insertNoRepetitionReturnID(QString tableName, QStringList keys, QStringList whereKeys, QList<SQLLinkSymbol> sqlLinkSymbol, QVariantList values)
{
    int keySize = keys.size();
    int whereKeySize = whereKeys.size();

    if(keySize+whereKeySize != values.size())
    {
        qDebug() << "SpeedyDao::joinNoRepetitionInsertSQL 参数不齐" << endl;
        return false;
    }

    QString sql = QString("INSERT INTO %1 (").arg(tableName);

    QString temp1;

    for(int i = 0; i < keySize; i++)
    {
        sql.append(tableName + "." + keys.at(i));
        temp1.append(" ? ");

        if(i != keySize-1)
        {
            sql.append(",");
            temp1.append(",");
        }
    }

    sql.append(QString(" ) SELECT %1 FROM DUAL WHERE NOT EXISTS ( SELECT * FROM %2 AS tb1 WHERE ")
               .arg(temp1).arg(tableName));


    for(int i = 0; i < whereKeySize; i++)
    {
        sql.append("tb1." + whereKeys.at(i) + m_sqlLinkSymbol.at(sqlLinkSymbol.at(i)));
    }

    sql.append(")");

    //qDebug() << "NoRepetitionInsert ：" << sql;

//    return 0;
    return m_interface->returnInsertRecordID(sql,values);
}

/**
 * @brief SpeedyDao::joinNoRepUpdateWhere
 * @param keys              第一层字段
 * @param whereKeys         第二层where字段
 * @param sqlLinkSymbol     连接符
 * @return                  修改，不重复的where条件
 * 功能：拼接，修改且数据不重复（where）
 */
QString SpeedyDao::joinNoRepUpdateWhere(QString tableName,QStringList keys, QStringList whereKeys, QList<SQLLinkSymbol> sqlLinkSymbol)
{
    QString sql = " WHERE ";

    int keySize = keys.size();
    int whereKeySize = whereKeys.size();

    if(keySize+whereKeySize != sqlLinkSymbol.size())
    {
        qDebug() << "SpeedyDao::joinNoRepUpdateWhere 拼接参数异常" << endl;
    }

    int i = 0;

    for(; i < keySize; i++)
    {
        sql.append(keys.at(i) + m_sqlLinkSymbol.at(sqlLinkSymbol.at(i)));
    }

    sql.append(QString(" AND NOT EXISTS (SELECT 1 FROM (SELECT 1 FROM %1 AS tb1 WHERE ").arg(tableName));

    for(int j = 0; j < whereKeySize; j++)
    {
        sql.append("tb1." + whereKeys.at(j) + m_sqlLinkSymbol.at(sqlLinkSymbol.at(i)));
        i++;
    }

    sql.append(") AS tb2)");

    return sql;
}

/**
 * @brief SpeedyDao::selectTableCount
 * @param sql
 * @return 条数
 * 查询某个表的数据条数
 */
int SpeedyDao::selectTableCount(QString sql)
{
    return m_interface->selectTableCount(sql);
}

/**
 * @brief SpeedyDao::selectTableCount
 * @param sql
 * @param values
 * @return 条数
 * 使用的时候，sql语句中必须包含count(*)并且是第一个
 */
int SpeedyDao::selectTableCount(QString sql, QVariantList values)
{
    return m_interface->selectTableCount(sql,values);
}

/**
 * @brief SpeedyDao::getDatabase
 * @return
 * 获取数据库db
 */
QSqlDatabase SpeedyDao::getDatabase()
{
    return m_interface->getQSqlDatabase();
}

/**
 * @brief SpeedyDao::beginTransaction
 * @return
 * 开启事务
 */
bool SpeedyDao::beginTransaction()
{
    return m_interface->beginTransaction();
}

/**
 * @brief SpeedyDao::endTransaction
 * @return
 * 结束事务(提交)
 */
bool SpeedyDao::endTransaction()
{
    return m_interface->endTransaction();
}

/**
 * @brief SpeedyDao::rollback
 * @return
 * 回滚
 */
bool SpeedyDao::rollback()
{
    return m_interface->rollback();
}


