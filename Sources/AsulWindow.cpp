#include "Headers/AsulWindow.h"

AsulWindow::AsulWindow(QWidget *parent)
    : ElaWindow{parent}
{

}

void AsulWindow::resizeEvent(QResizeEvent *event)
{
    // 获取窗口的新几何信息
    QRect rect = this->geometry(); // 包含 x, y, width, height
    emit sizeChanged(&rect);

    // 调用基类实现，确保正常处理
    QMainWindow::resizeEvent(event);
}



void AsulWindow::moveEvent(QMoveEvent *event)
{
    QRect rect = this->geometry(); // 包含 x, y, width, height
    emit sizeChanged(&rect);
    QMainWindow::moveEvent(event);
}

AsulWindow::~AsulWindow()
{

}
