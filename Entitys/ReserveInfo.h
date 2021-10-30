#ifndef RESERVEINFO_H
#define RESERVEINFO_H

#include <QString>
#include <QDate>
#include <QDateTime>

typedef  struct ReserveInfo
{
    /*--客户表---*/
    int client_id;              //客户ID
    QString sex;                //性别
    QDate time;                 //生日
    QString name;               //名称
    QString iphone;             //电话
    QString certificate_num;    //证件号码

    /*--主账单表---*/
    int main_bill_id;           //主账单ID
    int operator_id;            //操作员ID
    int bill_state_type_id;     //订单状态ID
    QString mian_bill_num;      //账单号

    /*--客史表---*/
    int client_history_id;      //客史ID
    int room_id;                //房间ID
    int appointment_day;        //预定多少天
    QDate appointment_date;     //预定什么时候开始住


}ReserveInfo;

#endif // RESERVEINFO_H
