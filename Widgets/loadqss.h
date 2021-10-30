#ifndef LOADQSS_H
#define LOADQSS_H

#include <QObject>

class LoadQss : public QObject
{
    Q_OBJECT
public:
    explicit LoadQss(QObject *parent = nullptr);

    static void loadQss();
    static void loadQssFilePath(QString path);

public:
    static QString g_qssPath;

private:

};

#endif // LOADQSS_H
