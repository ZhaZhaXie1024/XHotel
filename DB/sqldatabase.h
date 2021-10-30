#ifndef SQLDATABASE_H
#define SQLDATABASE_H


#include <QObject>
#include <QtSql>

/* 这个位全局变量，用于存储数据库基础信息  */


//extern 全局变量,只需定义一次

extern QSqlDatabase g_myDB;     //当前数据库
extern QString g_driver;        //驱动名称
extern QString g_dbName;        //数据库名称
extern QString g_host;          //数据库地址
extern QString g_userName;      //数据库用户名
extern QString g_password;      //数据库用户密码
extern int g_port;              //端口

#endif // SQLDATABASE_H
