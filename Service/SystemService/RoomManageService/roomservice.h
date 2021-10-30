#ifndef ROOMSERVICE_H
#define ROOMSERVICE_H

#include <QObject>
#include <QVariant>

class SpeedyDao;

class ComboboxItemService;
class ComboBoxSeleteItem;

class RoomService : public QObject
{
    Q_OBJECT
public:
    explicit RoomService(QObject *parent = nullptr);

    //查询语句
    QString selectSql(QStringList wherekeys = QStringList());
    //新增语句
    QString insertSql();

    //返回下拉框数据
    QList<ComboBoxSeleteItem> comboBoxData(QString tableName,QStringList field,
                                           QStringList whereKey = QStringList(),QVariantList values = QVariantList());

private:
    SpeedyDao *m_speedyDao;

    QString m_tableName;

    ComboboxItemService *m_comboBoxService;

signals:
    //当拼接sql中出现like发送该信号
    void onlikeIndex(int index);

};

#endif // ROOMSERVICE_H
