#ifndef T_SplashScreen_H
#define T_SplashScreen_H

#include <QObject>
#include <QWidget>
#include "BaseInclude.h"
#include "BaseScrollpage.h"
#include <QTextBrowser>
#include <ElaWindow.h>

class T_SplashScreen : public ElaWindow
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_SplashScreen(QWidget *parent = nullptr);
    ~T_SplashScreen();

signals:
};

#endif // T_SplashScreen_H
