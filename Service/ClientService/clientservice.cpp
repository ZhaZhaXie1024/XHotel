#include "clientservice.h"

#include "Dao/speedydao.h"
#include <QSqlDatabase>

ClientService::ClientService(QObject *parent) : QObject(parent)
{
    m_speedyDao = new SpeedyDao(this);

    m_tableName = "client";

    m_index = 0;
    m_showNum = 15;
}

/**
 * @brief ClientService::getSqlDatabase
 * @return
 * 获取数据库对象
 */
QSqlDatabase ClientService::getSqlDatabase()
{
    return m_speedyDao->getDatabase();
}

/**
 * @brief ClientService::pageCount
 * @return 总页数
 * 计算总页数
 * 计算公式：总条数/显示条数
 */
int ClientService::pageCount()
{
    int count = selectNum();
    int pageCount = count/m_showNum;

    if(count%m_showNum > 0)
    {
        pageCount++;
    }

    return pageCount;
}

/**
 * @brief ClientService::selectNum
 * @return 数据总数
 * 获取数据总数
 */
int ClientService::selectNum()
{
    QString sql = m_speedyDao->joinSelectSQL(m_tableName,QStringList()<<"client_id");
    return m_speedyDao->selectTableCount(sql);
}

/**
 * @brief ClientService::select
 * @return
 * 获取查询语句
 */
QString ClientService::select()
{
    //0.客户ID 1.姓名 2.性别 3.生日 4.电话 5.证件号
    QString sql = m_speedyDao->joinSelectSQL(m_tableName,QStringList()<<"client_id"<<"name"<<"sex"<<"time"<<"iphone"<<"certificate_num");
    sql.append(m_speedyDao->joinLimit(m_index,m_showNum));

    m_sql = sql;

    return m_sql;
}

/**
 * @brief ClientService::select
 * @param whereKeys
 * @param values
 * @return
 * 获取带条件的查询语句
 */
QString ClientService::select(QStringList whereKeys, QStringList values)
{
    if(whereKeys.size() != values.size())
    {
        //返回上一次查询的sql
        return m_sql;
    }

    QString sql = m_speedyDao->joinSelectSQL(m_tableName,QStringList()<<"client_id"<<"sex"<<"name"<<"time"<<"iphone"<<"certificate_num");

    //where，手动拼
    QString whereSql = " WHERE ";

    for(int i = 0; i < whereKeys.size(); i++)
    {
        if(i != 0)
        {
            whereSql.append(" AND ");
        }

        if(whereKeys.at(0).compare("name") == 0)
        {
            //模糊查询
            whereSql.append(m_tableName + '.' + whereKeys.at(i) + " LIKE '" + values.at(i) + "' ");
        }
        else
        {
            //精确查询(电话和证件号)
            whereSql.append(m_tableName + '.' + whereKeys.at(i) + " = '" + values.at(i) + "' ");
        }
    }

    if(whereKeys.size() > 0)
    {
        sql.append(whereSql);
    }

    //limit
    sql.append(m_speedyDao->joinLimit(m_index,m_showNum));

    m_sql = sql;

    return m_sql;
}

/**
 * @brief ClientService::setLimit
 * @param index
 * @param shownum
 * 设置分页
 */
void ClientService::setLimit(int index, int shownum)
{
    m_index = index;
    m_showNum = shownum;
}


