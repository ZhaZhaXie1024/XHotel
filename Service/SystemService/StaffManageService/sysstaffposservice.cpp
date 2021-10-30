#include "sysstaffposservice.h"

#include "Dao/speedydao.h"
#include "Entitys/SystemSimpleModelEntity.h"

#include <QDebug>

SysStaffPosService::SysStaffPosService(QObject *parent) : QObject(parent)
{
    m_speedyDao = new SpeedyDao(this);
    m_tableName = "staff_position";
}

QString SysStaffPosService::getLastError()
{
    return m_speedyDao->getLastError();
}

/**
 * @brief SysStaffPosService::selectStaffPos
 * @return 查询到的实体数据列表
 */
QList<StaffPosition> SysStaffPosService::selectStaffPos()
{
    QString sql = m_speedyDao->joinSelectSQL(m_tableName);
    QList<QVariantMap> data = m_speedyDao->selectSql(sql);

    QList<StaffPosition> staffPosList;

    for(int i = 0; i < data.size(); i++)
    {
        QVariantMap map = data.at(i);

        StaffPosition staffPos;
        staffPos.staff_position_id = map.value("staff_position_id").toInt();
        staffPos.code = map.value("code").toString();
        staffPos.m_describe = map.value("m_describe").toString();
        staffPosList.append(staffPos);
    }

    return staffPosList;
}

/**
 * @brief SysStaffPosService::insertData
 * @param code      代码
 * @param describe  描述
 * @return          是否成功
 * 功能：新增
 */
bool SysStaffPosService::insertData(QString code, QString describe)
{
    QStringList keys;
    keys << "code" << "m_describe";

    QList<SQLLinkSymbol> symbol;
    symbol << LinkEqualOR << LinkEqual;

    QVariantList values;
    values << code << describe << code << describe;

    return m_speedyDao->insertNoRepetitionSql(m_tableName,keys,keys,symbol,values);
}

/**
 * @brief SysStaffPosService::updateData
 * @param posId     id
 * @param code      代码
 * @param describe  描述
 * @return          是否成功
 * 功能：修改
 */
bool SysStaffPosService::updateData(int posId, QString code, QString describe)
{
    QStringList keys;
    keys << "code" << "m_describe";

    QString updateSqlBody = m_speedyDao->joinUpdateSQL(m_tableName,keys);

    QStringList noRepKeys;
    noRepKeys << "staff_position_id";

    QStringList noRepWhereKyes;
    noRepWhereKyes << "staff_position_id" << "code" << "m_describe";

    QList<SQLLinkSymbol> symbol;
    symbol << LinkEqual << LinkUnequalAND << LinkEqualOR << LinkEqual;

    QString updateWhere = m_speedyDao->joinNoRepUpdateWhere(m_tableName,noRepKeys,noRepWhereKyes,symbol);

    updateSqlBody.append(updateWhere);

    QVariantList values;

    values << code << describe << posId << posId << code << describe;

    qDebug() << updateSqlBody;

    return m_speedyDao->updateSql(updateSqlBody,values);
}

/**
 * @brief SysStaffPosService::deleteData
 * @param posId   id
 * @return        是否成功删除
 * 功能：删除
 */
bool SysStaffPosService::deleteData(int posId)
{
    QString deleteSql = m_speedyDao->joinDeleteSQL(m_tableName);
    QString deleteWhere = m_speedyDao->joinWhereSQL(QStringList() << m_tableName,
                                                    QStringList() << "staff_position_id",
                                                    QList<SQLLinkSymbol>() << LinkEqual);

    return m_speedyDao->deleteSql(deleteSql.append(deleteWhere),
                                  QVariantList() << posId);
}


