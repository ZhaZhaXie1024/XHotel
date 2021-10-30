#ifndef CUSTOMTABLEVIEWMODEL_H
#define CUSTOMTABLEVIEWMODEL_H

#include <QAbstractTableModel>

class SpeedyDao;
class TableModelPaging;

/**
 * @brief The CustomTableViewModel class
 * 这是一个带分页的model
 */
class CustomTableViewModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CustomTableViewModel(QObject *parent = nullptr);

    /*----------------------查询---------------------------------*/

    //设置SQL
    void setSql(QString sql);                               //单纯设置sql
    void setSql(QString sql,QStringList disHorHeaderData);  //设置sql的同时设置显示水平表头
    void setInsertSql(QString sql);                         //设置新增的修改语句

    //设置显示条数
    void setShowCount(int showCount);
    //设置当前页，跳转
    void setThisPage(int thisPage);

    //查询
    void select(QVariantList values = QVariantList(),QList<int> likeIndexs = QList<int>());

    /*----------------------------------------------------------*/

    //设置是否可编辑
    void setEditable(bool value);
    //设置是否分页
    void setOpenPaging(bool value);


    /*----------------------新增---------------------------------*/

    //开启事务
    bool openSqlTransaction();

    //提交
    bool submitAll();

    //回撤
    bool backAll();

    //私有成员
private:
    SpeedyDao *m_speedDao;                  //dao
    TableModelPaging *m_paging;             //分页

    //数据部分
    QList<QVariantList> m_data; 			//主容器，存放model数据
    QList<QVariantList> m_userData;         //存放userrole的数据

    QStringList m_headerData;   			//列头（查询、修改 字段）
    QStringList m_disHeaderData;			//列头（显示）

    QList<QVariantMap> m_updateData;     //存放 新增/修改 获得的数据容器

    //sql
    QString m_initSql;      //初始sql，不带limit
    QString m_sql;          //查询/刷新的sql
    QString m_insertSql;    //新增sql

    //表格名字（修改时用到）
    QString m_tableName;

    //判断是否第一次查询（用于获取数据总条数，计算总页数的）
    bool m_isFirstSelect;   //是否第一次查询
    bool m_isModelUpdate;   //是否被修改


    //用于新增时判断，应该返回什么flage
    bool m_isInsert;        //是否新增
    bool m_isTransaction;   //是否开启了事务
    bool m_isEditable;      //是否可编辑
    bool m_isPaging;        //是否分页

    //私有方法
private:
    //获取当前查询sql的表格名称(在setSql函数中使用)
    void getTableName();

    // 继承重写 QAbstractItemModel interface
public:
    //返回总行数
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    //返回总列数
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    //获取/显示数据/数据格式
    virtual QVariant data(const QModelIndex &index, int role) const override;

    //获取/显示表头数据
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    //设置表头数据
    virtual bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) override;


    //----------------信号-------------
signals:
    //发送总页数
    void pageCountChanged(int pageCount);

    //发送新增/修改的错误提示
    void sqlError(QString error);



    //继承重写 新增部分 QAbstractItemModel interface
public:
    //插入行
    virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
    //设置数据
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    //每一项的flages
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
};

#endif // CUSTOMTABLEVIEWMODEL_H
