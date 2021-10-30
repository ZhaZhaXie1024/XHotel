#include "customtablemodel.h"

#include <QDebug>
#include <QSqlRecord>

#include "Dao/speedydao.h"

CustomTableModel::CustomTableModel(QObject *parent) : QAbstractTableModel(parent)
{
    m_dao = new SpeedyDao(this);
    m_header.clear();
    m_data = QVariantList();

    m_sql = "";
    m_values = QVariantList();
}

/**
 * @brief CustomTableModel::setTableName
 * @param tableName
 * 通过表名进行查询
 */
void CustomTableModel::setTableName(QString tableName)
{
    QString sql = m_dao->joinSelectSQL(tableName);
    m_sql = sql;
    m_values = QVariantList();
}

/**
 * @brief CustomTableModel::setTableName
 * @param sql
 * @param values
 * 通过sql语句进行查询
 */
void CustomTableModel::setSql(QString sql, QVariantList values)
{
    m_sql = sql;
    m_values = values;
}

/**
 * @brief CustomTableModel::select
 * 真正意义上的查询，把查询到的数据添加到 m_data 中
 */
void CustomTableModel::select()
{
    QList<QSqlRecord> source = m_dao->selectSqlRecord(m_sql,m_values);

    //行
    int row = source.size();

    if(row <= 0)
    {
        qDebug() << "CustomTableModel::select() row 数据异常" << __LINE__ << endl;
        return;
    }

    //列
    int col = source.at(0).count();

    m_header.clear();
    m_data.clear();

    //开始赋值
    for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            if(i == 0)
            {
                m_header.append(source.at(i).fieldName(j));
            }

            m_data.append(source.at(i).value(j));

            if(j == 0)
            {
                //添加id进入userRoleData
                m_userRoleData.insert(m_data.size()-1,QVariant());
            }
        }
    }
}

/**
 * @brief CustomTableModel::getRow
 * @return
 * 获取行数
 */
int CustomTableModel::getRow()
{
    return rowCount();
}

/**
 * @brief CustomTableModel::getCol
 * @return
 * 获取列数
 */
int CustomTableModel::getCol()
{
    return columnCount();
}

/**
 * @brief CustomTableModel::rowCount
 * @param parent
 * @return 行数
 */
int CustomTableModel::rowCount(const QModelIndex &) const
{
    if(m_data.size() != 0 && m_header.size() != 0)
    {
        return m_data.size() / m_header.size();
    }

    return 0;
}

/**
 * @brief CustomTableModel::columnCount
 * @param parent
 * @return 列数
 */
int CustomTableModel::columnCount(const QModelIndex &) const
{
    return m_header.size();
}

/**
 * @brief CustomTableModel::data
 * @param index
 * @param role
 * @return
 * 设置表格数据/格式,根据角色(role)对数据或item的样式进行修改
 */
QVariant CustomTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }

    int row = index.row();
    int col = index.column();

    if(role == Qt::DisplayRole)
    {
        return m_data.at(row * columnCount() + col);
    }
    else if(role == Qt::TextAlignmentRole)
    {
        return Qt::AlignCenter;
    }
    else if(role == Qt::UserRole)
    {
        return m_userRoleData.value(row * columnCount() + col);
    }

    return QVariant();
}

/**
 * @brief CustomTableModel::setData
 * @param index
 * @param value
 * @param role
 * @return
 * 修改model中使用
 */
bool CustomTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
    {
        return false;
    }

    int row = index.row();
    int col = index.column();

    if(value.type() == QVariant::String)
    {

        QString  str = value.toString();

        if(str.isEmpty())
        {
            return false;
        }
    }

    if(role == Qt::UserRole)
    {
        m_userRoleData[row * columnCount() + col] = value;
    }

    if(role == Qt::EditRole)
    {
        QString header = headerData(index.column(),Qt::Horizontal).toString();

        //获取当前行的ID
        int id = m_data.at(row * columnCount()).toInt();

        //获取当前项的userrole的data
        QVariant userValue = m_userRoleData[row * columnCount() + col];

        //修改model数据
        m_data[row * columnCount() + col] = value;

        if(!userValue.isNull())
        {
            //发送一个修改信号至service
            emit updateToService(id,header,userValue);
        }
        else
        {
            emit updateToService(id,header,value);
        }

        return true;
    }



    return false;
}

/**
 * @brief CustomTableModel::flags
 * @param index
 * @return
 * 设置item的flags
 */
Qt::ItemFlags CustomTableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return QAbstractItemModel::flags(index);
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

/**
 * @brief CustomTableModel::headerData
 * @param section
 * @param orientation
 * @param role
 * @return 表头数据
 */
QVariant CustomTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if(section < 0 || section >= columnCount())
        {
            return QVariant();
        }
        return m_header.at(section);
    }

    return QVariant();
}

