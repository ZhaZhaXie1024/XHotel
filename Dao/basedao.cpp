#include "basedao.h"
#include <QSqlDatabase>
#include <QHash>
#include <QDebug>
#include "DB/sqlinterface.h"

BaseDao::BaseDao(QObject *parent) : QObject(parent)
{
    m_interface = new SqlInterface(this);

    m_sQLSymbol.append(" = ? ");
    m_sQLSymbol.append(" = ? OR ");
    m_sQLSymbol.append(" = ? AND ");
    m_sQLSymbol.append(" != ? ");
    m_sQLSymbol.append(" != ? OR ");
    m_sQLSymbol.append(" != ? AND ");
}

/**
 * @brief BaseDao::joinTopSQL
 * @param tableName
 * @param key
 * @return
 * 功能：拼接头部部分
 */
QString BaseDao::joinTopSQL(QString tableName, QStringList key)
{
    QString sql = "SELECT ";

    if(key.size() <= 0)
    {
        sql.append("* ");
    }
    else
    {
        for(int i = 0; i < key.size(); i++)
        {
            sql.append(key.at(i));
            if(i != key.size() - 1)
            {
                sql.append(", ");
            }
        }
    }

    sql.append(" FROM ");
    sql.append(tableName);

    return sql;
}


/**
 * @brief BaseDao::selectByInfo
 * @param tabelName     表名称
 * @param key           查询条件
 * @param value         查询条件值
 * @return              键值对列表《列名，值》
 * 功能：查询表
 */
QList<QVariantMap> BaseDao::selectByInfo(QString tableName, QStringList key, QVariantList value)
{
    QString sql = QString("select * from %1").arg(tableName);

    if(key.size() != value.size())
    {
        return QList<QVariantMap>();
    }

    QString tmp = "";

    for(int i = 0; i < key.size(); i++)
    {
        if(i == 0)
        {
            tmp.append(" where ");
        }
        else
        {
            tmp.append(" and ");
        }
        tmp.append(key[i]);
        tmp.append(" = ? ");
    }

    sql.append(tmp);

    return m_interface->queryList(sql,value);
}

/**
 * @brief BaseDao::selectData
 * @param sql     半成品的SQL语句
 * @param key     字段
 * @param symbol  拼接符号
 * @param values  值
 * @return 查询出来的数据
 */
QList<QVariantMap> BaseDao::selectData(QString sql, QStringList key, QList<SQLSymbol> symbol, QVariantList values)
{
    if(key.size() != values.size() && key.size() != symbol.size())
    {
        qDebug() << "BaseDao::selectData的key与values不符" << endl;
        return QList<QVariantMap>();
    }

    QString temp = " WHERE ";

    for(int i = 0; i < key.size(); i++)
    {
        temp.append(key.at(i) +  " ");

        if(i < symbol.size())
        {
            temp.append(m_sQLSymbol[symbol[i]]);
        }
    }

    sql.append(temp);

    return m_interface->queryList(sql,values);
}

/**
 * @brief BaseDao::DeleteByInfo
 * @param tabelName     表格名称
 * @param key           条件名称，列名
 * @param value         条件值
 * @return 1：成功，0：失败
 * 功能：删除一条数据
 */
int BaseDao::deleteByInfo(QString tableName, QStringList key, QVariantList value)
{
    QString sql = QString("delete from %1 where ").arg(tableName);

    QString tmp = "";

    if(key.size() != value.size())
    {
        return 0;
    }

    for(int i = 0; i < key.size(); i++)
    {
        if(i != 0)
        {
            tmp.append(" and ");
        }
        tmp.append(key[i]);
        tmp.append(" = ? ");
    }

    sql.append(tmp);

    return m_interface->deleteData(sql,value);
}

/**
 * @brief baseDao::InsertData
 * @param tableName     表明
 * @param key           字段值
 * @param value         插入值
 * @return              是否插入成功
 * 功能：新增
 */
