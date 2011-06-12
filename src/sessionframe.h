#ifndef SESSIONFRAME_H
#define SESSIONFRAME_H

#include "exercisewidgetdata.h"

#include <QFrame>
#include <QList>

namespace Ui
{
    class SessionFrame;
}
class ExerciseWidget;
class SessionProxy;

class SessionFrame : public QFrame
{
    Q_OBJECT
public:
    explicit SessionFrame(SessionProxy *proxy);
    ~SessionFrame();

    virtual QSize sizeHint() const;

    Q_INVOKABLE ExerciseWidgetDataList widgetsData() const;
    Q_INVOKABLE bool objectiveDone() const;
    Q_INVOKABLE void setWidgetsData(const ExerciseWidgetDataList& data, const QString &objective, bool objectiveDone, bool readOnly);

private slots:
    void btnNextClicked();
    void btnPreviousClicked();
    void btnFirstClicked();
    void btnLastClicked();

private:
    void paginate();
    void start();

    Ui::SessionFrame *ui;
    qint64 mSessionId;
    qint64 mUserId;
    QList<ExerciseWidget *> exercises;
    int mCurrentPage;
    SessionProxy *mProxy;
};

#endif // SESSIONFRAME_H
