#include "loadqss.h"

#include <QFile>
#include <QTextStream>

#include <QApplication>

QString LoadQss::g_qssPath = ":/qss/hotelQss.qss";

LoadQss::LoadQss(QObject *parent) : QObject(parent)
{

}

/**
 * @brief LoadQss::loadQss
 * 功能：加载默认的qss
 */
void LoadQss::loadQss()
{
    loadQssFilePath(":/qss/hotelQss.qss");
}

/**
 * @brief LoadQss::loadQssFilePath
 * @param path  qss文件路径
 */
void LoadQss::loadQssFilePath(QString path)
{
    g_qssPath = path;

    QFile file(g_qssPath);

    if(!file.open(QIODevice::ReadOnly))
    {
        return;
    }

    QTextStream textStream(&file);

    QString qss = textStream.readAll();

    file.close();

    qApp->setStyleSheet(qss);
}
