#include <QApplication>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QString>


#include "DB/sqlhelper.h"
#include "Dao/basedao.h"
#include "Widgets/Login/loginwidget.h"
#include "Widgets/loadqss.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SqlHelper sqlHelper;
    sqlHelper.initDB();
    sqlHelper.openDB();

    //加载默认的qss样式
    LoadQss::loadQss();

    //登录widget
    LoginWidget loginWidget;

    loginWidget.show();

    int e = a.exec();

    //当程序重启
    if(e == 773)
    {
        QProcess::startDetached(qApp->applicationFilePath(), QStringList());
        return 0;
    }

    return e;
}

