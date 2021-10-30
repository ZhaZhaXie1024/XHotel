#ifndef SYSOTHERENTITYS_H
#define SYSOTHERENTITYS_H

#include <QString>

//流水号管理
typedef struct TeminitialValueInfo
{
    int systemInitial_value_id;
    QString state;
    QString m_describe;
    QString prefix;
    int init_value;
    int this_value;

}TeminitialValueInfo;

//收费项目
typedef struct ChargeItemInfo
{
    int charge_item_id;
    double money;
    QString m_describe;

}ChargeItemInfo;

//订单状态
typedef struct BillStateTypeInfo
{
    int bill_state_type_id;
    QString code;
    QString bill_state;
}BillStateTypeInfo;

#endif // SYSOTHERENTITYS_H