bool BaseDao::insertData(QString tableName, QStringList key, QVariantList value)
{
    QString sql = QString("insert into %1 ").arg(tableName);

    if(key.size() != value.size())
    {
        return false;
    }

    QString temp = "";

    int i = 0;

    for(; i < key.size(); i++)
    {
        if(i == 0)
        {
            temp.append("(");
        }

        if(i != 0)
        {
            temp.append(",");
        }

        temp.append(key[i]);

        if (i == key.size() - 1) {
            temp.append(") ");
        }
    }

    for(int j = 0; j < i; j++)
    {
        if(j == 0)
        {
            temp.append("values (");
        }

        temp.append("?");

        if(j != i-1)
        {
            temp.append(",");
        }

        else if(j == i-1)
        {
            temp.append(")");
        }
    }

    sql.append(temp);

    //qDebug() << sql;

    return m_interface->insertData(sql,value);
}

/**
 * @brief BaseDao::insertData
 * @param tableName
 * @param key
 * @param value
 * @param hash    hash中的 key值只能存放两个，一个是key，一个是value
 * @return true:新增成功，false：已有相同数据/发生错误
 */
bool BaseDao::insertDataNotRepeat(QString tableName, QStringList key, QVariantList value, QHash<QString,QList<QVariant>> hash)
{
    if(key.size() != value.size())
    {
        return false;
    }

    QString sql = QString("INSERT INTO %1 ").arg(tableName);

    QString temp = "";

    for(int i = 0; i < key.size(); i++)
    {
        if(i == 0)
        {
            temp.append("(");
        }

        if(i != 0)
        {
            temp.append(",");
        }

        temp.append(key[i]);

        if (i == key.size() - 1) {
            temp.append(") ");
        }
    }

    temp.append("SELECT ");

    for(int j = 0;j < value.size();j++)
    {
        temp.append("? ");

        if(j != value.size() - 1)
        {
            temp.append(",");
        }
    }

    QString temp2 = QString("FROM DUAL WHERE NOT EXISTS(SELECT * FROM %1 WHERE ").arg(tableName);

    //创建map的迭代器
    QHash<QString,QList<QVariant>> ::const_iterator ite = hash.begin();

    QVariantList value2;

    for(; ite != hash.end(); ite++)
    {
        if(ite.key().compare("key") == 0)
        {
            QList<QVariant> myKey = ite.value();
            for(int i = 0; i < myKey.size(); i++)
            {
                temp2.append(ite.value()[i].toString());
                temp2.append(" = ? ");
                if(i != myKey.size() - 1)
                {
                    temp2.append("OR ");
                }
            }
        }
        else if(ite.key().compare("value") == 0)
        {
            value2 = ite.value();
        }
    }

    temp2.append(")");
    temp.append(temp2);
    sql.append(temp);

    return m_interface->insertNotRepeatData(sql,value,value2);
}

/**
 * @brief BaseDao::UpDataByInfo
 * @param tableName     表名
 * @param key           key 修改的字段，最后一个字段为id字段，用于where条件
 * @param value         value 修改的值，最后一个值为id值，用于where条件
 * @return              是否成功
 * 功能：修改相关条件的表数据
 */
bool BaseDao::upDataByInfo(QString tableName, QStringList key, QVariantList value)
{
    if(key.size() != value.size())
    {
        return false;
    }

    QString sql = QString("update %1 set ").arg(tableName);

    QString tmp = "";

    for(int i = 0; i < key.size() - 1;i++)
    {
        if(i != 0)
        {
           tmp.append(" , ");
        }
        //qDebug() << key[i];
        tmp.append(key[i]);
        tmp.append(" = ? ");
    }

    tmp.append("where " + key[key.size()-1] + " = ?");

    sql.append(tmp);

    //qDebug() << sql;

    return m_interface->upDateLastWhere(sql,value);
}

/**
 * @brief BaseDao::upDataByInfoRepeat
 * @param tableName
 * @param key
 * @param value
 * @param hash  最后一个参数必须是id ,并且 hash中的 key值只能存放两个，一个是key，一个是value
 * @return
 * 功能：修改一行数据，当已有重复的数据，将不在修改
 */
