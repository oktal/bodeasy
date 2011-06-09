#ifndef SESSIONFRAME_H
#define SESSIONFRAME_H

#include "SessionProxy.h"
#include "exercisewidgetdata.h"

#include <QFrame>
#include <QList>

namespace Ui
{
    class SessionFrame;
}
class ExerciseWidget;

class SessionFrame : public QFrame
{
    Q_OBJECT
public:
    explicit SessionFrame(QWidget *parent = 0);
    ~SessionFrame();

    virtual QSize sizeHint() const;
    SessionProxy* sessionProxy() const;

    Q_INVOKABLE ExerciseWidgetDataList widgetsData() const;
    Q_INVOKABLE void setWidgetsData( const ExerciseWidgetDataList& data, bool readOnly );

private slots:
    void on_btnNext_clicked();
    void on_btnPrevious_clicked();
    void on_btnFirst_clicked();
    void on_btnLast_clicked();
    void on_btnFinish_clicked();

private:
    void paginate();
    void start();

    Ui::SessionFrame *ui;
    qint64 mSessionId;
    qint64 mUserId;
    QList<ExerciseWidget *> exercises;
    int mCurrentPage;
};

#endif // SESSIONFRAME_H
