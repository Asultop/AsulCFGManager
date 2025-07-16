#ifndef T_DEPLOY_H
#define T_DEPLOY_H

#include <QObject>
#include <QWidget>
#include "BaseScrollpage.h"
#include <QTemporaryDir>
#include <ElaFlowLayout.h>
#include "ElaContentDialog.h"
#include <QStringList>
#include <ElaWindow.h>
#include <QSettings>
#include <ElaComboBox.h>
#include "ElaPopularCard.h"
#include "Headers/W_DragFrame.h"
#include "Headers/F_Translator.h"
class T_Deploy : public BaseScrollPage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Deploy(QWidget *parent = nullptr);
    ~T_Deploy();
    QFont subFont;
    QStringList scanExistProject();
    QStringList findProjectsWithConfig(const QString& steamPath);
    QStringList ExistDir;
    QString ZipFilePath;
    QString perfectPath;
    QString steamPath;
    QString CFGPath;
    QString CPUVendor,GPUVendor,CPUName,GPUName;
    void getAllPath();



private:
    bool validFile=false;

    F_Translator *trans;
    QString targetLang;
    QString checkIfOnlyOneDirectory(const QString &folderPath);
    QString replaceArgs(QString val);
    QString CFGFileLocations;
    QString getPath(QString vdfFile);
    QString manageDirectory(const QString &CFGPath, const QString &CFGDirName, const QString &CFGFileLocation);
    QString copyRecursively(const QString &sourcePath, const QString &targetPath);
    QString getProcessPath(const QString &processName);
    QStringList parseLine(QString line);
    ElaPushButton *selectFileButton;
    ElaLineEdit *selectedFilePath;
    ElaFlowLayout *flowLayout;
    bool UnCompressFiles(QString zipFile, QString sDestPath);
    bool UnCompressFiles2(const QString &archivePath, const QString &extractDir);
    QMap<ElaPopularCard*, int> pageNumMap;
    void AnalyseFile(QString FileName);
    void extracted(QRegularExpression &regex, QStringList &lines, int &index);
    void generateScrollPageLayout(QString CFGFileLocation,
                                  ElaFlowLayout *cVLayout);
    void createDirectories(const QString &path);
    void resetToDefault();

    // QMap<QString,QVariant> *data;


signals:
    void targetToFormatHelp();
    void generateError(QString content);
    void cancelWidget(QString cancelID);
    void pushTextToWaitingDialog(QString content);
};

#endif // T_DEPLOY_H
