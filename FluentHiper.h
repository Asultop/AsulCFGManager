#ifndef FLUENTHIPER_H
#define FLUENTHIPER_H

#include <QMainWindow>
#include <ElaWindow.h>
#include "Headers/T_Deploy.h"
#include "Headers/T_FormatHelp.h"
#include <QTemporaryDir>
class FluentHiper : public ElaWindow
{
    Q_OBJECT

public:
    QString GlobalLocation;
    QTemporaryDir *gLocation;
    void warnTips(ElaWindow *parent,QString title,QString context);
    void succTips(ElaWindow *parent,QString title,QString context);
    void normTips(ElaWindow *parent,QString title,QString context);
    void errTips(ElaWindow *parent,QString title,QString context);
    void originTips(ElaWindow *parent,QString title,QString context,ElaMessageBarType::MessageMode type);
    void warnTips(QString title,QString context);
    void succTips(QString title,QString context);
    void normTips(QString title,QString context);
    void errTips(QString title,QString context);
    void originTips(QString title,QString context,ElaMessageBarType::MessageMode type);
    void saveResourceFile(QString resProfiex,QString resFileName,QString destFullPathFileName);
    void targetToFormatHelp();
    T_FormatHelp *formatPage;
    FluentHiper(QWidget *parent = nullptr);
    ~FluentHiper();
};
#endif // FLUENTHIPER_H
