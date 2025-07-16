#ifndef F_SYSTEMINFO_H
#define F_SYSTEMINFO_H
#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <QStringList>

class SystemInfo {
public:
    static QString getCpuVendor() {
        return getPsInfo("Get-WmiObject Win32_Processor | Select-Object -ExpandProperty Manufacturer");
    }

    static QString getCpuName() {
        return getPsInfo("Get-WmiObject Win32_Processor | Select-Object -ExpandProperty Name");
    }

    static QString getGpuVendor() {
        return getPsInfo("Get-WmiObject Win32_VideoController | Select-Object -ExpandProperty AdapterCompatibility");
    }

    static QString getGpuName() {
        return getPsInfo("Get-WmiObject Win32_VideoController | Select-Object -ExpandProperty Name");
    }

private:
    static QString getPsInfo(const QString& script) {
        QProcess process;
        QCoreApplication::processEvents(QEventLoop::AllEvents,5);
        process.start("powershell", QStringList() << "-Command" << script);
        process.waitForFinished();

        if (process.exitCode() != 0) {
            qWarning() << "PowerShell Error:" << process.readAllStandardError();
            return "Unknown";
        }

        return QString::fromUtf8(process.readAllStandardOutput()).trimmed();
    }
};
#endif // F_SYSTEMINFO_H
