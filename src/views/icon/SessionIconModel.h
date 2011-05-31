#ifndef SESSIONICONMODEL_H
#define SESSIONICONMODEL_H

#include <QStringListModel>

class SessionIconModel : public QStringListModel
{
	Q_OBJECT
	
public:
	SessionIconModel( QObject* parent = 0 );
	virtual ~SessionIconModel();
};

#endif // SESSIONICONMODEL_H
