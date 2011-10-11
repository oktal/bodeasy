#ifndef SESSIONCONTROLWIDGET_H
#define SESSIONCONTROLWIDGET_H

#include <QWidget>

namespace Ui
{
class SessionControlWidget;
}

class QSignalMapper;

class SessionControlWidget : public QWidget
{
    Q_OBJECT

public:
    enum Button { PreviousButton = 0, NextButton, FirstButton, LastButton, FinishButton };

    explicit SessionControlWidget(QWidget *parent = 0);
    ~SessionControlWidget();

    void setButtonEnabled(SessionControlWidget::Button button, bool enabled);
    bool isButtonEnabled(SessionControlWidget::Button button) const;
    
    void setObjectiveVisible(bool visible);
    bool isObjectiveVisible() const;
    
    void setObjectiveText(const QString &text);
    QString objectiveText() const;
    
    void setObjectiveReadOnly(bool readOnly);
    bool isObjectiveReadOnly() const;
    
    void setObjectiveChecked(bool checked);
    bool isObjectiveChecked() const;

    void setComment(const QString &comment);
    QString comment() const;

    void setCommentReadOnly(bool readOnly);
    bool isCommentReadOnly() const;

signals:
    void previousClicked();
    void nextClicked();
    void firstClicked();
    void lastClicked();
    void finishClicked();
    void buttonClicked(SessionControlWidget::Button button);
    void objectiveClicked(bool checked);

protected:
    void changeEvent(QEvent *e);

private slots:
    void onButtonClicked(int id);

private:
    Ui::SessionControlWidget *ui;

    QSignalMapper *mapper;
};

#endif // SESSIONCONTROLWIDGET_H
