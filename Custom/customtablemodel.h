#ifndef CUSTOMTABLEMODEL_H
#define CUSTOMTABLEMODEL_H

#include <QAbstractTableModel>

class SpeedyDao;

class CustomTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CustomTableModel(QObject *parent = nullptr);

    //通过表名进行查询
    void setTableName(QString tableName);
    //通过sql语句进行查询
    void setSql(QString sql,QVariantList values = QVariantList());

    //进行真正意义上的查询
    void select();

    //获取行数
    int getRow();

    //获取列数
    int getCol();



    //覆写QAbstractTableModel的必备或需要的虚函数
protected:

    //返回行数
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    //返回列数
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    //设置表格数据/格式
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //修改model使用
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    //设置item的flags
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    //获取表头
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;


private:
    //dao层
    SpeedyDao *m_dao;

    //存储数据的容器
    QStringList m_header;       //表头
    QVariantList m_data;        //数据

    //UserRole的数据
    QMap<int,QVariant> m_userRoleData;  //

    //查询/刷新 用到的
private:
    QString m_sql;
    QVariantList m_values;

    //修改用到的
    //int ：当前行id，QVariantMap 列名和数据
//    QMultiMap<int,QVariantMap > m_updateMap;


signals:
    //直接编辑tableview，触发setData，在setData中发送该信号
    void updateToService(int id,QString field,QVariant value);

};

#endif // CUSTOMTABLEMODEL_H
