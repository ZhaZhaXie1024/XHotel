#ifndef SQLCOMBOBOXDELEGATE_H
#define SQLCOMBOBOXDELEGATE_H

#include <QItemDelegate>

#include <QComboBox>

class SpeedyDao;
class ComboboxItemService;

class SqlComboBoxDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit SqlComboBoxDelegate(QObject *parent = nullptr);
    SqlComboBoxDelegate(QString sql,int relevanceIndex,QObject *parent = nullptr);

    void init();

    void setSql(QString sql,int relevanceIndex);

private:
    SpeedyDao *m_speedyDao;
    ComboboxItemService *m_comService;

    QString m_sql;
    int m_relevanceIndex;

    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

#endif // SQLCOMBOBOXDELEGATE_H
