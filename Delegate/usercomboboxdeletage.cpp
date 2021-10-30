#include "usercomboboxdeletage.h"

#include "Service/comboboxitemservice.h"
#include <QComboBox>
#include <QPainter>
#include <QDebug>

UserComboBoxDeletage::UserComboBoxDeletage(QList<ComboBoxSeleteItem> data,QObject *parent)
    : QItemDelegate(parent),
      m_data(data)
{
    m_service = new ComboboxItemService(this);
}

UserComboBoxDeletage::UserComboBoxDeletage(QList<QVariantMap> data, QObject *parent)
    : QItemDelegate(parent),
      m_mapData(data)
{
    m_service = new ComboboxItemService(this);
}

/**
 * @brief UserComboBoxDeletage::createEditor
 * @param parent    父控件
 * @param option    样式
 * @param index     当前索引
 * @return QComboBox
 * 创建控件
 */
QWidget *UserComboBoxDeletage::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QComboBox *comboBox = new QComboBox(parent);

    if(m_data.size() <= 0 && m_mapData.size() > 0)
    {
        m_service->boundData(comboBox,m_mapData);
    }
    else if(m_mapData.size() <= 0 && m_data.size() > 0)
    {
        m_service->boundData(comboBox,m_data);
    }

    return comboBox;
}

/**
 * @brief UserComboBoxDeletage::setEditorData
 * @param editor
 * @param index
 * 选中其它选项，修改（控件改变数据）
 */
void UserComboBoxDeletage::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString str = index.data().toString();
    QComboBox *box = static_cast<QComboBox*>(editor);
    box->setCurrentText(str);
}

/**
 * @brief UserComboBoxDeletage::setModelData
 * @param editor
 * @param model
 * @param index
 * 改变项后，同时改变model数据
 */
void UserComboBoxDeletage::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *box = static_cast<QComboBox*>(editor);

    QString str = box->currentText();

    QVariant userRoleValue = static_cast<QComboBox*>(editor)->currentData();

    if(model->setData(index,userRoleValue,Qt::UserRole))
    {
        model->setData(index,str,Qt::EditRole);
    }
}

/**
 * @brief UserComboBoxDeletage::paint
 * @param painter
 * @param option
 * @param index
 * 绘制
 */
void UserComboBoxDeletage::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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
