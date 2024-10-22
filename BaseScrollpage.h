#ifndef BASESCROLLPAGE_H
#define BASESCROLLPAGE_H

#include <QWidget>
#include "ElaScrollPage.h"

#include "BaseInclude.h"
class BaseScrollPage : public ElaScrollPage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit BaseScrollPage(QWidget *parent=nullptr);
    ~BaseScrollPage();
    void createCustomWidget(QString desText);
    QWidget* centralWidget;
    QVBoxLayout* centerVLayout;
    void addTopWidget(QWidget *widget);
    void initWidget(QString title,QString subTitle,QString content);

    int TinyHeight=30;
    int MiddleHeight=45;
    int HugeHeight=60;

    int MainFont=25;
    int subFont=15;

    int normSpacing=10;
    int topSpacing=5;
};

#endif // BASESCROLLPAGE_H
