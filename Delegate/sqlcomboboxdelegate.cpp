#include "sqlcomboboxdelegate.h"

#include <QPainter>
#include <QDebug>

#include "Dao/speedydao.h"
#include "Service/comboboxitemservice.h"

SqlComboBoxDelegate::SqlComboBoxDelegate(QObject *parent) : QItemDelegate(parent)
{
    init();
}

SqlComboBoxDelegate::SqlComboBoxDelegate(QString sql,int relevanceIndex,QObject *parent)
    : QItemDelegate(parent),
      m_sql(sql),
      m_relevanceIndex(relevanceIndex)
{
    init();
}

void SqlComboBoxDelegate::init()
{
    m_speedyDao = new SpeedyDao(this);
    m_comService = new ComboboxItemService(this);
}

void SqlComboBoxDelegate::setSql(QString sql,int relevanceIndex)
{
    m_sql = sql;
    m_relevanceIndex = relevanceIndex;
}

void SqlComboBoxDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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

QWidget *SqlComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex & index) const
{
    QList<ComboBoxSeleteItem> data =
            m_comService->select(m_sql,QVariantList()<<index.sibling(index.row(),m_relevanceIndex).data(Qt::DisplayRole));

    QVariant dis = index.sibling(index.row(),m_relevanceIndex).data(Qt::DisplayRole);

    QComboBox *comboBox = new QComboBox(parent);

    m_comService->boundData(comboBox,data);

    return comboBox;
}

void SqlComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str = index.data().toString();
    QComboBox *box = static_cast<QComboBox*>(editor);

    if(str.trimmed().isEmpty())
    {
        box->setCurrentIndex(0);
    }
    else
    {
        box->setCurrentText(str);
    }
}

void SqlComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *box = static_cast<QComboBox*>(editor);

    QString str = box->currentText();

    QVariant userRoleValue = static_cast<QComboBox*>(editor)->currentData();

    model->setData(index,userRoleValue,Qt::UserRole);
    model->setData(index,str,Qt::EditRole);
}
