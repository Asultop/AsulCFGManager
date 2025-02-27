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
class T_Deploy : public BaseScrollPage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Deploy(QWidget *parent = nullptr,QTemporaryDir *tempDir=nullptr);
    ~T_Deploy();
    QStringList scanExistProject();
    QStringList findProjectsWithConfig(const QString& steamPath);
    QStringList ExistDir;

    QString ZipFilePath;
    QTemporaryDir *gLocation;
    QFont subFont;
    QString perfectPath;
    QString steamPath;
    QString CFGPath;
    void changeOccured();
    void getAllPath();
    bool autoDir=true;
    bool autoExe=true;
    bool autoHelp=true;
    bool previewShow=false;
private:
    void AnalyseFile(QString FileName);
    bool UnCompressFiles(QString zipFile, QString sDestPath);
    bool UnCompressFiles2(const QString &archivePath, const QString &extractDir);
    QString checkIfOnlyOneDirectory(const QString &folderPath);
    ElaPushButton *selectFileButton;
    ElaLineEdit *selectedFilePath;
    ElaFlowLayout *flowLayout;
    void extracted(QRegularExpression &regex, QStringList &lines, int &index);
    void generateScrollPageLayout(QString CFGFileLocation,
                                  ElaFlowLayout *cVLayout);

    bool validFile=false;
    QMap<ElaPopularCard*, int> pageNumMap;
    // QMap<QString, QMap<int, QString>> m_configStore;  // 成员变量
    // QString m_currentConfigId;

    QString CFGFileLocations;
    QString getPath(QString vdfFile);

    void createDirectories(const QString &path);
    QString manageDirectory(const QString &CFGPath, const QString &CFGDirName, const QString &CFGFileLocation);
    QString copyRecursively(const QString &sourcePath, const QString &targetPath);
    QString getProcessPath(const QString &processName);
    void resetToDefault();

    // QMap<QString,QVariant> *data;


signals:
    void changePreviewHide(bool hide);
    void targetToFormatHelp();
    void generateError(QString content);
    void changeHideAShowState(bool show);
    void cancelWidget(QString cancelID);
    void pushTextToWaitingDialog(QString content);
    void complete();
};

#endif // T_DEPLOY_H
