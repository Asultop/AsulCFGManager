#ifndef T_SETTING_H
#define T_SETTING_H

#include <QObject>
#include <QWidget>
#include "BaseInclude.h"
#include "BaseScrollpage.h"
#include <QTextBrowser>
class T_Setting : public BaseScrollPage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Setting(QWidget *parent = nullptr);
    ~T_Setting();
    QString perfectPath;
    QString steamPath;
    void updateText();
    bool autoDir=true;
    bool autoExe=true;
    bool autoHelp=true;
    bool previewShow=false;
private:
    ElaText *steamPathText;
    ElaPushButton *steamPathChangeButton;
    ElaText *perfectPathText;
    ElaPushButton *perfectPathChangeButton;

signals:
    void steamPathHasChanged();
    void perfectPathHasChanged();
    void toggleAreaHasChanged();
    void changeAll();
};

#endif // T_SETTING_H
