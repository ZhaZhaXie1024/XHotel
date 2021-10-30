#include "pagingclass.h"

#include <QDebug>

PagingClass::PagingClass(QObject *parent) : QObject(parent)
{
    m_dataCount = 0;
    m_pageCount = 0;
    m_thisPage = 1;
    m_showCount = 0;

    //更新总页数
    connect(this,&PagingClass::updatePageCount,this,&PagingClass::setPageCount);
}

/**
 * @brief PagingClass::setDataCount
 * @param count
 * 设置数据总数
 */
void PagingClass::setDataCount(int count)
{
    m_dataCount = count;
    emit updatePageCount();
}

/**
 * @brief PagingClass::setShowCount
 * @param showCount
 * 设置显示数量
 */
void PagingClass::setShowCount(int showCount)
{
    m_showCount = showCount;
    emit updatePageCount();
}

/**
 * @brief PagingClass::setThisPage
 * @param thisPage
 * 设置当前页
 */
bool PagingClass::setThisPage(int thisPage)
{
    if(thisPage > 0 && thisPage <= m_pageCount)
    {
        m_thisPage = thisPage;

        emit tableUpdate((m_thisPage-1) * m_showCount,m_showCount);

        return true;
    }
    else
    {
        emit pageNumInvalid("请输入正确的页码！");
        return false;
    }

    return false;
}

/**
 * @brief PagingClass::setPageCount
 * @param pageCount
 * 设置总页数
 */
void PagingClass::setPageCount()
{
    if(m_dataCount > 0 && m_showCount > 0)
    {
        int page = m_dataCount / m_showCount;

        if(m_dataCount % m_showCount > 0)
        {
            page++;
        }

        m_pageCount = page;

        emit updataUiPageCount(m_pageCount);
    }
}
