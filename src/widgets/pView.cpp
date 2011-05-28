#include "pView.h"
#include "pHeaderView.h"

#include <QListView>
#include <QTreeView>
#include <QTableView>
#include <QColumnView>

pView::pView( QAbstractItemView* view )
{
	Q_ASSERT( view );
	init( view );
}

bool pView::hasProportionalSectionSizes( Qt::Orientation orientation ) const
{
	switch ( orientation ) {
		case Qt::Horizontal:
			return horizontal ? horizontal->hasProportionalSectionSizes() : false;
		case Qt::Vertical:
			return vertical ? vertical->hasProportionalSectionSizes() : false;
	}
}

void pView::setProportionalSectionSizes( Qt::Orientation orientation, bool enabled )
{
	switch ( orientation ) {
		case Qt::Horizontal: {
			if ( horizontal ) {
				horizontal->setProportionalSectionSizes( enabled );
			}
			
			break;
		}
		case Qt::Vertical: {
			if ( vertical ) {
				vertical->setProportionalSectionSizes( enabled );
			}
			
			break;
		}
	}
}

int pView::sectionStretchFactor( Qt::Orientation orientation, int logicalIndex ) const
{
	switch ( orientation ) {
		case Qt::Horizontal:
			return horizontal ? horizontal->sectionStretchFactor( logicalIndex ) : false;
		case Qt::Vertical:
			return vertical ? vertical->sectionStretchFactor( logicalIndex ) : false;
	}
}

void pView::setSectionStretchFactor( Qt::Orientation orientation, int logicalIndex, int factor )
{
	switch ( orientation ) {
		case Qt::Horizontal: {
			if ( horizontal ) {
				horizontal->setSectionStretchFactor( logicalIndex, factor );
			}
			
			break;
		}
		case Qt::Vertical: {
			if ( vertical ) {
				vertical->setSectionStretchFactor( logicalIndex, factor );
			}
			
			break;
		}
	}
}

void pView::init( QAbstractItemView* view )
{
	horizontal = 0;
	vertical = 0;
	
	if ( !view ) {
		return;
	}
	
	if ( view->inherits( "QListView" ) ) {
	}
	else if ( view->inherits( "QTreeView" ) ) {
		QTreeView* tv = qobject_cast<QTreeView*>( view );
		horizontal = new pHeaderView( Qt::Horizontal, view );
		horizontal->setMovable( true );
		horizontal->setStretchLastSection( true );
		horizontal->setDefaultAlignment( Qt::AlignLeft|Qt::AlignVCenter );
		tv->setHeader( horizontal );
	}
	else if ( view->inherits( "QTableView" ) ) {
		QTableView* tv = qobject_cast<QTableView*>( view );
		horizontal = new pHeaderView( Qt::Horizontal, view );
		horizontal->setClickable( true );
		horizontal->setHighlightSections( true );
		vertical = new pHeaderView( Qt::Vertical, view );
		vertical->setClickable( true );
		vertical->setHighlightSections( true );
		tv->setHorizontalHeader( horizontal );
		tv->setVerticalHeader( vertical );
	}
	else if ( view->inherits( "QColumnView" ) ) {
	}
	else if ( view->inherits( "QHeaderView" ) ) {
	}
}
