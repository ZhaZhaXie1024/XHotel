#include "staffservice.h"


#include <QDebug>
#include <QDate>

#include "Dao/speedydao.h"
#include "Custom/customtablemodel.h"
#include "Entitys/pagingclass.h"
#include "Service/comboboxitemservice.h"
#include "Entitys/comboboxentity.h"

StaffService::StaffService(QObject *parent) : QObject(parent)
{
    m_speedyDao = new SpeedyDao(this);
    m_model = new CustomTableModel(this);
    m_paging = new PagingClass(this);
    m_comboBoxService = new ComboboxItemService(this);
    m_tableName = "staff";
    m_col = 8;
    m_sql = "";
    m_index = 0;

    initModel();

    connect(m_paging,&PagingClass::tableUpdate,this,&StaffService::pageChanged);
    connect(m_paging,&PagingClass::updataUiPageCount,this,&StaffService::onPageCount);
    connect(m_paging,&PagingClass::pageNumInvalid,this,&StaffService::onPageError);
    connect(m_model,&CustomTableModel::updateToService,this,&StaffService::onModelUpdateAppendSql);
}

/**
 * @brief StaffService::initModel
 * 功能：初始化model
 */
void StaffService::initModel()
{
    //拼接SQL
    QStringList keys;
    keys << "staff.staff_id" << "staff.name AS 姓名" << "staff.sex AS 性别"
         << "department.m_describe AS 部门" << "staff_position.m_describe AS 职位"
         << "staff.phone AS 电话"
         << "staff.id_card AS 身份证" << "staff.entry_date AS 何时入职";


    QString select = m_speedyDao->joinSelectSQL(m_tableName,keys);

    QStringList leftJoinDepKeys;
    leftJoinDepKeys << "department_id";
    QString leftJoinDep = m_speedyDao->joinLeftJoinSQL(m_tableName,"department",leftJoinDepKeys);

    QStringList leftJoinPosKeys;
    leftJoinPosKeys << "staff_position_id";
    QString leftJoinPos = m_speedyDao->joinLeftJoinSQL(m_tableName,"staff_position",leftJoinPosKeys);

    QString OrderBySql = m_speedyDao->joinOrderSQL(QStringList() << m_tableName,
                                                   QStringList() << "staff_id",
                                                   QStringList() << "DESC");

    select.append(leftJoinDep);
    select.append(leftJoinPos);
    select.append(OrderBySql);

    m_model->setSql(select);

    m_sql = select;
}

/**
 * @brief StaffService::select
 * 功能：刷新/查询
 */
void StaffService::select()
{
    m_model->select();
    m_paging->setDataCount(selectTableCount());
}

/**
 * @brief StaffService::setLimit
 * @param index
 * @param showCount
 * 设置limit
 */
void StaffService::setLimit(int index, int showCount)
{
    m_paging->setShowCount(showCount);
    QString sql = m_speedyDao->joinLimit(index,showCount);
    QString select = m_sql;
    select.append(sql);

    m_model->setSql(select);
}

/**
 * @brief StaffService::pageChanged
 * @param index
 * @param showCount
 * 刷新数据
 */
void StaffService::pageChanged(int index, int showCount)
{
    setLimit(index,showCount);
    select();
}

/**
 * @brief StaffService::setPage
 * @param page
 * 修改页数，跟新数据
 */
bool StaffService::setPage(int page)
{
    return m_paging->setThisPage(page);
}

/**
 * @brief StaffService::insertData
 * @param staffName
 * @param phone
 * @param idCard
 * @param sex
 * @param depID
 * @param posID
 * 新增
 */
bool StaffService::insertData(QString staffName, QString phone, QString idCard, QString sex, int depID, int posID,QDate date)
{
    QStringList key;
    key << "staff_position_id" << "department_id" << "sex" <<
           "name" << "phone" << "id_card" << "entry_date";
    //拼接SQL
    QString sql = m_speedyDao->joinInsertSQL(m_tableName,key);

    QVariantList values;
    values << posID << depID << sex
           << staffName << phone << idCard << date;

    return m_speedyDao->insertSql(sql,values);
}

