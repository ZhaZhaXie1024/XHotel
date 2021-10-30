#include "spinboxdelegate.h"

#include <QPainter>

SpinBoxDelegate::SpinBoxDelegate(QObject *parent) : QItemDelegate(parent)
{

}

void SpinBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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
        QItemDelegate::paint(painter,option,index);
    }
}

QWidget *SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &) const
{
    QSpinBox *spinbox = new QSpinBox(parent);
    spinbox->setMaximum(100);
    spinbox->setMinimum(1);
    return spinbox;
}

void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QSpinBox* spinbox = static_cast<QSpinBox*>(editor);
    spinbox->setValue(index.data().toInt());
}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox* spinbox = static_cast<QSpinBox*>(editor);

    int value = spinbox->value();

    if(model->setData(index,value,Qt::UserRole))
    {
        model->setData(index,value);
    }
}
