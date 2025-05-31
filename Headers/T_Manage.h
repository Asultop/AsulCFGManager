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
    Q_INVOKABLE explicit T_Manage(QWidget *parent = nullptr);
    ~T_Manage();
    void update();
    QString CFGPath;
    QString ExistDir;
    void changeOccured();
    bool previewShow=false;
private:
    bool myChange=false;
    bool lock=false;
    QMap<QString,ElaToggleSwitch*> SMap;
    ElaToggleSwitch *getToggle(QString label);
signals:
    void updateData();
    void changePreviewHide(bool hide);
    void textHasChanged();
    void lockSwitch();
};

#endif // T_MANAGE_H
