#ifndef Asul_H
#define Asul_H

#include <QMainWindow>
#include <ElaWindow.h>
#include "Headers/T_Deploy.h"
#include "Headers/T_FormatHelp.h"
#include "Headers/T_Setting.h"
#include "Headers/T_About.h"
#include <QTemporaryDir>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
class Asul : public ElaWindow
{
    Q_OBJECT

public:
    QString GlobalLocation;
    QTemporaryDir *gLocation;
    QString _settingKey={""},_aboutKey={""};
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
    Asul(QWidget *parent = nullptr);
    ~Asul();
private:
    T_About *aboutpage;
protected:
    void dragEnterEvent(QDragEnterEvent* ev) override;
    void dropEvent(QDropEvent* ev) override;
signals:
    void fileDropped(QString fileName);
};
#endif // Asul_H
