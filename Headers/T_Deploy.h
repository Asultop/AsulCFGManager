#ifndef T_DEPLOY_H
#define T_DEPLOY_H

#include <QObject>
#include <QWidget>
#include "BaseScrollpage.h"
#include <QTemporaryDir>
#include <ElaFlowLayout.h>
#include <QStringList>
#include <ElaWindow.h>
#include <QSettings>
#include <ElaComboBox.h>

class T_Deploy : public BaseScrollPage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Deploy(QWidget *parent = nullptr,QTemporaryDir *tempDir=nullptr);
    ~T_Deploy();
    QString ZipFilePath;
    QTemporaryDir *gLocation;
    QFont subFont;
    QString perfectPath;
    QString steamPath;
    QString CFGPath;
private:
    void AnalyseFile();
    bool UnCompressFiles(QString zipFile, QString sDestPath);
    QString checkIfOnlyOneDirectory(const QString &folderPath);
    ElaPushButton *selectFileButton;
    ElaLineEdit *selectedFilePath;
    ElaFlowLayout *flowLayout;
    void generateScrollPageLayout(QString CFGFileLocation,ElaFlowLayout *cVLayout);
    bool validFile=false;
    QString CFGFileLocations;
    QString getPath(QString vdfFile);
    void getAllPath();
signals:
    void targetToFormatHelp();
    void generateError(QString content);
};

#endif // T_DEPLOY_H
