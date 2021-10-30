#ifndef CHARGEITEMSERVICE_H
#define CHARGEITEMSERVICE_H

#include <QObject>

class SpeedyDao;
struct ChargeItemInfo;

class ChargeItemService : public QObject
{
    Q_OBJECT
public:
    explicit ChargeItemService(QObject *parent = nullptr);

    //查询
    QList<ChargeItemInfo> select();

    //新增
    bool insertChargeItem(QVariantList values);

    //修改
    bool updateChargeItem(int charge_item_id,double money,QString m_describe);

    //删除
    bool deleteChargeItem(int charge_item_id);

    //获取最后的错误
    QString getLastError();

private:
    SpeedyDao *m_speedyDao;

    QString m_tableName;

};

#endif // CHARGEITEMSERVICE_H
