#include "Headers/GlobalSettings.h"
// #include "QsLog.h"
Q_SINGLETON_CREATE_CPP(GlobalSettings)
GlobalSettings::GlobalSettings(QObject *parent)
    : QObject{parent}
{

}

void GlobalSettings::logM(QString val){
    Q_UNUSED(val);
}

