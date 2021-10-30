#include "reserveservice.h"

#include <QSqlDatabase>
#include <QSqlRecord>

#include "Dao/speedydao.h"
#include "Entitys/ReserveInfo.h"
#include <QDateTime>
#include <QDebug>

ReserveService::ReserveService(QObject *parent) : QObject(parent)
{
    m_speedyDao = new SpeedyDao(this);
    m_tableName = "client_history";
    m_padding = false;
    m_index = 0;
    m_showNum = 15;

    //查询订单，用于初始化，初始值ID和当前新增initValue
    selectBillNum();
    //用于初始化，房间状态ID:预定
    selectRoomStateID();
    //初始化 订单状态ID：取消  和 预定
    selectBillStateID();
    //处理过期预约订单
    cancelReserveBill();
}

/**
 * @brief ReserveService::getDatabase
 * @return
 * 获取数据库对象
 */
QSqlDatabase ReserveService::getDatabase()
{
    return m_speedyDao->getDatabase();
}

/**
 * @brief ReserveService::canCelReserveBill
 * 处理过期的预约订单
 */
void ReserveService::cancelReserveBill()
{
    if(m_billStateIDYY <= 0 || m_billStateIDQX <= 0)
    {
        return;
    }

    QString thisDate = QDate::currentDate().toString("yyyy-MM-dd");

    //通过预约ID 查询出订单
    QString reserveBillSql = m_speedyDao->joinSelectSQL(m_tableName,QStringList()<<"client_history.main_bill_id");
    reserveBillSql.append(m_speedyDao->joinLeftJoinSQL(m_tableName,"main_bill",QStringList()<<"main_bill_id"));
    reserveBillSql.append(m_speedyDao->joinWhereSQL("main_bill",QStringList() << "bill_state_type_id"));
    reserveBillSql.append(QString(" AND appointment_date < '%1'").arg(thisDate));

    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(reserveBillSql,QVariantList()<<m_billStateIDYY);

    if(records.size() <= 0)
    {
        return;
    }

    //修改状态为取消

    //开启事务
    m_speedyDao->beginTransaction();

    QString updateStateIDSql = m_speedyDao->joinUpdateSQL("main_bill",QStringList()<<"bill_state_type_id");

    for(int i = 0; i < records.size(); i++)
    {
        QString tempStr = updateStateIDSql;
        tempStr.append(m_speedyDao->joinWhereSQL("main_bill",QStringList()<<"main_bill_id"));

        QVariantList values;
        values << m_billStateIDQX << records.at(i).value(0);

        if(!m_speedyDao->updateSql(tempStr,values))
        {
            m_speedyDao->rollback();
            qDebug() << "ReserveService::cancelReserveBill 处理过期订单失败";
            return;
        }
    }

    //提交
    m_speedyDao->endTransaction();
}

/**
 * @brief ReserveService::selectBillNum
 * @return
 * 生成订单号
 */
QString ReserveService::selectBillNum()
{
    QString sql = m_speedyDao->joinSelectSQL("systemInitial_value",QStringList()<<"systemInitial_value_id"<<"prefix"<<"this_value");
    sql.append(m_speedyDao->joinWhereSQL("systemInitial_value",QStringList()<<"init_value_type_id"));

    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(sql,QVariantList()<<1);

    if(records.size() <= 0)
    {
        return QString("");
    }

    m_initValueID = records.at(0).value(0).toInt();

    if(m_initValueID <= 0)
    {
        return QString("");
    }

    m_initValue = records.at(0).value(2).toInt();

    QString billNum = QString("%1%2%3").arg(records.at(0).value(1).toString()).arg(QDateTime::currentDateTime().toString("yyyyMMdd")).arg(records.at(0).value(2).toString());

    return billNum;
}

/**
 * @brief ReserveService::selectRoomStateID
 * 通过预定代码，获取“预定”房间状态ID
 */
void ReserveService::selectRoomStateID()
{
    //初始化
    m_roomStateIDYY = 0;

    QString sql = m_speedyDao->joinSelectSQL("room_state",QStringList()<<"room_state_id");
    sql.append(m_speedyDao->joinWhereSQL("room_state",QStringList()<<"code"));

    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(sql,QVariantList()<<"YY");
    if(records.size() > 0)
    {
        m_roomStateIDYY = records.at(0).value(0).toInt();
    }
}

/**
 * @brief ReserveService::selectLeisureRoomStateID
 * @return
 * 获取空闲房间的房间状态ID
 */
