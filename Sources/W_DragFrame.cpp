#include "Headers/W_DragFrame.h"
#include "qevent.h"
#include <QEvent>
#include <QMimeData>
#include <QPen>
#include <QPainter>

void W_DragFrame::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPen pen;
    pen.setWidth(2);


    pen.setColor(Qt::gray); // 默认虚线颜色


    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);

    // 绘制虚线边框
    drawDashedRect(&painter, rect());

    // 绘制文字
    QFont font = painter.font();
    font.setPointSize(10); // 设置字体大小
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(rect(), Qt::AlignCenter, "拖拽文件至此");
}


void W_DragFrame::drawDashedRect(QPainter *painter, const QRect &rect) {
    painter->drawRect(rect.adjusted(1, 1, -1, -1));
}







W_DragFrame::W_DragFrame(QWidget *parent) :
    QWidget(parent)
{

    this->setAcceptDrops(true);

}

W_DragFrame::~W_DragFrame()
{

}

void W_DragFrame::dragEnterEvent(QDragEnterEvent *ev)
{
    if(ev->mimeData()->hasUrls())
    {
        ev->accept();//接收拖动进入事件
    }
    //ev->ignore();//若忽略该事件，则不会发生之后的事件，拖放到此结束
}

void W_DragFrame::dropEvent(QDropEvent *ev)
{
    if(ev->mimeData()->hasUrls())
    {
        QList<QUrl> urls = ev->mimeData()->urls();

        for(int i=0; i<urls.size(); i++)
        {
            QString filename=urls.at(i).toLocalFile();
            emit fileDropped(filename);
            qDebug()<<filename;
        }
    }
}
