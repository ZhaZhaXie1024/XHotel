#ifndef COMBOBOXITEMSERVICE_H
#define COMBOBOXITEMSERVICE_H

#include <QObject>
#include <QVariant>

#include "Entitys/comboboxentity.h"

class SpeedyDao;
class QComboBox;

class ComboboxItemService : public QObject
{
    Q_OBJECT
public:
    explicit ComboboxItemService(QObject *parent = nullptr);

    ComboboxItemService(QString tableName,QStringList field,QObject *parent = nullptr);

    void init(QString tableName,QStringList field);
    void init(QString tableName,QStringList field,QStringList where);
    void init(QString tableName,QStringList field,QStringList where,  QList<int> sqlLinkSymbol);

    QList<ComboBoxSeleteItem> select(QVariantList values = QVariantList());
    QList<ComboBoxSeleteItem> select(QString sql,QVariantList values = QVariantList());

    //绑定下拉框
    void boundData(QComboBox *box,QList<ComboBoxSeleteItem> data );

    void boundData(QComboBox *box,QList<QVariantMap> data );

private:
    SpeedyDao *m_dao;

    QString m_sql;
};

#endif // COMBOBOXITEMSERVICE_H