int ReserveService::selectLeisureRoomStateID()
{
    QString sql = m_speedyDao->joinSelectSQL("room_state",QStringList()<<"room_state_id");
    sql.append(m_speedyDao->joinWhereSQL("room_state",QStringList()<<"code"));

    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(sql,QVariantList()<<"KX");
    if(records.size() > 0)
    {
        return records.at(0).value(0).toInt();
    }

    return 0;
}

/**
 * @brief ReserveService::selectBillStateID
 * 获取订单状态ID
 */
void ReserveService::selectBillStateID()
{
    //初始化
    m_billStateIDYY = 0;
    m_billStateIDQX = 0;

    QString sql = m_speedyDao->joinSelectSQL("bill_state_type",QStringList()<<"bill_state_type_id");
    sql.append(m_speedyDao->joinWhereSQL("bill_state_type",QStringList()<<"code"));

    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(sql,QVariantList()<<"YY");
    if(records.size() > 0)
    {
        m_billStateIDYY = records.at(0).value(0).toInt();
    }

    records.clear();
    records = m_speedyDao->selectSqlRecord(sql,QVariantList()<<"QX");
    if(records.size() > 0)
    {
        m_billStateIDQX = records.at(0).value(0).toInt();
    }
}

/**
 * @brief ReserveService::select
 * @return
 * 默认的查询方法
 */
QString ReserveService::select()
{
    /*
        1.订单id 2.客户id 3.客史id 4. 房间id 5.订单状态 6.主订单号 7.客户名称 8.房间号 9.预约入住时间 10.预约天数
    */
    QStringList key;
    key << "client_history.main_bill_id" << "client_history.client_id" << "client_history.client_history_id" << "client_history.room_id"
        << "bill_state_type.bill_state" << "main_bill.mian_bill_num" << "client.name" << "room.room_num" << "client_history.appointment_date"
        << "client_history.appointment_day";

    QString sql = m_speedyDao->joinSelectSQL(m_tableName,key);

    sql.append(m_speedyDao->joinLeftJoinSQL(m_tableName,"main_bill",QStringList()<<"main_bill_id"));
    sql.append(m_speedyDao->joinLeftJoinSQL("main_bill","bill_state_type",QStringList()<<"bill_state_type_id"));
    sql.append(m_speedyDao->joinLeftJoinSQL(m_tableName,"client",QStringList()<<"client_id"));
    sql.append(m_speedyDao->joinLeftJoinSQL(m_tableName,"room",QStringList()<<"room_id"));

    if(m_billStateIDYY > 0 && m_billStateIDQX > 0)
    {
        sql.append(QString(" WHERE main_bill.bill_state_type_id = %1 OR main_bill.bill_state_type_id = %2 ")
                   .arg(m_billStateIDYY).arg(m_billStateIDQX));
    }

    sql.append(m_speedyDao->joinOrderSQL(QStringList() << "main_bill",QStringList()<<"bill_state_type_id",QStringList()<<"ASC"));

    if(m_padding)
    {
        sql.append(m_speedyDao->joinLimit(m_index,m_showNum));
    }

    return sql;
}

/**
 * @brief ReserveService::select
 * @param keys
 * @param values
 * @return
 * 带条件的查询方法
 */
QString ReserveService::select(QStringList keys,QStringList values)
{
    if(keys.size() != values.size())
    {
        return select();
    }

    QStringList key;
    key << "client_history.main_bill_id" << "client_history.client_id" << "client_history.client_history_id" << "client_history.room_id"
        << "bill_state_type.bill_state" << "main_bill.mian_bill_num" << "client.name" << "room.room_num" << "client_history.appointment_date"
        << "client_history.appointment_day";
    QString sql = m_speedyDao->joinSelectSQL(m_tableName,key);

    sql.append(m_speedyDao->joinLeftJoinSQL(m_tableName,"main_bill",QStringList()<<"main_bill_id"));
    sql.append(m_speedyDao->joinLeftJoinSQL("main_bill","bill_state_type",QStringList()<<"bill_state_type_id"));
    sql.append(m_speedyDao->joinLeftJoinSQL(m_tableName,"client",QStringList()<<"client_id"));
    sql.append(m_speedyDao->joinLeftJoinSQL(m_tableName,"room",QStringList()<<"room_id"));

    bool first = true;
    bool isAdd = keys.contains(" main_bill.bill_state_type_id = ");

    QString whereSql = " WHERE ";

    for(int i = 0; i < keys.size(); i++)
    {
        first = false;
        if(i != 0)
        {
            whereSql.append(" AND ");
        }
        whereSql.append(keys.at(i) + values.at(i));
    }

    if(!isAdd)
    {
        if(first)
        {
            whereSql.append(QString(" main_bill.bill_state_type_id = %1 OR main_bill.bill_state_type_id = %2 ")
                            .arg(m_billStateIDYY).arg(m_billStateIDQX));
        }
        else
        {
            whereSql.append(QString(" AND (main_bill.bill_state_type_id = %1 OR main_bill.bill_state_type_id = %2) ")
                            .arg(m_billStateIDYY).arg(m_billStateIDQX));
        }
    }

    sql.append(whereSql);

    //order by
    sql.append(m_speedyDao->joinOrderSQL(QStringList() << "main_bill",QStringList()<<"bill_state_type_id",QStringList()<<"ASC"));

    if(m_padding)
    {
        sql.append(m_speedyDao->joinLimit(m_index,m_showNum));
    }

    return sql;
}

