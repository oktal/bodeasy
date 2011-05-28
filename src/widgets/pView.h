#ifndef PVIEW_H
#define PVIEW_H

#include <QListView>
#include <QTreeView>
#include <QTableView>
#include <QColumnView>

class pHeaderView;

class pView
{
public:
	pView( QAbstractItemView* view );
	
	bool hasProportionalSectionSizes( Qt::Orientation orientation ) const;
	void setProportionalSectionSizes( Qt::Orientation orientation, bool enabled );
	
	int sectionStretchFactor( Qt::Orientation orientation, int logicalIndex ) const;
	void setSectionStretchFactor( Qt::Orientation orientation, int logicalIndex, int factor );

protected:
	pHeaderView* horizontal;
	pHeaderView* vertical;
	
	void init( QAbstractItemView* view );
};

class pListView : public QListView, public pView
{
	Q_OBJECT

public:
	pListView( QWidget* parent = 0 ) : QListView( parent ), pView( this ) {}
};

class pTreeView : public QTreeView, public pView
{
	Q_OBJECT

public:
	pTreeView( QWidget* parent = 0 ) : QTreeView( parent ), pView( this ) {}
};

class pTableView : public QTableView, public pView
{
	Q_OBJECT

public:
	pTableView( QWidget* parent = 0 ) : QTableView( parent ), pView( this ) {}
};

#endif // PVIEW_H
