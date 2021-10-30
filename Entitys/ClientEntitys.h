#ifndef CLIENTENTITYS_H
#define CLIENTENTITYS_H

#include <QString>
#include <QDate>

typedef struct ClientInfo
{
    int client_id;              //客户ID
    QString sex;                //性别
    QDate time;                 //生日
    QString name;               //名称
    QString iphone;             //电话
    QString certificate_num;    //身份证

}ClientInfo;

#endif // CLIENTENTITYS_H
