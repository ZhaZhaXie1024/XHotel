#include "sqlinterface.h"

#include "DB/sqldatabase.h"


SqlInterface::SqlInterface(QObject *parent) : QObject(parent)
{
    if(!g_myDB.isOpen())
    {
        g_myDB.open();
    }

    m_query = new QSqlQuery(g_myDB);
}

SqlInterface::~SqlInterface()
{
    deleteQuery();
}

/**
 * @brief SqlInterface::queryList
 * @param sql       Sql语句
 * @param params    参数列表
 * @return          返回查询的内容
 * 功能：查询
 */
QList<QVariantMap> SqlInterface::queryList(QString sql, QVariantList params)
{
    QList<QVariantMap> result;

    if(!g_myDB.isOpen())
    {
        g_myDB.open();
    }

    //prepare 准备查询语句，准备成功返回true
    m_query->prepare(sql);

    //绑定参数
    for(int i = 0; i < params.size(); i++)
    {
        QVariant &p = params[i];
        m_query->bindValue(i,p);
    }

    m_query->exec();

    //将数据添加进 result 中
    while (m_query->next()) {
        QSqlRecord record = m_query->record();

        int fieldCount = record.count();

        QVariantMap map;

        for(int i = 0; i < fieldCount; i++)
        {
            QString fieldName = record.fieldName(i);
            QVariant fieldValue = record.value(i);

            map.insert(fieldName,fieldValue);
        }

        result.append(map);
    }

    QSqlError error = m_query->lastError();

    if(error.type() != QSqlError::NoError)
    {
        qDebug() << "SqlInterface::queryList : " << error.text();
    }

    return result;
}

/**
 * @brief SqlInterface::Delete
 * @param sql       sql语句
 * @param params    参数
 * @return          1：成功 ， 0：失败
 * 功能：删除表中某些数据
 */
int SqlInterface::deleteData(QString sql, QVariantList params)
{
    if(!g_myDB.isOpen())
    {
        g_myDB.open();
    }

    m_query->prepare(sql);

    for(int i = 0; i < params.size(); i++)
    {
        QVariant &p = params[i];
        m_query->bindValue(i,p);
    }

    if(m_query->exec())
    {
        return m_query->numRowsAffected();
    }

    return 0;
}

/**
 * @brief SqlInterface::Insert
 * @param sql       Sql语句
 * @param params    参数列表
 * @return          是否插入成功，true 成功，false 失败
 */
bool SqlInterface::insertData(QString sql, QVariantList params)
{
    if(!g_myDB.isOpen())
    {
        g_myDB.open();
    }

    m_query->prepare(sql);

    for(int i = 0; i < params.size(); i++)
    {
        QVariant &p = params[i];
        m_query->bindValue(i,p);
    }

//    QMap<QString,QVariant> values = m_query->boundValues();

//    QMapIterator<QString,QVariant> ite(values);
//    while (ite.hasNext()) {
//        qDebug() << ite.key() << ite.next().value() << endl;
//    }

    if(m_query->exec())
    {
        return m_query->numRowsAffected();
    }

    return false;
}

/**
 * @brief SqlInterface::insertData
 * @param sql
 * @param params
 * @param params2
 * @return 是否插入成功，true 成功，false 失败
 */
bool SqlInterface::insertNotRepeatData(QString sql, QVariantList params, QVariantList params2)
{
    if(!g_myDB.isOpen())
    {
        g_myDB.open();
    }

    m_query->prepare(sql);

    int i = 0;

    for(; i < params.size(); i++)
    {
        QVariant &p = params[i];
        m_query->bindValue(i,p);
    }

    for(int j = 0; j < params2.size(); j++)
    {
        QVariant &p = params2[j];
        m_query->bindValue(i,p);
        i++;
    }

    if(m_query->exec())
    {
        return m_query->numRowsAffected();
    }

    return false;
}

/**
 * @brief SqlInterface::UpDateInfo
 * @param sql       sql语句
 * @param params    参数列表，最后一个为where条件
 * @return          是否修改成功
 */
bool SqlInterface::upDateLastWhere(QString sql, QVariantList params)
{
    if(!g_myDB.isOpen())
    {
        g_myDB.open();
    }

    m_query->prepare(sql);

    for(int i = 0; i < params.size(); i++)
    {
        QVariant &p = params[i];
        m_query->bindValue(i,p);
    }

    if(m_query->exec())
    {
        return m_query->numRowsAffected();
    }

    return false;
}

/**
 * @brief SqlInterface::upDateNotRepeat
 * @param sql       sql语句
 * @param params    参数
 * @param params2   检查值
 * @return true：修改成功，false：修改失败/无修改
 * 功能：修改一行数据，若指定的params2与数据库中的值对应，那么该数据将不会被修改
 */
