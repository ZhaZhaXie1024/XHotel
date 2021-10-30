#ifndef USERCALENDARDELEGATE_H
#define USERCALENDARDELEGATE_H

#include <QItemDelegate>

class UserCalendarDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit UserCalendarDelegate(QObject *parent = nullptr);

    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

#endif // USERCALENDARDELEGATE_H