/**
 * @brief StaffService::updateData
 * @param staffID
 * @param staffName
 * @param phone
 * @param idCard
 * @param sex
 * @param depID
 * @param posID
 * @param date
 * @return
 * 修改
 */
bool StaffService::updateData(int staffID, QString staffName, QString phone, QString idCard, QString sex, int depID, int posID,QDate date)
{
    QStringList key;

    key << "staff_position_id" << "department_id" << "sex" <<
           "name" << "phone" << "id_card" << "entry_date";

    QString sql = m_speedyDao->joinUpdateSQL(m_tableName,key);

    QStringList whereKey;

    QString where = m_speedyDao->joinWhereSQL(QStringList() << m_tableName,
                                              QStringList() << "staff_id",
                                              QList<SQLLinkSymbol>() << SQLLinkSymbol::LinkEqual);

    sql.append(where);

    QVariantList values;
    values << posID << depID << sex
           << staffName << phone << idCard << date << staffID;

    return m_speedyDao->updateSql(sql,values);
}

/**
 * @brief StaffService::deleteData
 * @param staffID
 * @return
 * 删除
 */
bool StaffService::deleteData(int staffID)
{
    QString sql = m_speedyDao->joinDeleteSQL(m_tableName);
    sql.append(m_speedyDao->joinWhereSQL(QStringList() << m_tableName,
                                         QStringList() << "staff_id",
                                         QList<SQLLinkSymbol>() << SQLLinkSymbol::LinkEqual));

    return m_speedyDao->deleteSql(sql,QVariantList() << staffID);
}

/**
 * @brief StaffService::getLastError
 * @return 最后的错误
 * 获取最后的错误
 */
QString StaffService::getLastError()
{
    return m_speedyDao->getLastError();
}

/**
 * @brief StaffService::commiteData
 * 提交修改的数据
 * @return 返回修改成功的条数
 */
int StaffService::commiteData()
{
    QMap<int,int>::const_iterator ite = m_updateMap.begin();

    int i = 0;

    for(;ite != m_updateMap.end(); ite++)
    {
        int id = ite.key();

        int index = ite.value();

        if(m_updateField.at(index).size() != m_updateValues.at(index).size())
        {
            //发生错误,进行回滚
            backRoll();
        }

        QString sql = m_speedyDao->joinUpdateSQL(m_tableName,m_updateField.at(index));
        QString where = m_speedyDao->joinWhereSQL(QStringList() << m_tableName,
                                                  QStringList() << "staff_id",
                                                  QList<SQLLinkSymbol>() << LinkEqual);

        sql.append(where);

        //拼接values
        QVariantList values = m_updateValues.at(index);
        values.append(id);

//        qDebug() << sql << endl;
//        qDebug() << values << endl;

        if(m_speedyDao->updateSql(sql,values))
        {
            i++;
        }
        else
        {
            qDebug() << "StaffService::commiteData(): " << getLastError();
        }
    }

    return i;
}

/**
 * @brief StaffService::backRoll
 * 回滚
 *
 */
void StaffService::backRoll()
{
    //清空修改容器
    m_updateMap.clear();
    m_updateField.clear();
    m_updateValues.clear();

    //刷新数据
    select();
}

/**
 * @brief StaffService::fieldName
 * @param chineseFieldName
 * @return
 * 返回正确的字段名称
 */