bool SqlInterface::upDateNotRepeat(QString sql, QVariantList params, QVariantList params2)
{
    if(!g_myDB.isOpen())
    {
        g_myDB.open();
    }

    m_query->prepare(sql);

    int i = 0;

    for(; i < params.size(); i++)
    {
        QVariant &p = params[i];
        m_query->bindValue(i,p);
    }

    for(int j = 0; j < params2.size(); j++)
    {
        QVariant &p = params2[j];
        m_query->bindValue(i,p);
        i++;
    }

//    QMap<QString,QVariant> map = m_query->boundValues();

//    QMapIterator<QString,QVariant> ite(map);
//    while (ite.hasNext()) {
//        qDebug() << ite.key() << ite.next().value() << endl;
//    }

    if(m_query->exec())
    {
        return m_query->numRowsAffected();
    }

    return false;
}

/**
 * @brief SqlInterface::createDb
 * @return true 连接成功，false 失败
 * 功能：连接数据库
 */
bool SqlInterface::createDb()
{
    if(!g_myDB.isOpen())
    {
        return g_myDB.open();
    }
    return false;
}

/**
 * @brief SqlInterface::beginTransaction
 * @return 是否开启成功
 * 功能：开始事务
 */
bool SqlInterface::beginTransaction()
{
    if(!g_myDB.isOpen())
    {
        g_myDB.open();
    }

    return m_query->exec("START TRANSACTION;");
}

/**
 * @brief SqlInterface::endTransaction
 * @return 是否提交成功
 * 功能：提交事务
 */
bool SqlInterface::endTransaction()
{
    return m_query->exec("commit;");
}

/**
 * @brief SqlInterface::rollback
 * @return 是否回滚成功
 * 功能：回滚
 */
bool SqlInterface::rollback()
{
    return m_query->exec("rollback;");
}

/**
 * @brief SqlInterface::getQSqlDatabase
 * @return 返回数据库db
 */
QSqlDatabase SqlInterface::getQSqlDatabase()
{
    return g_myDB;
}

/**
 * @brief SqlInterface::getLastError
 * @return 错误信息
 * 功能：获取最后产生的异常
 */
QString SqlInterface::getLastError()
{
    QString text = "";

    if(m_query != nullptr)
    {
        text = m_query->lastError().text();
    }

    return text;
}

/**
 * @brief SqlInterface::deleteQuery
 * 功能：清除query
 */
void SqlInterface::deleteQuery()
{
    if(m_query != nullptr)
    {
        delete m_query;
        m_query = nullptr;
    }
}

/**
 * @brief SqlInterface::selectList
 * @param sql       sql语句
 * @param params    值
 * @return          查询的数据
 * 功能：新查询（会造成key排序，而不是我想要的顺序）不常用
 */
QList<QVariantMap> SqlInterface::selectList(QString sql, QVariantList params)
{
//    qDebug() << "SqlInterface::selectList : " << sql;

    QList<QVariantMap> result;

    if(!g_myDB.isOpen())
    {
        g_myDB.open();
    }

    //prepare 准备查询语句，准备成功返回true
    m_query->prepare(sql);

    //绑定参数
    for(int i = 0; i < params.size(); i++)
    {
        QVariant &p = params[i];
        m_query->bindValue(i,p);
    }

    m_query->exec();

    //将数据添加进 result 中
    while (m_query->next()) {
        QSqlRecord record = m_query->record();

        int fieldCount = record.count();

        QVariantMap map;

        for(int i = 0; i < fieldCount; i++)
        {
            QString fieldName = record.fieldName(i);
            QVariant fieldValue = record.value(i);

            map.insert(fieldName,fieldValue);
        }

        result.append(map);
    }

//    qDebug() << "-----------selectList 开始-----" << endl;

//    foreach (QVariantMap map,result)
//    {
//        if(map.contains("staff_id"))
//        {
//            qDebug() << __FILE__ << __LINE__ << map.value("staff_id");
//        }
//    }

//    qDebug() << "-----------selectList 结束-----" << endl;

    return result;
}

/**
 * @brief SqlInterface::selectForList
 * @param sql
 * @param params
 * @return
 * 不常用
 */
QList<QList<QVariant> > SqlInterface::selectForList(QString sql, QVariantList params)
{

//    qDebug() << "SqlInterface::selectForList : " << sql;

    QList<QList<QVariant> > result;

    if(!g_myDB.isOpen())
    {
        g_myDB.open();
    }

    //prepare 准备查询语句，准备成功返回true
    m_query->prepare(sql);

    //绑定参数
    for(int i = 0; i < params.size(); i++)
    {
        QVariant &p = params[i];
        m_query->bindValue(i,p);
    }

    m_query->exec();

    QList<QVariant> field;
    QList<QVariant> values;

    int j = 0;

    //将数据添加进 result 中
    while (m_query->next()) {

        QSqlRecord record = m_query->record();

        int fieldCount = record.count();

        for(int i = 0; i < fieldCount; i++)
        {
            if(j == 0)
            {
                field.append(record.fieldName(i));
            }

            values.append(record.value(i));
        }

//        qDebug() << record.value(0);

        j++;
    }

    if(field.size() > 0 && values.size() > 0)
    {
        result.append(field);
        result.append(values);
    }

    return result;
}