/**
 * @brief ReserveService::setLimit
 * @param index
 * @param showNum
 * 设置分页
 */
void ReserveService::setLimit(int index, int showNum)
{
    m_index = index;
    m_showNum = showNum;
}

/**
 * @brief ReserveService::insert
 * @param info
 * @return
 * 新增----在这里有很大的改进空间，不应该去数据库查询订单状态和订单编号的，如果，我把
 * 订单状态的预约删掉或者把订单编号的预约这条数据删掉，那么这个方法将作废，完全没用...页面上没有
 */
bool ReserveService::insert(ReserveInfo info)
{
    /*
        1.新增客户表(判断相同用户无法新增,可以同名字但是不可以同身份证和手机号)
        2.新增主账单表
        3.新增客史表
        4.修改房间状态
    */


    //开启事务
    m_speedyDao->beginTransaction();

    //新增客户表------
    QStringList clientKyes;
    clientKyes <<"sex"<<"time"<<"name"<<"iphone"<<"certificate_num";

    QStringList noRepetitionKeys;
    noRepetitionKeys << "iphone" << "certificate_num";

    QList<SQLLinkSymbol> symbols;
    symbols << LinkEqualOR << LinkEqual;

    QVariantList clientValues;
    clientValues << info.sex << info.time << info.name << info.iphone << info.certificate_num << info.iphone << info.certificate_num;

    //客户ID
    int clientID = m_speedyDao->insertNoRepetitionReturnID("client",clientKyes,noRepetitionKeys,symbols,clientValues).toInt();

    if(clientID <= 0)
    {
        m_errorStr = "新增用户时失败，新增的用户可能重复，请检查证件号或手机号！";
        m_speedyDao->rollback();
        return false;
    }

    //新增主账单表-----
    QStringList billKeys;
    billKeys << "bill_state_type_id" << "operator_id" << "mian_bill_num";

    QString insertBill = m_speedyDao->joinInsertSQL("main_bill",billKeys);

    QVariantList billValues;
    billValues << info.bill_state_type_id << info.operator_id << info.mian_bill_num;

    int billID = m_speedyDao->insertReturnID(insertBill,billValues).toInt();

    if(billID <= 0)
    {
        m_errorStr = "新增账单时失败，数据错误！";
        m_speedyDao->rollback();
        return false;
    }

    //修改初始值表，让当前值+1
    QString updateInitValue = m_speedyDao->joinUpdateSQL("systemInitial_value",QStringList()<<"this_value");
    updateInitValue.append(m_speedyDao->joinWhereSQL("systemInitial_value",QStringList()<<"systemInitial_value_id"));

    if(m_initValueID <= 0)
    {
        m_errorStr = "修改初始值表错误，找不到初始表对应的ID！";
        m_speedyDao->rollback();
        return false;
    }

    if(!m_speedyDao->ordUpdateSql(updateInitValue,QVariantList()<<m_initValue+1<<m_initValueID))
    {
        m_errorStr = "修改初始表错误！";
        m_speedyDao->rollback();
        return false;
    }

    //新增客史表-----
    QStringList clientHistoryKeys;
    clientHistoryKeys << "client_id" << "main_bill_id" << "room_id" << "appointment_day" << "appointment_date";

    QString insertClientHistory = m_speedyDao->joinInsertSQL("client_history",clientHistoryKeys);

    QVariantList clientHistoryValues;
    clientHistoryValues << clientID << billID << info.room_id << info.appointment_day << info.appointment_date;

    if(!m_speedyDao->insertSql(insertClientHistory,clientHistoryValues))
    {
        m_errorStr = "新增客史表时发生错误！";
        m_speedyDao->rollback();
        return false;
    }

    //修改房间状态-----
    QString updateRoomState = m_speedyDao->joinUpdateSQL("room",QStringList()<<"room_state_id");
    updateRoomState.append(m_speedyDao->joinWhereSQL("room",QStringList()<<"room_id"));

    if(m_roomStateIDYY <= 0)
    {
        m_errorStr = "无法获取到房间状态ID，数据错误！";
        m_speedyDao->rollback();
        return false;
    }

    if(!m_speedyDao->ordUpdateSql(updateRoomState,QVariantList()<<m_roomStateIDYY<<info.room_id))
    {
        m_errorStr = "修改房间状态时错误！";
        m_speedyDao->rollback();
        return false;
    }

    m_speedyDao->endTransaction();

    return true;
}

