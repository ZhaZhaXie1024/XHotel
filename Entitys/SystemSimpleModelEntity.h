#ifndef SYSTEMSIMPLEMODELENTITY_H
#define SYSTEMSIMPLEMODELENTITY_H

#include <QString>

//员工职位
typedef struct StaffPosition{
    int staff_position_id;
    QString code;
    QString m_describe;

}StaffPosition;

//管理员
typedef  struct StaffOperator{
    int operator_id;
    int staff_id;
    QString staffName;
    QString depName;
    QString posName;
} StaffOperator;


#endif // SYSTEMSIMPLEMODELENTITY_H
