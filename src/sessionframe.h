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
    Q_INVOKABLE void setWidgetsData(const ExerciseWidgetDataList& data, bool readOnly);
    Q_INVOKABLE void setObjective(const QString &objective);

private slots:
    void on_btnNext_clicked();
    void on_btnPrevious_clicked();
    void on_btnFirst_clicked();
    void on_btnLast_clicked();

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
