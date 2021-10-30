#ifndef TABLEMODELPAGING_H
#define TABLEMODELPAGING_H

#include <QObject>

class TableModelPaging : public QObject
{
    Q_OBJECT
public:
    explicit TableModelPaging(QObject *parent = nullptr);

    //设置显示条数
    void setShowCount(int showCount);

    //计算总页数,并返回
    int alculatePageCount(int dataCount);

    //通过当前页计算当前显示索引，从多少开始，显示多少，返回SQL语句Limit部分
    QString setThisPage(int thisPage);

    //获取当前页
    int getThisPage();

private:
    //总页数
    int m_pageCount;

    //当前页
    int m_thisPage;

    //显示条数
    int m_showCount;

};

#endif // TABLEMODELPAGING_H
