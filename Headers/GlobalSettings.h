#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H


#include <QObject>

#include "Headers/stdafx.h"
#include "Headers/singleton.h"
#include "Headers/BaseInclude.h"
#include "qsettings.h"
#include <ElaLog.h>
#include <QsLog.h>
#include <QTemporaryDir>

#define gSettings GlobalSettings::getInstance()
#define log(x) gSettings->logM(x)

class GlobalSettings : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(GlobalSettings);
    Q_PROPERTY_CREATE(QString,CFGPath);
    Q_PROPERTY_CREATE(QString,SteamPath);
    Q_PROPERTY_CREATE(QString,SteamUserPath);
    Q_PROPERTY_CREATE(QString,SteamConfPath);
    Q_PROPERTY_CREATE(QString,PerfectPath);
    Q_PROPERTY_CREATE(bool,AutoExe);
    Q_PROPERTY_CREATE(bool,AutoDir);
    Q_PROPERTY_CREATE(bool,AutoHelp);
    Q_PROPERTY_CREATE(bool,PreviewShow);
    Q_PROPERTY_CREATE(QRect*,ParentRect);
    Q_PROPERTY_CREATE(QTemporaryDir*,GLocation);
    Q_PROPERTY_CREATE(QSettings*,Settings);


    // Q_PROPERTY_CREATE();
public:
    explicit GlobalSettings(QObject *parent = nullptr);
    bool logMode;
    void logM(QString val);
signals:
};

#endif // GLOBALSETTINGS_H
