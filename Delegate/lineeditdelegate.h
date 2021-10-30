#ifndef LINEEDITDELEGATE_H
#define LINEEDITDELEGATE_H

#include <QItemDelegate>

#include <QLineEdit>

class LineEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit LineEditDelegate(QObject *parent = nullptr);




    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};

#endif // LINEEDITDELEGATE_H
