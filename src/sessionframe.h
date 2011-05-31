#ifndef SESSIONFRAME_H
#define SESSIONFRAME_H

#include "SessionProxy.h"

#include <QFrame>
#include <QList>

namespace Ui
{
    class SessionFrame;
}
class ExerciseWidget;

class SessionFrame : public QFrame, public SessionProxy
{
    Q_OBJECT
public:
    explicit SessionFrame(QWidget *parent = 0);
    ~SessionFrame();

    virtual QSize sizeHint() const;
    
    virtual void setSessionId(qint64 id);
    virtual void setUserId(qint64 id);
    virtual void refresh();
    virtual void start();
    virtual void stop();

signals:
    virtual void sessionFinished();

private slots:
    void on_btnNext_clicked();
    void on_btnPrevious_clicked();
    void on_btnFirst_clicked();
    void on_btnLast_clicked();
    void on_btnFinish_clicked();

private:
    void selectExercises();
    void paginate();

    Ui::SessionFrame *ui;
    qint64 mSessionId;
    qint64 mUserId;
    QList<ExerciseWidget *> exercises;
    int mCurrentPage;
};

#endif // SESSIONFRAME_H
