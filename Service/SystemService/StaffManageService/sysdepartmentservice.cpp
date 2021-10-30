#include "sysdepartmentservice.h"

#include <QSqlTableModel>
#include <QSqlError>
#include <QDebug>

#include "Dao/basedao.h"
#include "Dao/speedydao.h"

SysDepartmentService::SysDepartmentService(QObject *parent) : QObject(parent),m_tableName("department")
{
    m_baseDao = new BaseDao(this);
    m_speedyDao = new SpeedyDao(this);
    m_model = nullptr;
}

/**
 * @brief SysDepartmentService::selectDepAll
 * 功能：查询所有的部门
 */
QSqlTableModel* SysDepartmentService::selectDepAll()
{
    m_model = new QSqlTableModel(this,m_baseDao->getQSqlDatabase());
    //设置它的双击编辑表格数据的策略,OnManualSubmit,需要手动调用submitAll()才能提交修改数据库。
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_model->setTable(m_tableName);
    m_model->setHeaderData(1,Qt::Horizontal,"部门代码",Qt::DisplayRole);
    m_model->setHeaderData(2,Qt::Horizontal,"部门名称",Qt::DisplayRole);

    m_model->setHeaderData(1,Qt::Horizontal,"code",Qt::EditRole);
    m_model->setHeaderData(2,Qt::Horizontal,"m_describe",Qt::EditRole);

    m_model->select();

    return m_model;
}

/**
 * @brief SysDepartmentService::selectDepOwn
 * @param depId         部门ID
 * @param fieldName     字段名
 * @param value         值
 * @return 条数
 * 功能：查询这个字段名有多少条这个值，除了这个部门ID
 */
int SysDepartmentService::selectDepOwn(int depId, QString fieldName, QVariant value)
{
    QString sql = m_baseDao->joinTopSQL(m_tableName,QStringList() << fieldName);
    int size = m_baseDao->selectData(sql,
                                     QStringList() << "department_id" << fieldName,
                                     QList<SQLSymbol>() << UnequalAND << Equal,
                                     QVariantList() << depId << value).size();

    //qDebug() << size;

    return size;
}


void SysDepartmentService::sqlTest()
{
    QStringList key;
    key << "code" << "m_descript";

    m_speedyDao->joinSelectSQL(m_tableName,key);
    m_speedyDao->joinInsertSQL(m_tableName,key);
    m_speedyDao->joinUpdateSQL(m_tableName,key);
    m_speedyDao->joinDeleteSQL(m_tableName);
    m_speedyDao->joinNumSQL(m_tableName);
    m_speedyDao->joinLeftJoinSQL(m_tableName,"leftTableName",QStringList() << "department_id");
    m_speedyDao->joinWhereSQL(QStringList() << m_tableName,key,
                              QList<SQLLinkSymbol>() << LinkUnequalAND << LinkEqual);

    m_speedyDao->joinWhereSQL(QStringList() << m_tableName << "leftTableName",key,
                              QList<SQLLinkSymbol>() << LinkUnequalAND << LinkEqual);

    m_speedyDao->joinGroupBySQL(QStringList() << m_tableName << "leftTableName",key);

    m_speedyDao->joinHavingSQL(QStringList() << m_tableName << "leftTableName",key,
                              QList<SQLLinkSymbol>() << LinkUnequalAND << LinkEqual);
}

/**
 * @brief SysDepartmentService::refreshTable
 * 功能：刷新表格
 */
void SysDepartmentService::refreshTable()
{
    m_model->select();
}

/**
 * @brief SysDepartmentService::insertDepData
 * @return true:成功，false：失败
 * 功能：新增部门
 */
bool SysDepartmentService::insertDepData(QString code,QString depName)
{
    QStringList key;
    key << "code" << "m_describe";
    QVariantList value;
    value << code << depName;

    QList<QVariant> whereKey;
    whereKey << "code" << "m_describe";

    QList<QVariant> whereValue;
    whereValue << code << depName;

    QHash<QString,QList<QVariant>> hash;
    hash.insert("key",whereKey);
    hash.insert("value",whereValue);
    return m_baseDao->insertDataNotRepeat(m_tableName,key,value,hash);
}

/**
 * @brief SysDepartmentService::updateDepData
 * @param depId
 * @param code
 * @param depName
 * @return 功能：修改部门
 */
bool SysDepartmentService::updateDepData(int depId, QString code, QString depName)
{
    QStringList key;
    key << "code" << "m_describe" << "department_id";
    QVariantList value;
    value << code << depName << depId;
    QHash<QString,QList<QVariant>> hash;

    QList<QVariant> whereKey;
    whereKey << "department_id" << "code" << "m_describe";

    QList<QVariant> whereValue;
    whereValue << depId << code << depName;

    hash.insert("key",whereKey);
    hash.insert("value",whereValue);
    return m_baseDao->upDataByInfoRepeat(m_tableName,key,value,hash);
}

/**
 * @brief SysDepartmentService::deleteDepData
 * @param depId
 * @return 删除的条数
 */
int SysDepartmentService::deleteDepData(int depId)
{
    return m_baseDao->deleteByInfo(m_tableName,
                                   QStringList() << "department_id",
                                   QVariantList() << depId);
}

/**
 * @brief SysDepartmentService::getLastError
 * @return 获取最后发生的错误
 */
QString SysDepartmentService::getLastError()
{
    return m_baseDao->getLastError();
}


