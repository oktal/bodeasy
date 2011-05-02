#include "ruleritem.h"

#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

#include <QLineEdit>

RulerItem::RulerItem(Qt::Orientation orientation, QGraphicsItem *parent) :
    QGraphicsItem(parent),
    mWidth(0), mOrientation(orientation),
    mLineField(0)
{
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptsHoverEvents(true);
}

void RulerItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    if (mLineField)
    {
        mLineField->setStyleSheet("border: 2px solid rgb(255, 66, 66);");
        setCursor(Qt::PointingHandCursor);
    }
}

void RulerItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event)
    if (mLineField)
    {
        mLineField->setStyleSheet("border: 1px solid black;");
        setCursor(Qt::ArrowCursor);
    }
}

void RulerItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (mLineField && event->button() == Qt::LeftButton)
    {
        mLineField->setFocus(Qt::OtherFocusReason);
    }
}

Qt::Orientation RulerItem::orientation() const
{
    return mOrientation;
}

void RulerItem::setWidth(int width)
{
    mWidth = width;
    update();
}

void RulerItem::setBuddy(QLineEdit *field)
{
    mLineField = field;
}

QRectF RulerItem::boundingRect() const
{
    return mOrientation == Qt::Horizontal ? QRectF(0, 0, mWidth, Height) :
                                            QRectF(0, 0, Height, mWidth);
}

int RulerItem::type() const
{
    return Type;
}

void RulerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);
    painter->setBrush(QColor(255, 247, 83));
    if (mOrientation == Qt::Horizontal)
    {
        painter->drawRect(QRect(0, 0, mWidth, Height));
    }
    else
    {
        painter->drawRect(QRect(0, 0, Height, mWidth));
    }
    qreal const Step = mWidth / Separators;
    qreal const MiniStep = Step / 4;
    qreal x = 0.0;
    qreal minix = 0.0;
    for (int i = 0; i < Separators; ++i)
    {
        x += Step;
        if (mOrientation == Qt::Horizontal)
        {
            painter->drawLine(QPointF(x, 3), QPointF(x, Height));
        }
        else
        {
            painter->drawLine(QPointF(3, x), QPointF(Height, x));
        }
        for (int j = 0; j < 4; ++j)
        {
            minix += MiniStep;
            if (mOrientation == Qt::Horizontal)
            {
                painter->drawLine(QPointF(minix, 5), QPointF(minix, Height));
            }
            else
            {
                painter->drawLine(QPointF(5, minix), QPointF(Height, minix));
            }
        }
    }
}
