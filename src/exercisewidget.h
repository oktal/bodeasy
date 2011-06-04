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

class QSpinBox;
class QGridLayout;

class ExerciseWidget : public QWidget
{
    friend class SessionFrame;

    Q_OBJECT
public:
    explicit ExerciseWidget(QWidget *parent = 0);
    ~ExerciseWidget();
    void setData(const ExerciseWidgetData &data);
    const ExerciseWidgetData &data() const;

    bool save(qint64 userId, qint64 sessionId, qint64 sessionMadeId);
    bool isComplete() const;

protected:
    void changeEvent(QEvent *);

private slots:
    void on_btnAddSerie_clicked();
    void on_btnDeleteSerie_clicked();
    
    void spinBox_valueChanged(int value);

private:
    typedef QPair<QSpinBox *, QSpinBox *> PairSpinBox;
    ExerciseWidget::PairSpinBox addRow();
    void createLayout();
    bool saveResult(qint64 resultId, qint64 sessionMadeId);
    void selectResults();

    Ui::ExerciseWidget *ui;
    QGridLayout *scrollAreaLayout;
    ExerciseWidgetData mData;

    QList<ExerciseWidget::PairSpinBox> mPairs;
};

#endif // EXERCISEWIDGET_H
