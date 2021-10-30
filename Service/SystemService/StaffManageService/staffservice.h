#ifndef STAFFSERVICE_H
#define STAFFSERVICE_H

#include <QObject>
#include <QVariant>

class SpeedyDao;
class CustomTableModel;
class QAbstractItemModel;
class PagingClass;

class ComboboxItemService;
class ComboBoxSeleteItem;

class StaffService : public QObject
{
    Q_OBJECT
public:
    explicit StaffService(QObject *parent = nullptr);

    //初始化model
    void initModel();

    //获取model
    QAbstractItemModel *getModel();

    //获取行
    int getRow();
    //获取列
    int getCol();

    //获取数据库该表全部条数
    int selectTableCount();

    //修改页数，更新数据
    bool setPage(int page);

    //------------------------------

    //新增
    bool insertData(QString staffName,QString phone,
                    QString idCard,QString sex,
                    int depID,int posID,QDate date);

    //修改
    bool updateData(int staffID,QString staffName,
                    QString phone,QString idCard,
                    QString sex,int depID,int posID,QDate date);

    //删除
    bool deleteData(int staffID);

    //获取最后的错误
    QString getLastError();

    //提交
    int commiteData();

    //回滚
    void backRoll();

    //返回正确的列名
    QString fieldName(QString chineseFieldName);

    //返回下拉框数据
    QList<ComboBoxSeleteItem> comboBoxData(QString tableName,QStringList field);

public slots:
    //刷新数据
    void select();

    //设置limit
    void setLimit(int index,int showCount);

    //当页码发生变化，刷新
    void pageChanged(int index,int showCount);

    //转发总页数
    void onPageCount(int num);
    //转发pageErrorStr
    void onPageError(QString str);

    //接收来自 CustomTableModel 修改信号
    void onModelUpdateAppendSql(int id,QString field,QVariant value);

private:
    QString m_tableName;

    SpeedyDao *m_speedyDao;
    CustomTableModel *m_model;
    ComboboxItemService *m_comboBoxService;

    PagingClass *m_paging;

    QString m_sql;
    QString m_countSql;

    int m_col;  //列数,

    //直接编辑表格修改用到

    //开始索引
    int m_index;

    //int：id，int：字段/值索引
    QMap<int,int> m_updateMap;
    //QString :字段
    QList<QList<QString> > m_updateField;
    //QVariant：值
    QList<QList<QVariant> > m_updateValues;
signals:
    void updataUiPageCount(int m_pageCount);
    void pageError(QString error);
};

#endif // STAFFSERVICE_H
