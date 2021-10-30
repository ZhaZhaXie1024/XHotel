#ifndef USERLINEEDITDELEGATE_H
#define USERLINEEDITDELEGATE_H

#include <QItemDelegate>

class UserLineEditDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    //设置输入字符长度范围
    UserLineEditDelegate(int length = 30,QObject *parent = nullptr);

    //设置正则表达式(验证器)
    UserLineEditDelegate(const QValidator *v,QObject *parent = nullptr);

    //设置输入字符长度范围，正则表达式(验证器)
    UserLineEditDelegate(int length,const QValidator *v,QObject *parent = nullptr);

private:
    //字符长度
    int m_length;
    //正则表达式(验证器)
    const QValidator *m_validator;

    // QAbstractItemDelegate interface
public:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // USERLINEEDITDELEGATE_H
