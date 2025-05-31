#ifndef T_FORMATHELP_H
#define T_FORMATHELP_H

#include <QObject>
#include <QWidget>
#include "BaseInclude.h"
#include "BaseScrollpage.h"
#include <QTextBrowser>
class T_FormatHelp : public BaseScrollPage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_FormatHelp(QWidget *parent = nullptr);
    ~T_FormatHelp();

signals:
};

#endif // T_FORMATHELP_H
