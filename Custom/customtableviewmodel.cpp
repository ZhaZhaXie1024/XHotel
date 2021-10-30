#include "customtableviewmodel.h"

#include <QSqlRecord>
#include <QRegularExpression>
#include <QDebug>

#include "Dao/speedydao.h"
#include "Custom/tablemodelpaging.h"

//#define 预处理、宏定义
//将 cout 替换 qDebug() 并且自带一些输出数据
#define cout qDebug() << "[" << __FILE__ << ":" << __LINE__ << "] "

CustomTableViewModel::CustomTableViewModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_speedDao = new SpeedyDao(this);
    m_paging = new TableModelPaging(this);
    m_isFirstSelect = true;
    m_isInsert = false;
    m_isTransaction = false;
    m_isEditable = true;
    m_isPaging = true;
}

/*------------------公有方法-----------------------*/

/**
 * @brief CustomTableViewModel::setSql
 * @param sql sql语句
 * 单纯设置sql
 */
void CustomTableViewModel::setSql(QString sql)
{
    m_initSql = sql;
    m_sql = sql;
    m_isFirstSelect = true;
    getTableName();
}

/**
 * @brief CustomTableViewModel::setSql
 * @param sql sql语句
 * @param disHorHeaderData 水平表头显示的数据
 * 设置sql的同时设置显示水平表头
 */
void CustomTableViewModel::setSql(QString sql, QStringList disHorHeaderData)
{
    m_disHeaderData = disHorHeaderData;
    m_initSql = sql;
    m_sql = sql;
    m_isFirstSelect = true;
    getTableName();
}

/**
 * @brief CustomTableViewModel::setInsertSql
 * @param sql
 * 设置新增语句
 */
void CustomTableViewModel::setInsertSql(QString sql)
{
    m_insertSql = sql;
}

/**
 * @brief CustomTableViewModel::setShowCount
 * @param showCount
 * 设置显示条数
 */
void CustomTableViewModel::setShowCount(int showCount)
{
    m_isFirstSelect = true;
    m_paging->setShowCount(showCount);
}

/**
 * @brief CustomTableViewModel::setThisPage
 * @param thisPage
 * 设置当前页，跳转
 */
void CustomTableViewModel::setThisPage(int thisPage)
{
    QString limit = m_paging->setThisPage(thisPage);
    m_sql = m_sql.split("LIMIT",QString::KeepEmptyParts,Qt::CaseInsensitive).at(0);
    m_sql.append(" " + limit);
}

/**
 * @brief CustomTableViewModel::select
 * 查询
 * 1.关于这个查询，传递过来的sql必须得有 as ，并且 别名为字段名（与数据库一致）
 * 2.必须给这个类设置 disHeaderData
 */
void CustomTableViewModel::select(QVariantList values,QList<int> likeIndexs)
{
    //清除数据
    m_data.clear();
    m_headerData.clear();

    QList<QSqlRecord> dataCount; //获取数据条数的临时容器
    QList<QSqlRecord> records;

    //查询
    if(likeIndexs.size() > 0)
    {
        if(m_isFirstSelect && m_isPaging)
        {
            dataCount = m_speedDao->selectSqlRecord(m_initSql,values,likeIndexs);
            m_isFirstSelect = false;
            emit pageCountChanged(m_paging->alculatePageCount(dataCount.size()));
        }
        records = m_speedDao->selectSqlRecord(m_sql,values,likeIndexs);
    }
    else
    {
        if(m_isFirstSelect && m_isPaging)
        {
            dataCount = m_speedDao->selectSqlRecord(m_initSql,values);
            m_isFirstSelect = false;
            emit pageCountChanged(m_paging->alculatePageCount(dataCount.size()));
        }
        records = m_speedDao->selectSqlRecord(m_sql,values);
    }

    //添加数据
    for(int i = 0; i < records.size(); i++)
    {
        QSqlRecord item = records.at(i);

        //一项
        QVariantList dataItem;
        QVariantList dataItem2;

        for(int j = 0; j < item.count(); j++)
        {
            //添加表头数据(m_headerData),与数据库字段一致
            if(i == 0)
            {
                m_headerData.append(item.fieldName(j));
                if(m_disHeaderData.size() <= 0)
                {
                    m_disHeaderData.append(item.fieldName(j));
                }
            }

            //添加model数据
            QVariant value = item.value(j);

            dataItem.append(value);
            dataItem2.append(QVariant());
        }

        m_data.append(dataItem);
        m_userData.append(dataItem2);
    }


}

