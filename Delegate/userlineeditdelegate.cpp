#include "userlineeditdelegate.h"

#include <QLineEdit>
#include <QPainter>

/**
 * @brief UserLineEditDelegate::UserLineEditDelegate
 * @param length 字符长度
 * @param parent
 */
UserLineEditDelegate::UserLineEditDelegate(int length, QObject *parent)
    : QItemDelegate(parent),
      m_length(length)
{

}

UserLineEditDelegate::UserLineEditDelegate(const QValidator *v, QObject *parent)
    : QItemDelegate(parent),
      m_validator(v)
{

}

UserLineEditDelegate::UserLineEditDelegate(int length, const QValidator *v, QObject *parent)
    : QItemDelegate(parent),
      m_length(length),
      m_validator(v)
{

}

QWidget *UserLineEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    QLineEdit *lineEdit = new QLineEdit(parent);
    lineEdit->setMaxLength(m_length);
    lineEdit->setValidator(m_validator);
    return lineEdit;
}

void UserLineEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(index.data().toString());
}

void UserLineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);

    QVariant data = lineEdit->text();

    model->setData(index,data);
}

void UserLineEditDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString data = index.data().toString();

    if(option.state & QStyle::State_Selected)
    {
        //保存旧画笔
        QPen oldPen = painter->pen();
        //画笔。绘制图形边线，由颜色、宽度、线风格等参数组成
        QPen pen;
        //将它设置为无颜色,不绘制边框了
        pen.setColor(QColor(255,255,255,0));

        //画刷。填充几何图形的调色板，由颜色和填充风格组成
        QBrush oldBrush = painter->brush();
        QBrush brush;
        brush.setColor(QColor(255,209,143,120));
        brush.setStyle(Qt::SolidPattern);

        //可在QPaintDevice上绘制各种图形。QPaintDevice有之类QWidget、QImage、QOpenGLPaintDevice等
        painter->setPen(pen);
        painter->setBrush(brush);
        painter->drawRect(option.rect);

        pen.setColor("#618BE5");
        painter->setPen(pen);
        painter->drawText(option.rect,Qt::AlignCenter,data);

        //还原旧画笔
        painter->setPen(oldPen);
        painter->setBrush(oldBrush);
    }
    else
    {
        painter->drawText(option.rect,Qt::AlignCenter,data);
    }
}


