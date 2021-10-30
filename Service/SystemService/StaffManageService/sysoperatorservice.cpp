#include "sysoperatorservice.h"

#include <QSqlRecord>
#include <QDebug>

#include "Entitys/SystemSimpleModelEntity.h"

SysOperatorService::SysOperatorService(QObject *parent)
    : QObject(parent),
      m_tableName("operator")
{
    m_speedyDao = new SpeedyDao(this);
    m_selectSql = "";
}

/**
 * @brief SysOperatorService::initSql
 * 初始化sql语句
 */
void SysOperatorService::initSql()
{
    QStringList keys;
    keys << "operator.operator_id" << "operator.staff_id"
         << "staff.name AS 员工名称" << "department.m_describe AS 部门"
         << "staff_position.m_describe AS 职业";

    QString sql = m_speedyDao->joinSelectSQL(m_tableName,keys);

    QString leftStaff = m_speedyDao->joinLeftJoinSQL(m_tableName,"staff",QStringList() << "staff_id");
    QString leftDep = m_speedyDao->joinLeftJoinSQL("staff","department",QStringList() << "department_id");
    QString leftPos = m_speedyDao->joinLeftJoinSQL("staff","staff_position",QStringList() << "staff_position_id");

    sql.append(leftStaff);
    sql.append(leftDep);
    sql.append(leftPos);

    m_selectSql = sql;
}

/**
 * @brief SysOperatorService::select
 * @return 数据
 * 查询
 */
QList<StaffOperator> SysOperatorService::select()
{
    if(m_selectSql.isEmpty())
    {
        initSql();
    }

    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(m_selectSql);

    QList<StaffOperator> data;

    for(int i = 0; i < records.size(); i ++)
    {
        QSqlRecord record = records.at(i);

        StaffOperator item;
        item.operator_id = record.value("operator_id").toInt();
        item.staff_id = record.value("staff_id").toInt();
        item.staffName = record.value("员工名称").toString();
        item.depName = record.value("部门").toString();
        item.posName = record.value("职业").toString();

        data.append(item);
    }

    return data;
}

/**
 * @brief SysOperatorService::selectStaff
 * @param keys
 * @param values
 * @return staff数据
 * 通过字段进行赛选查询员工信息
 */
QList<StaffOperator> SysOperatorService::selectStaff(QStringList whereKeys, QVariantList whereValues)
{
    QString sql = m_speedyDao->joinSelectSQL("staff",
                                             QStringList() << "staff.staff_id AS ID" << "staff.name AS staffName"
                                             << "department.m_describe AS dep" << "staff_position.m_describe AS pos");



    QString leftDep = m_speedyDao->joinLeftJoinSQL("staff","department",QStringList() << "department_id");
    QString leftPos = m_speedyDao->joinLeftJoinSQL("staff","staff_position",QStringList() << "staff_position_id");

    sql.append(leftDep);
    sql.append(leftPos);

    int keySize = whereKeys.size();
    int valueSize = whereValues.size();

    QList<SQLLinkSymbol> symbol;
    QList<int> like;

    for(int i = 0; i < keySize; i++)
    {
        if(whereKeys.at(i).compare("name") == 0)
        {
            if(keySize-1 != i)
            {
                symbol.append(SQLLinkSymbol::LinkLIKEAND);
            }
            else
            {
                symbol.append(SQLLinkSymbol::LinkLIKE);
            }
            like.append(i);
        }
        else if(whereKeys.at(i).compare("id_card") == 0)
        {
            if(keySize-1 != i)
            {
                symbol.append(SQLLinkSymbol::LinkEqualAND);
            }
            else
            {
                symbol.append(SQLLinkSymbol::LinkEqual);
            }

        }
        else if(whereKeys.at(i).compare("phone") == 0)
        {
            if(keySize-1 != i)
            {
                symbol.append(SQLLinkSymbol::LinkEqualAND);
            }
            else
            {
                symbol.append(SQLLinkSymbol::LinkEqual);
            }
        }
    }

    int symbolSize = symbol.size();

    QString whereSql;

    if(keySize > 0 && valueSize == keySize && symbolSize == keySize)
    {
        //名字，身份证，电话号码
        whereSql = m_speedyDao->joinWhereSQL(QStringList() << "staff",whereKeys,
                                                     symbol);

    }

    if(whereSql.length() <= 0)
    {
        whereSql.append(" WHERE staff.staff_id NOT IN (SELECT operator.staff_id FROM operator ) ");
    }
    else
    {
        whereSql.append(" AND staff.staff_id NOT IN (SELECT operator.staff_id FROM operator ) ");
    }

    sql.append(whereSql);

    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(sql,whereValues,like);

    QList<StaffOperator> data;

    for(int i = 0; i < records.size(); i ++)
    {
        QSqlRecord record = records.at(i);

        StaffOperator item;
        int id = record.value("ID").toInt();
        item.operator_id = id;
        item.staff_id = id;
        item.staffName = record.value("staffName").toString();
        item.depName = record.value("dep").toString();
        item.posName = record.value("pos").toString();

        data.append(item);
    }

    return data;
}

/**
 * @brief SysOperatorService::insertOperator
 * @param staffID
 * @param password
 * @return
 * 新增一位管理员
 */
bool SysOperatorService::insertOperator(int staffID, QString password)
{
    QString sql = m_speedyDao->joinInsertSQL(m_tableName,QStringList() << "staff_id" << "s_password");
    return m_speedyDao->insertSql(sql,QVariantList() << staffID << password);
}

/**
 * @brief SysOperatorService::updateOperator
 * @param operatorID
 * @param password
 * @return
 * 修改管理员密码
 */
bool SysOperatorService::updateOperator(int operatorID, QString password)
{
    QString sql = m_speedyDao->joinUpdateSQL(m_tableName,QStringList() << "s_password");
    sql.append(m_speedyDao->joinWhereSQL(QStringList() << m_tableName,QStringList() << "operator_id",QList<SQLLinkSymbol>()<<LinkEqual));
    return m_speedyDao->updateSql(sql,QVariantList() << password << operatorID);
}

/**
 * @brief SysOperatorService::deleteOperator
 * @param operatorID
 * @return
 * 删除操作员
 */
bool SysOperatorService::deleteOperator(int operatorID)
{
    QString sql = m_speedyDao->joinDeleteSQL(m_tableName);
    sql.append(m_speedyDao->joinWhereSQL(QStringList() << m_tableName,QStringList() << "operator_id",QList<SQLLinkSymbol>() << LinkEqual));
    return m_speedyDao->deleteSql(sql,QVariantList() << operatorID);
}

/**
 * @brief SysOperatorService::getLastError
 * @return 错误
 * 获取SQL最后的错误
 */
QString SysOperatorService::getLastError()
{
    return m_speedyDao->getLastError();
}