/**
 * @brief SqlInterface::selectSqlRecord
 * @param sql
 * @param params
 * @return
 * 功能：查询常用
 */
QList<QSqlRecord> SqlInterface::selectSqlRecord(QString sql, QVariantList params)
{
    QList<QSqlRecord> result;

    if(!g_myDB.isOpen())
    {
        g_myDB.open();
    }

    //prepare 准备查询语句，准备成功返回true
    m_query->prepare(sql);

    //绑定参数
    for(int i = 0; i < params.size(); i++)
    {
        QVariant &p = params[i];
        m_query->bindValue(i,p);
    }

    m_query->exec();

    while (m_query->next()) {

        QSqlRecord record = m_query->record();

        result.append(record);
    }

//    qDebug() << "-----------selectSqlRecord 开始-----" << endl;

//    //输出看一下
//    for(int i = 0;i < result.size(); i++)
//    {
//        qDebug() << __FILE__ << __LINE__ << result.at(i).value(0);
//    }

//    qDebug() << "-----------selectSqlRecord 结束-----" << endl;

    return result;
}

/**
 * @brief SqlInterface::selectSqlRecord
 * @param sql       sql
 * @param params    值
 * @param likeIndex 第几个由模糊查询
 * @return
 * 查询SqlRecord,带有模糊查询的，使用一个QList<int>来判断
 */
QList<QSqlRecord> SqlInterface::selectSqlRecord(QString sql, QVariantList params, QList<int> likeIndex)
{
    QList<QSqlRecord> result;

    if(!g_myDB.isOpen())
    {
        g_myDB.open();
    }

    //prepare 准备查询语句，准备成功返回true
    m_query->prepare(sql);

//    qDebug() << sql << endl;

    //绑定参数
    for(int i = 0; i < params.size(); i++)
    {
        QVariant &p = params[i];

        if(likeIndex.contains(i))
        {
            m_query->bindValue(i,QString("%%1%").arg(p.toString()));
        }
        else
        {
            m_query->bindValue(i,p);
        }
    }

   m_query->exec();

    while (m_query->next()) {

        QSqlRecord record = m_query->record();

        result.append(record);
    }

    return result;
}

/**
 * @brief SqlInterface::otherOperation
 * @param sql       sql
 * @param params    值
 * @return          受影响的条数
 * 功能：其它(新增/修改/删除)，绑定参数，进行查询
 */
int SqlInterface::otherOperation(QString sql, QVariantList params)
{
    if(!g_myDB.isOpen())
    {
        g_myDB.open();
    }

    m_query->prepare(sql);

    for(int i = 0; i < params.size(); i++)
    {
        QVariant &p = params[i];
        m_query->bindValue(i,p);
    }

//    QMap<QString,QVariant> map = m_query->boundValues();

//    QMap<QString,QVariant>::const_iterator ite = map.begin();

//    for(ite;ite != map.end();ite++)
//    {
//        qDebug() << ite.key() << ite.value();
//    }
//    qDebug() << "------";

    if(m_query->exec())
    {
        int num = m_query->numRowsAffected();

        if(num <= 0)
        {
            num = 1;
        }

        return num;
    }

    return 0;
}

/**
 * @brief SqlInterface::returnInsertRecordID
 * @param sql
 * @param params
 * @return
 * 新增成功返回它的ID
 */
QVariant SqlInterface::returnInsertRecordID(QString sql, QVariantList params)
{
    if(!g_myDB.isOpen())
    {
        g_myDB.open();
    }

    m_query->prepare(sql);

    for(int i = 0; i < params.size(); i++)
    {
        QVariant &p = params[i];
        m_query->bindValue(i,p);
    }

    if(m_query->exec())
    {
        return m_query->lastInsertId();
    }

    return QVariant();
}

/**
 * @brief SqlInterface::selectTableCount
 * @param sql
 * @return 某表条数
 * 查询某表条数，并且只是查询方法才有效
 */
int SqlInterface::selectTableCount(QString sql)
{
    if(!g_myDB.isOpen())
    {
        g_myDB.open();
    }

    m_query->prepare(sql);

    if(m_query->exec())
    {
        return m_query->size();
    }
    return 0;
}

/**
 * @brief SqlInterface::selectTableCount
 * @param sql
 * @param params
 * @return 条数
 * 功能：查询某表条数
 */
int SqlInterface::selectTableCount(QString sql, QVariantList params)
{
    if(!g_myDB.isOpen())
    {
        g_myDB.open();
    }

    m_query->prepare(sql);

    for(int i = 0; i < params.size(); i++)
    {
        QVariant &p = params[i];
        m_query->bindValue(i,p);
    }

    if(m_query->exec())
    {
        m_query->next();
        return m_query->value(0).toInt();
    }
    return 0;
}

