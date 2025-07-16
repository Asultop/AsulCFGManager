#ifndef ASULWINDOW_H
#define ASULWINDOW_H

#include <QMainWindow>
#include <ElaWindow.h>
class AsulWindow : public ElaWindow
{
    Q_OBJECT
public:
    explicit AsulWindow(QWidget *parent = nullptr);
    virtual void resizeEvent(QResizeEvent *event) override;

    virtual void moveEvent(QMoveEvent *event) override;
    ~AsulWindow();
signals:
    void sizeChanged(QRect *rect);
};

#endif // ASULWINDOW_H