/**
 * @brief CustomTableViewModel::setEditable
 * @param value
 * 设置是否可编辑,true 可编辑，false不可编辑
 */
void CustomTableViewModel::setEditable(bool value)
{
    m_isEditable = value;
}

/**
 * @brief CustomTableViewModel::setOpenPaging
 * @param value
 * 设置是否分页
 */
void CustomTableViewModel::setOpenPaging(bool value)
{
    m_isPaging = value;
}




/*------------------私有方法-----------------------*/

/**
 * @brief CustomTableViewModel::getTableName
 * 获取当前查询sql的表格名称
 */
void CustomTableViewModel::getTableName()
{
    //QRegularExpression::CaseInsensitiveOption 不区分大小写
    QRegularExpression re("(?<=FROM\\s)\\S+",QRegularExpression::CaseInsensitiveOption);

    //QRegularExpression::PartialPreferFirstMatch 匹配到第一个立即返回
    QRegularExpressionMatch match = re.match(m_initSql,0,QRegularExpression::PartialPreferFirstMatch);

    m_tableName = match.captured();
}


/**
 * @brief CustomTableViewModel::openSqlTransaction
 * 打开事务
 */
bool CustomTableViewModel::openSqlTransaction()
{
    if(m_isTransaction)
    {
        return true;
    }
    m_isTransaction = m_speedDao->beginTransaction();
    return m_isTransaction;
}

/**
 * @brief CustomTableViewModel::submitAll
 * @return
 * 提交
 */
bool CustomTableViewModel::submitAll()
{
    if(m_isTransaction)
    {
        m_isInsert = false;
        m_isTransaction = false;
        return m_speedDao->endTransaction();
    }
    else
    {
        qDebug() << "您当前还没有开启事务呢！" << endl;
        return false;
    }
}

/**
 * @brief CustomTableViewModel::backAll
 * @return
 * 回撤
 */
bool CustomTableViewModel::backAll()
{
    if(m_isTransaction)
    {
        m_isInsert = false;
        m_isTransaction = false;
        return m_speedDao->rollback();
    }
    else
    {
        qDebug() << "您当前还没有开启事务呢！" << endl;
        return false;
    }
}

/*------------------继承重写方法-----------------------*/

/**
 * @brief CustomTableViewModel::rowCount
 * @param parent
 * @return 总行数
 * 返回总行数
 */
int CustomTableViewModel::rowCount(const QModelIndex &) const
{
    return m_data.size();
}

/**
 * @brief CustomTableViewModel::columnCount
 * @param parent
 * @return 总列数
 * 返回总列数
 */
int CustomTableViewModel::columnCount(const QModelIndex &) const
{
    return m_headerData.size();
}

/**
 * @brief CustomTableViewModel::data
 * @param index     索引
 * @param role      角色
 * @return          数据
 * 获取数据/数据格式
 */
QVariant CustomTableViewModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    int row = index.row();
    int col = index.column();

    if(role == Qt::DisplayRole)
    {
        return m_data.at(row).at(col);
    }
    else if(role == Qt::TextAlignmentRole)
    {
        return Qt::AlignCenter;
    }
    else if(role == Qt::UserRole)
    {
        return m_userData.at(row).at(col);
    }

    return QVariant();
}

/**
 * @brief CustomTableViewModel::headerData
 * @param section
 * @param orientation
 * @param role
 * @return
 * 获取/显示表头数据
 */
QVariant CustomTableViewModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section < 0 || section >= columnCount())
        {
            return QVariant();
        }

        if(!m_disHeaderData.isEmpty())
        {
            return m_disHeaderData.at(section);
        }

        return m_headerData.at(section);
    }

    return QVariant();
}