/**
 * @brief ReserveService::update
 * @param info
 * @return
 * 修改（预定入住日期、入住天数、订单状态、房间ID）
 */
bool ReserveService::update(const ReserveInfo &info)
{
    //房间状态：空闲
    int roomStateIDKX = selectLeisureRoomStateID();

    //获取之前的房间ID，如果一致说明没有改变房间，不一致说明改变房间，把原来的房间状态改成空闲，把现在的房间改成预约
    QString selectClientHistorySql = m_speedyDao->joinSelectSQL(m_tableName,QStringList() << "room_id");
    selectClientHistorySql.append(m_speedyDao->joinWhereSQL(m_tableName,QStringList()<<"client_history_id"));
    QList<QSqlRecord> records = m_speedyDao->selectSqlRecord(selectClientHistorySql,QVariantList()<<info.client_history_id);

    if(records.size() <= 0)
    {
        m_errorStr = "获取旧房间ID出现错误！";
        return false;
    }

    int oldRoomID = records.at(0).value(0).toInt();

    if(oldRoomID <= 0)
    {
        m_errorStr = "旧房间ID出现错误！";
        return false;
    }

    //开启事务
    m_speedyDao->beginTransaction();

    qDebug() << info.room_id << oldRoomID;

    //判断当前房间ID和旧房间ID是否一致，不一致就修改
    if(info.room_id != oldRoomID)
    {
        //将旧房间的房间状态改成空闲
        QString updateOldRoomStateIDSql = m_speedyDao->joinUpdateSQL("room",QStringList()<<"room_state_id");
        updateOldRoomStateIDSql.append(m_speedyDao->joinWhereSQL("room",QStringList()<<"room_id"));

        if(!m_speedyDao->ordUpdateSql(updateOldRoomStateIDSql,QVariantList()<<roomStateIDKX<<oldRoomID))
        {
            m_errorStr = "修改 旧 房间状态出现错误！";
            m_speedyDao->rollback();
            return false;
        }

        //修改新房间的房间状态为预定
        QString updateNewRoomStateIDSql = m_speedyDao->joinUpdateSQL("room",QStringList()<<"room_state_id");
        updateNewRoomStateIDSql.append(m_speedyDao->joinWhereSQL("room",QStringList()<<"room_id"));

        if(!m_speedyDao->ordUpdateSql(updateNewRoomStateIDSql,QVariantList()<<m_roomStateIDYY<<info.room_id))
        {
            m_errorStr = "修改 新 房间状态出现错误！";
            m_speedyDao->rollback();
            return false;
        }
    }

    //修改客史表------
    QString updateClientHistorySql = m_speedyDao->joinUpdateSQL(m_tableName,QStringList()<<"room_id"<<"appointment_day"<<"appointment_date");
    updateClientHistorySql.append(m_speedyDao->joinWhereSQL(m_tableName,QStringList()<<"client_history_id"));

    QVariantList hisValues;
    hisValues << info.room_id << info.appointment_day << info.appointment_date << info.client_history_id;

    if(!m_speedyDao->ordUpdateSql(updateClientHistorySql,hisValues))
    {
        m_errorStr = "修改客史表时，失败!";
        m_speedyDao->rollback();
        return false;
    }

    //修改主订单表
    QString updateMainBillSql = m_speedyDao->joinUpdateSQL("main_bill",QStringList()<<"bill_state_type_id"<<"operator_id");
    updateMainBillSql.append(m_speedyDao->joinWhereSQL("main_bill",QStringList()<<"main_bill_id"));

    if(!m_speedyDao->ordUpdateSql(updateMainBillSql,QVariantList()<<info.bill_state_type_id<<info.operator_id<<info.main_bill_id))
    {
        m_errorStr = "修改主订单表时，失败!";
        m_speedyDao->rollback();
        return false;
    }

    m_speedyDao->endTransaction();

    return true;
}

/**
 * @brief ReserveService::getLastError
 * @return
 * 获取最后一个错误
 */
QString ReserveService::getLastError()
{
    QString str = m_speedyDao->getLastError();
    if(str.trimmed().isEmpty())
    {
        str = m_errorStr;
    }
    return str;
}
