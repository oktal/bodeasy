#ifndef SESSIONFRAME_H
#define SESSIONFRAME_H

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

    QSize sizeHint() const;
    void setSessionId(qint64 id);
    void setUserId(qint64 id);
    void refresh();
    void start();

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