/**
 * @brief CustomTableViewModel::setHeaderData
 * @param section           列
 * @param orientation       水平/垂直
 * @param value             值
 * @param role              角色
 * @return
 * 设置表头数据
 */
bool CustomTableViewModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    bool flag = false;

    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section < 0 || section >= columnCount())
        {
            flag = false;
        }
        else
        {
            m_headerData[section] = value.toString();
            flag = true;
        }
    }

    return flag;
}

/**
 * @brief CustomTableViewModel::insertRows
 * @param row       从第几行开始
 * @param count     插入count行
 * @param parent    父节点
 * @return
 */
bool CustomTableViewModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent,row,row+count-1);

    for(int j = 0; j < count; j++)
    {
        QVariantList value;
        for(int i = 0; i < columnCount();i++)
        {
            value.append(QVariant());
        }
        m_data.append(value);
    }

    endInsertRows();

    //当前为新增状态
    m_isInsert = true;

    //清空新增容器
    m_updateData.clear();

    return true;
}

/**
 * @brief CustomTableViewModel::setData
 * @param index         索引
 * @param value         值
 * @param role          角色
 * @return
 * 设置数据
 */
bool CustomTableViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
    {
        return false;
    }

    int row = index.row();
    int col = index.column();

    if(value == m_data[row][col])
    {
        return true;
    }

    if(role == Qt::EditRole)
    {
        m_data[row][col] = value;
    }
    else if(role == Qt::UserRole)
    {
        //获取当前列名
        QString field = m_headerData.at(col);

        //新增
        if(m_isInsert)
        {
            if(m_updateData.size() > 0)
            {
                //添加至容器
                for(int i = 0; i < m_updateData.size(); i++)
                {
                    QVariantMap map =  m_updateData[i];
                    //存在
                    QMap<QString,QVariant>::iterator ite = map.find(field);

                    if(ite != map.end())
                    {
                        ite->setValue(value);
                        break;
                    }
                    else
                    {
                        QVariantMap map1;
                        //不存在
                        map1.insert(field,value);
                        m_updateData.append(map1);
                        break;
                    }
                }

                //排序,新增
                if(m_headerData.size()-1 == m_updateData.size())
                {
                    QVariantList myvalue;

                    //排序
                    for(int i = 0; i < m_headerData.size(); i++)
                    {
                        for(int j = 0; j < m_headerData.size()-1; j++)
                        {
                            QMap<QString,QVariant>::iterator ite = m_updateData[j].find(m_headerData[i]);
                            if(ite != m_updateData[j].end())
                            {
                                myvalue.append(ite.value());
                                break;
                            }
                        }
                    }

                    //新增
                    if(!m_speedDao->insertSql(m_insertSql,myvalue))
                    {
                        emit sqlError("新增发生错误！" + m_speedDao->getLastError());
                        backAll();
                        return false;
                    }
                    else
                    {
                        m_isFirstSelect = true;
                        m_updateData.clear();
                    }
                }
            }
            else
            {
                //不存在
                QVariantMap map;
                map.insert(field,value);
                m_updateData.append(map);
            }

        }
        else
        {
            //修改 1.修改的表 2.修改的字段 3.表主键
            QString updateSql = QString(" UPDATE %1 SET %2 = ? WHERE %3 = ? ").arg(m_tableName)
                    .arg(field).arg(m_headerData.at(0));

            QVariantList values ;
            values << value << index.sibling(index.row(),0).data();

            if(!m_speedDao->updateToModel(updateSql,values))
            {
                qDebug() << m_speedDao->getLastError();
                emit sqlError("修改发生错误！");
                backAll();
                return false;
            }

        }

        m_userData[row][col] = value;
    }

    return true;
}

/**
 * @brief CustomTableViewModel::flags
 * @param index
 * @return
 * 每一项的flages,是否可编辑,选中等
 */
Qt::ItemFlags CustomTableViewModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return QAbstractItemModel::flags(index);
    }

    if(!m_isEditable)
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    if(m_isInsert && index.row() == rowCount() - 1)
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }
    else if(m_isInsert && index.row() != rowCount() - 1)
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }
    else
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
    }
}


