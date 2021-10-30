#ifndef PAGINGCLASS_H
#define PAGINGCLASS_H

#include <QObject>

/**
 * @brief The PagingClass class
 * 分页，计算分页,只是用在 员工管理这里
 * 因为这里的逻辑太过于混乱，没有达到，完整封装的意义
 */
class PagingClass : public QObject
{
    Q_OBJECT
public:
    explicit PagingClass(QObject *parent = nullptr);

    //设置数据总数
    void setDataCount(int count);

    //设置显示数量
    void setShowCount(int showCount);

    //设置当前页
    bool setThisPage(int thisPage);

public slots:
    //设置总页数
    void setPageCount();

private:
    int m_dataCount;    //数据总数

    int m_pageCount;    //总页数

    int m_thisPage;     //当前页

    int m_showCount;    //显示几条

signals:
    //更新总页数
    void updatePageCount();

    //通知页面更新总页数
    void updataUiPageCount(int pageCount);

    //通知更新信号
    void tableUpdate(int index,int showCount);

    //通知页数无效信号
    void pageNumInvalid(QString errorStr);
};

#endif // PAGINGCLASS_H
