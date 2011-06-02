#ifndef EXERCISEWIDGET_H
#define EXERCISEWIDGET_H

#include <QWidget>
#include <QList>
#include <QPair>

#include "sql/exercise.h"
#include "exercisewidgetdata.h"

namespace Ui
{
    class ExerciseWidget;
}

class QLineEdit;
class QGridLayout;

class ExerciseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExerciseWidget(QWidget *parent = 0);
    ~ExerciseWidget();
    void setData(const ExerciseWidgetData &data);
    const ExerciseWidgetData &data() const;

    bool save(qint64 userId, qint64 sessionId);
    bool isComplete() const;

protected:
    void changeEvent(QEvent *);

private slots:
    void on_btnAddSerie_clicked();
    void on_btnDeleteSerie_clicked();

private:
    void createLayout();

    Ui::ExerciseWidget *ui;
    QGridLayout *scrollAreaLayout;
    ExerciseWidgetData mData;

    QList<QPair<QLineEdit *, QLineEdit *> > mPairs;
    int mExtraSeries;

};

#endif // EXERCISEWIDGET_H
