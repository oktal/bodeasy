#ifndef SESSIONICONDELEGATE_H
#define SESSIONICONDELEGATE_H

#include <QStyledItemDelegate>

class SessionIconView;

class SessionIconDelegate : public QStyledItemDelegate
{
	Q_OBJECT
	
public:
	SessionIconDelegate( SessionIconView* parent = 0 );
	virtual ~SessionIconDelegate();
	
	virtual QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
	virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
	virtual QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const;

protected:
	SessionIconView* mView;
};

#endif // SESSIONICONDELEGATE_H
