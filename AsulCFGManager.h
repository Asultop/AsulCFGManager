#ifndef Asul_H
#define Asul_H

#include <QMainWindow>
#include <ElaWindow.h>
#include "Headers/T_Deploy.h"
#include "Headers/T_FormatHelp.h"
#include "Headers/T_Setting.h"
#include "Headers/T_About.h"
#include "Headers/AsulWindow.h"
#include "Headers/W_SCFGTransformer.h"
#include <QTemporaryDir>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
class Asul : public AsulWindow
{
    Q_OBJECT

public:
    QString GlobalLocation;
    QTemporaryDir *gLocation;
    QString _settingKey={""},_aboutKey={""},_formatKey={""};
    QString getPath(QString vdfFile);
    QString getProcessPath(const QString &processName);
    void getAllPath();

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
    void targetToSetting();
    T_FormatHelp *formatPage;
    T_Setting *settingPage;
    W_SCFGTransformer* SCFGPage;
    Asul(QWidget *parent = nullptr);
    ~Asul();

    void showEvent(QShowEvent *event) override;
private:
    T_About *aboutpage;
    bool readPolicy,hwInfo,logMode;
    bool m_initialized = false;
    // bool ask(QString title,QString content);
protected:
    void dragEnterEvent(QDragEnterEvent* ev) override;
    void dropEvent(QDropEvent* ev) override;
signals:
    void fileDropped(QString fileName);
    void initFinished();
};
#endif // Asul_H
