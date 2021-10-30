#ifndef SQLHELPER_H
#define SQLHELPER_H

#include <QObject>


/**
 * @brief The SqlHelper class
 *
 * 介绍：
 *      1.这个类用于main.cpp
 *      2.用于初始化/连接数据库,
 *      --3.作用于登录功能
 */
class SqlHelper : public QObject
{
    Q_OBJECT
public:
    explicit SqlHelper(QObject *parent = nullptr);

    ~SqlHelper();

    //初始化数据库信息
    void initDB();

    //连接数据库
    void openDB();

    //检查数据库是否打开
    bool isOpenDB();

    //关闭数据库
    void colseDB();

};

#endif // SQLHELPER_H
