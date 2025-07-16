#ifndef T_MANAGE_H
#define T_MANAGE_H

#include <QObject>
#include <QWidget>
#include "BaseInclude.h"
#include "BaseScrollpage.h"
#include <QTextBrowser>
#include <ElaToggleSwitch.h>
class T_Manage : public BaseScrollPage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Manage(QWidget *parent = nullptr,bool readPolicy = false);
    ~T_Manage();
    QString CFGPath;
    QString ExistDir;


private:
    bool myChange=false;
    bool lock=false;
    QStringList allKeyHere={};
    QMap<QString,ElaToggleSwitch*> SMap;
    ElaToggleSwitch *getToggle(QString label);
    QVBoxLayout *funcLayout;
    void clearLayout(QVBoxLayout * layout);
    QWidget *funcWidget;
    QString autoExecFileContent;
    QFile *autoexecCfg;
signals:
    void updateData();
    void textHasChanged();
    void lockSwitch();
};

#endif // T_MANAGE_H
