#ifndef RESERVESERVICE_H
#define RESERVESERVICE_H

#include <QObject>

class SpeedyDao;
struct ReserveInfo;
class QSqlDatabase;

class ReserveService : public QObject
{
    Q_OBJECT
public:
    explicit ReserveService(QObject *parent = nullptr);

    //获取数据库对象
    QSqlDatabase getDatabase();

    //处理过期的预约订单
    void cancelReserveBill();

    //生成订单号
    QString selectBillNum();
    //通过预定代码(YD)，获取“预定”房间状态ID
    void selectRoomStateID();
    //获取空闲房间的房间状态ID
    int selectLeisureRoomStateID();
    //获取订单状态ID
    void selectBillStateID();

    //默认的查询方法
    QString select();
    //带条件的查询方法
    QString select(QStringList keys,QStringList values);

    //设置分页
    void setLimit(int index,int showNum);

    //新增
    bool insert(ReserveInfo info);

    //修改
    bool update(const ReserveInfo &info);

    //获取最后一个错误
    QString getLastError();

private:
    SpeedyDao *m_speedyDao;

    QString m_tableName;

    //预约过期的订单ID
    QList<int> m_overdueBill;

    //初始值ID
    int m_initValueID;
    //当前新增initValue
    int m_initValue;

    //房间状态ID
    int m_roomStateIDYY;

    //订单状态ID
    int m_billStateIDYY;        //预约
    int m_billStateIDQX;        //取消

    //是否分页
    bool m_padding;
    int m_index;
    int m_showNum;

    //错误提示，更人性化
    QString m_errorStr;
};

#endif // RESERVESERVICE_H