QString StaffService::fieldName(QString chineseFieldName)
{
    if(chineseFieldName.trimmed().compare("姓名") == 0)
    {
        return "staff.name";
    }
    else if(chineseFieldName.trimmed().compare("性别") == 0)
    {
       return "staff.sex";
    }
    else if(chineseFieldName.trimmed().compare("部门") == 0)
    {
        return "staff.department_id";
    }
    else if(chineseFieldName.trimmed().compare("职位") == 0)
    {
        return "staff.staff_position_id";
    }
    else if(chineseFieldName.trimmed().compare("电话") == 0)
    {
        return "staff.phone";
    }
    else if(chineseFieldName.trimmed().compare("身份证") == 0)
    {
        return "staff.id_card";
    }
    else if(chineseFieldName.trimmed().compare("何时入职") == 0)
    {
        return "staff.entry_date";
    }

    return chineseFieldName;
}

/**
 * @brief StaffService::comboBoxData
 * @param tableName
 * @param field
 * @return
 * 返回下拉框数据
 */
QList<ComboBoxSeleteItem> StaffService::comboBoxData(QString tableName, QStringList field)
{
    m_comboBoxService->init(tableName,field);
    return m_comboBoxService->select();
}

/**
 * @brief StaffService::onPageCount
 * @param num
 * 转发总页数
 */
void StaffService::onPageCount(int num)
{
    if(this->getRow() <= 0)
    {
        emit updataUiPageCount(0);
    }
    else
    {
        emit updataUiPageCount(num);
    }
}

/**
 * @brief StaffService::onPageError
 * @param str
 * 转发pageErrorStr
 */
void StaffService::onPageError(QString str)
{
    emit pageError(str);
}

/**
 * @brief StaffService::onModelUpdateAppendSql
 * @param id
 * @param field
 * @param value
 * 接收来自 CustomTableModel 修改信号
 */
void StaffService::onModelUpdateAppendSql(int id, QString field, QVariant value)
{
    //判断map中是否存在该id
    if(m_updateMap.contains(id))
    {

        //存在追加
        int index = m_updateMap.value(id);
        QList<QString> fields = m_updateField.at(index);

        QString fieldMyName = fieldName(field);

        if(!fields.contains(fieldMyName))
        {
            m_updateField[index].append(fieldMyName);
            m_updateValues[index].append(value);
        }
        else
        {
            int repIndex = fields.indexOf(fieldMyName);
            (m_updateField[index])[repIndex] = fieldMyName;
            (m_updateValues[index])[repIndex] = value;
        }
    }
    else
    {
        //不存在，添加
        m_updateMap.insert(id,m_index);

        QList<QString> fields;
        fields.append(fieldName(field));
        m_updateField.append(fields);

        QVariantList values;
        values.append(value);
        m_updateValues.append(values);

        m_index++;
    }
}

/**
 * @brief StaffService::getModel
 * @return m_model
 * 功能：获取model
 */
QAbstractItemModel *StaffService::getModel()
{
    if(m_model != nullptr)
    {
        return m_model;
    }

    return nullptr;
}

/**
 * @brief StaffService::getRow
 * @return 行
 */
int StaffService::getRow()
{
    return m_model->getRow();
}

/**
 * @brief StaffService::getCol
 * @return 列
 */
int StaffService::getCol()
{
    if(m_model->getCol() != m_col && m_model->getCol() > 0)
    {
        return m_model->getCol();
    }
    return m_col;
}

/**
 * @brief StaffService::selectTableCount
 * @return 获取全部条数
 */
int StaffService::selectTableCount()
{
    QString sql = QString("Select count(*) from %1 ").arg(m_tableName);

    QStringList leftJoinDepKeys;
    leftJoinDepKeys << "department_id";
    QString leftJoinDep = m_speedyDao->joinLeftJoinSQL(m_tableName,"department",leftJoinDepKeys);

    QStringList leftJoinPosKeys;
    leftJoinPosKeys << "staff_position_id";
    QString leftJoinPos = m_speedyDao->joinLeftJoinSQL(m_tableName,"staff_position",leftJoinPosKeys);

    sql.append(leftJoinDep);
    sql.append(leftJoinPos);

    return m_speedyDao->selectTableCount(sql,QVariantList());
}


