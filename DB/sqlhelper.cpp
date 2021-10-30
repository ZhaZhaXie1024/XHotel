#include "sqlhelper.h"

#include <QTextCodec>
#include <QDebug>
#include <QSqlDriver>

#include "DB/sqldatabase.h"

/**
 * @brief SqlHelper::SqlHelper
 * @param parent
 * 功能：将本地语言编码改成utf-8，为了防止乱码
 */
SqlHelper::SqlHelper(QObject *parent) : QObject(parent)
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
}

SqlHelper::~SqlHelper()
{
    //关闭数据库连接
    g_myDB.close();
}

/**
 * @brief SqlHelper::initDB
 * 功能：初始化数据库信息
 */
void SqlHelper::initDB()
{
    try {
        //校验驱动名称
        if(g_driver.isEmpty())
        {
            //抛出异常
            qDebug() << "数据库驱动异常或数据库驱动为空";
            throw "数据库驱动异常或数据库驱动为空";
        }
        //获取所有数据库驱动进行校验
        QStringList driverList = QSqlDatabase::drivers();

        if(!driverList.contains(g_driver)){
            //驱动异常
            qDebug() << "没有MYSQL数据库驱动";
            throw "没有MYSQL数据库驱动";
        }

        g_myDB = QSqlDatabase::addDatabase(g_driver,"hf_all");//第二个参数标识唯一，以防冲突
        g_myDB.setDatabaseName(g_dbName);
        g_myDB.setHostName(g_host);
        g_myDB.setPassword(g_password);
        g_myDB.setUserName(g_userName);
        g_myDB.setPort(g_port);

    } catch (QException e) {
        //异常
        qDebug() << "SqlHelper::initDB() : " << e.what() << endl;
    } catch (const char * e){
        qDebug() << "SqlHelper::initDB() : " << e << endl;
        delete  e;
        e = nullptr;
    }
}

/**
 * @brief SqlHelper::openDB
 * 功能：打开/链接数据库
 */
void SqlHelper::openDB()
{
    if(!g_myDB.open()){
        //失败
        qDebug() << "打开数据库失败" << endl;
    }

    //qDebug() << "驱动名称：" << g_myDB.driverName() << " 是否支持事务：" << g_myDB.driver()->hasFeature(QSqlDriver::Transactions) << endl;
}

/**
 * @brief SqlHelper::isOpenDB
 * @return true，已连接，false，未连接
 * 功能：判断数据库是否打开
 */
bool SqlHelper::isOpenDB()
{
    bool flag = g_myDB.isOpen();
    return flag;
}

/**
 * @brief SqlHelper::colseDB
 * 功能：关闭数据库链接
 */
void SqlHelper::colseDB()
{
    g_myDB.close();
}
