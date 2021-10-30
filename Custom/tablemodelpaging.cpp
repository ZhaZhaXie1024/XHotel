#include "tablemodelpaging.h"

#include <QDebug>

TableModelPaging::TableModelPaging(QObject *parent) : QObject(parent)
{
    m_pageCount = 0;
    m_thisPage = 0;
    m_showCount = 15;
}

/**
 * @brief TableModelPaging::setShowCount
 * @param showCount
 * 设置显示条数
 */
void TableModelPaging::setShowCount(int showCount)
{
    m_showCount = showCount;
}

/**
 * @brief TableModelPaging::alculatePageCount
 * @param showCount  显示多少条
 * @param dataCount  数据总数
 * @return 0参数错误，大于0正确
 * 计算总页数
 */
int TableModelPaging::alculatePageCount(int dataCount)
{
    if(m_showCount == 0 || dataCount == 0)
    {
        return 0;
    }

    int page = dataCount / m_showCount;

//    qDebug() << "alculatePageCount " << m_showCount << dataCount << page << dataCount % m_showCount << endl;

    if(dataCount % m_showCount > 0)
    {
        page++;
    }

    m_pageCount = page;

    return m_pageCount;
}

/**
 * @brief TableModelPaging::setThisPage
 * @param thisPage 当前页
 * @return 返回SQL语句Limit部分
 * 通过当前页计算当前显示索引，从多少开始，显示多少
 */
QString TableModelPaging::setThisPage(int thisPage)
{
    int index = (thisPage-1) * m_showCount;

    QString limit = QString(" LIMIT %1,%2 ").arg(index).arg(m_showCount);

    return limit;
}

/**
 * @brief TableModelPaging::getThisPage
 * @return  当前页
 * 获取当前页
 */
int TableModelPaging::getThisPage()
{
    return m_thisPage;
}
