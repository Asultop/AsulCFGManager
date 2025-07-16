#ifndef BASESCROLLPAGE_H
#define BASESCROLLPAGE_H

#include <QWidget>
#include "ElaScrollPage.h"

#include "BaseInclude.h"
#include "ElaScrollPageArea.h"
#include <ElaListView.h>


class BaseScrollPage : public ElaScrollPage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit BaseScrollPage(QWidget *parent=nullptr);
    ~BaseScrollPage();
    void createCustomWidget(QString desText);
    QWidget* centralWidget,*debugWidget;
    QVBoxLayout* centerVLayout;
    void addTopWidget(QWidget *widget);
    void initWidget(QString title,QString subTitle,QString content);

    static void showSuccEx(QString title,QString content,int mSec=-1);
    static void showErrEx(QString title,QString content,int mSec=-1);
    static void showInfoEx(QString title,QString content,int mSec=-1);
    static void showWarnEx(QString title,QString content,int mSec=-1);

    void showSuccess(QString title,QString content,int mSec=-1);
    void showErr(QString title,QString content,int mSec=-1);
    void showInfo(QString title,QString content,int mSec=-1);
    void showWarn(QString title,QString content,int mSec=-1);

    int TinyHeight=30;
    int MiddleHeight=45;
    int HugeHeight=60;

    int MainFont=25;
    int subFont=15;

    int normSpacing=10;
    int topSpacing=5;

    bool askDialog(QString title,QString content);
    double getReadTime(QString context);
    int charactersPerMinute = 200; // 每分钟阅读的字符数（可调整）

    static bool askDialog(QWidget *parent,QString title,QString content);
    ElaScrollPageArea * GenerateArea(QWidget *parent,ElaText * title, QWidget * widget,bool convert=true);
    ElaScrollPageArea * GenerateArea(QWidget *parent,ElaText * title, QHBoxLayout * widget,bool convert=true);
    ElaScrollPageArea * GenerateArea(QWidget *parent,QWidget * title, QHBoxLayout * widget,bool convert=true);
    ElaScrollPageArea * GenerateArea(QWidget *parent,QHBoxLayout * title, QHBoxLayout * widget,bool convert=true);

};

#endif // BASESCROLLPAGE_H
