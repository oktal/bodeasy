#ifndef RULERITEM_H
#define RULERITEM_H

#include <QGraphicsItem>

class QLineEdit;

class RulerItem : public QGraphicsItem
{
public:
    enum { Type = UserType + 1 };
    explicit RulerItem(Qt::Orientation orientation = Qt::Horizontal, QGraphicsItem *parent = 0);
    Qt::Orientation orientation() const;
    void setWidth(int width);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setBuddy(QLineEdit *field);
    int type() const;

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    static const int Height = 8;
    static const int Separators = 4;
    int mWidth;
    Qt::Orientation mOrientation;
    QLineEdit *mLineField;
};

#endif // RULERITEM_H