bool BaseDao::upDataByInfoRepeat(QString tableName, QStringList key, QVariantList value, QHash<QString,QList<QVariant>> hash)
{
    if(key.size() != value.size())
    {
        return false;
    }

    QString sql = QString("UPDATE %1 SET ").arg(tableName);

    QString tmp = "";

    for(int i = 0; i < key.size() - 1;i++)
    {
        if(i != 0)
        {
           tmp.append(" , ");
        }
        tmp.append(key[i]);
        tmp.append(" = ? ");
    }

    tmp.append(" WHERE " + key[key.size()-1] + " = ? AND NOT EXISTS ");

    QString tmp2 = QString("( SELECT 1 FROM ( SELECT 1 FROM %1 AS table1 WHERE ").arg(tableName);

    //创建map的迭代器
    QHash<QString,QList<QVariant>>::const_iterator ite = hash.begin();

    QVariantList value2;

    for(; ite != hash.end(); ite++)
    {
        if(ite.key().compare("key") == 0)
        {
            QList<QVariant> myKey = ite.value();

            //拼装Key
            for(int i = 0; i < myKey.size(); i++)
            {
                tmp2.append("table1." + ite.value()[i].toString());

                if(i == 0){
                    tmp2.append(" != ? AND ");
                }
                else
                {
                    tmp2.append(" = ? ");
                }

                if(i != myKey.size()-1 && i != 0)
                {
                    tmp2.append(" OR ");
                }
            }
        }
        else if(ite.key().compare("value") == 0)
        {
            value2 = ite.value();
        }
    }

    tmp2.append(") AS table2 )");

    tmp.append(tmp2);
    sql.append(tmp);

    return m_interface->upDateNotRepeat(sql,value,value2);
}


/**
 * @brief BaseDao::selectByFieldInfo
 * @param tableName     表格名称
 * @param fields        字段名称
 * @param key           筛选条件
 * @param value         筛选条件值
 * @return              一个键值对列表
 */
QList<QVariantMap> BaseDao::selectByFieldInfo(QString tableName, QStringList fields, QStringList key, QVariantList value)
{
    if(key.size() != value.size())
    {
        return QList<QVariantMap>();
    }

    QString sql = "select ";

    QString fieldStr = "";//

    for(int i = 0; i < fields.size(); i++)
    {
        fieldStr.append(fields[i]);

        if((i + 1) != fields.size())
        {
            fieldStr.append(",");
        }
    }

    sql.append(fieldStr);

    QString tableStr = QString(" from %1").arg(tableName);

    sql.append(tableStr);

    if(key.size() > 0)
    {
        QString keyStr = QString(" where ");

        for(int i = 0; i < key.size(); i++)
        {
            if(i != 0)
            {
               keyStr.append(" and ");
            }
            keyStr.append(key[i]);
            keyStr.append(" = ?");
        }

        sql.append(keyStr);
    }

    return m_interface->queryList(sql,value);
}

/**
 * @brief BaseDao::selectBySqlStr
 * @param sql     sql语句
 * @param keys    筛选条件
 * @param values  筛选值
 * @return        键值对列表
 */
QList<QVariantMap> BaseDao::selectBySqlStr(QString sql, QStringList keys, QVariantList values)
{
    if(keys.size() == values.size() && keys.size() > 0)
    {
        QString temp = " where ";

        for(int i = 0; i < keys.size(); i++)
        {
            temp.append(keys[i]);
            temp.append(" = ? ");

            if(i != keys.size() - 1)
            {
                temp.append("and ");
            }
        }

        sql.append(temp);
    }

    return m_interface->queryList(sql,values);
}

/**
 * @brief BaseDao::openTransaction
 * @return
 * 功能：开启事务
 */
bool BaseDao::openTransaction()
{
    return m_interface->beginTransaction();
}

/**
 * @brief BaseDao::submitTransaction
 * @return
 * 功能：提交事务（结束）
 */
bool BaseDao::submitTransaction()
{
    return m_interface->endTransaction();
}

/**
 * @brief BaseDao::rollback
 * @return
 * 功能：回滚
 */
bool BaseDao::rollback()
{
    return m_interface->rollback();
}

/**
 * @brief BaseDao::getQSqlDatabase
 * @return 数据库
 */
QSqlDatabase BaseDao::getQSqlDatabase()
{
    return m_interface->getQSqlDatabase();
}

/**
 * @brief BaseDao::getLastError 虚函数
 * @return 获取最后产生的异常
 */
QString BaseDao::getLastError()
{
    return m_interface->getLastError();
}
