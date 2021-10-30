#ifndef USERCOMBOBOXDELETAGE_H
#define USERCOMBOBOXDELETAGE_H

#include <QItemDelegate>
#include "Entitys/comboboxentity.h"

class ComboboxItemService;

class UserComboBoxDeletage : public QItemDelegate
{
    Q_OBJECT
public:
    explicit UserComboBoxDeletage(QList<ComboBoxSeleteItem> data,QObject *parent = nullptr);

    UserComboBoxDeletage(QList<QVariantMap> data,QObject *parent = nullptr);

private:
    //service
    ComboboxItemService *m_service;

    //数据
    QList<ComboBoxSeleteItem> m_data;
    QList<QVariantMap> m_mapData;


    // QAbstractItemDelegate interface
public:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:
    void updateRelevanceComboBox() const;
};

#endif // USERCOMBOBOXDELETAGE_H
