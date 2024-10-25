#ifndef W_DRAGFRAME_H
#define W_DRAGFRAME_H

#include <QWidget>
#include <ElaFlowLayout.h>
namespace Ui {
class W_DragFrame;
}

class W_DragFrame : public QWidget
{
    Q_OBJECT

public:
    Q_INVOKABLE explicit W_DragFrame(QWidget *parent = 0);
    ~W_DragFrame();

protected:
    void dragEnterEvent(QDragEnterEvent* ev) override;
    void dropEvent(QDropEvent* ev) override;
    void paintEvent(QPaintEvent *event) override;
private:
    void drawDashedRect(QPainter *painter, const QRect &rect);
signals:
    void fileDropped(const QString &filename);
};


#endif // W_DRAGFRAME_H
