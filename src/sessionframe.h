#ifndef SESSIONFRAME_H
#define SESSIONFRAME_H

#include <QFrame>

namespace Ui
{
    class SessionFrame;
}

class SessionFrame : public QFrame
{
    Q_OBJECT
public:
    explicit SessionFrame(QWidget *parent = 0);
    ~SessionFrame();

    QSize sizeHint() const;

private:
    Ui::SessionFrame *ui;
};

#endif // SESSIONFRAME_H
