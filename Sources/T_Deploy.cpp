#include "Headers/T_Deploy.h"

#include <QQueue>
#include <QFileDialog>
#include <QStandardPaths>
#include <QTemporaryDir>
#include "AsulCFGManager.h"
#include <ElaContentDialog.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <ElaPopularCard.h>
#include <QDesktopServices>
#include "Headers/F_Collector.h"
#include <QDirIterator>
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <ElaMessageBar.h>
#include <Headers/BanElaComboBox.h>
#include <QSettings>
#include <QsLog.h>


#define DBG(x) QLOG_DEBUG()<<"[Key] "<<#x<<" : "<<x;
#define _DBG(label,x) QLOG_DEBUG()<<"["<<label<<"] "<<#x<<" : "<<x;

using namespace QsLogging;
#define parentWindow dynamic_cast<Asul *>(this->parent())
T_Deploy::T_Deploy(QWidget *parent)
    : BaseScrollPage{parent}
{
    initWidget("",tr("安装"),tr("选择符合格式的安装包")+"\n"+tr("我们会为您解析数据并自动安装到您的CFG目录下"));

    steamPath=gSettings->getSteamPath();
    perfectPath=gSettings->getPerfectPath();
    CFGPath=gSettings->getCFGPath();
    connect(gSettings,&GlobalSettings::pSteamPathChanged,[=](){
        steamPath=gSettings->getSteamPath();
    });
    connect(gSettings,&GlobalSettings::pPerfectPathChanged,[=](){
        perfectPath=gSettings->getPerfectPath();
    });
    connect(gSettings,&GlobalSettings::pCFGPathChanged,[=](){
        CFGPath=gSettings->getCFGPath();
    });

    // waitingDialog->show();
    trans=new F_Translator(this);
    QSettings setting("HKEY_CURRENT_USER\\Software\\Asul\\AM",QSettings::NativeFormat);
    QString tar=setting.value("Language").toString();
    if(tar=="zh_CN") targetLang="zh";
    else if(tar=="en_US") targetLang="en";
    emit pushTextToWaitingDialog(tr("寻找文件目录"));

    this->setAcceptDrops(true);
    emit pushTextToWaitingDialog(tr("获取目录"));
    // getAllPath();

    // QDesktopServices::openUrl(CFGPath);
    QString tempLocation=QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    // data->insert("gLocation",gLocation->path());

    emit pushTextToWaitingDialog(tr("构建布局"));
    // ElaScrollPageArea *selectFileArea=new ElaScrollPageArea(this);
    QHBoxLayout *selectFileLayout=new QHBoxLayout();
    ElaMessageButton *scanFileButton=new ElaMessageButton(tr("扫描"),this);
    scanFileButton->setBarText(tr("扫描"));
    scanFileButton->setBarTitle(tr("扫描中"));
    scanFileButton->setPositionPolicy(ElaMessageBarType::Top);
    scanFileButton->setDisplayMsec(1000);

    ElaPushButton *cleanFileButton=new ElaPushButton(tr("移除全部"),this);

    // ElaText *selectFileText=new ElaText(tr("选择文件:"),this);
    selectedFilePath=new ElaLineEdit(this);
    selectFileButton=new ElaPushButton(tr("选择"),this);

    selectFileButton->setMaximumWidth(200);
    scanFileButton->setMaximumWidth(200);
    cleanFileButton->setMaximumWidth(200);
    selectFileButton->setMinimumHeight(MiddleHeight);
    cleanFileButton->setMinimumHeight(MiddleHeight);
    scanFileButton->setMinimumHeight(MiddleHeight);
    // ElaPushButton *analyFile=n\=]-[p0oijuhgbv c=-p809ou7yd xz765w ElaPushButton("分析文件",this);
    // analyFile->setMinimumHeight(HugeHeight);
    // selectFileText->setTextPixelSize(15);
    selectedFilePath->setReadOnly(true);
    selectedFilePath->setMinimumHeight(MiddleHeight);
    selectedFilePath->setPlaceholderText(tr("请选择文件"));
    // selectFileLayout->addWidget(selectFileText);
    // selectFileLayout->addWidget(selectedFilePath);
    // selectFileText->hide();
    selectedFilePath->hide();
    selectFileButton->setMinimumHeight(MiddleHeight);
    selectFileLayout->addWidget(selectFileButton);
    selectFileLayout->addWidget(scanFileButton);
    selectFileLayout->addWidget(cleanFileButton);
    connect(cleanFileButton,&ElaPushButton::clicked,[=](){
        for (int i = 0; i < flowLayout->count(); ++i) {
            QLayoutItem* item = flowLayout->itemAt(i);
            if (!item) continue;

            // 检查是否是控件（QWidget）
            if (QWidget* widget = item->widget()) {
                QLOG_DEBUG() << "[CleanLayout] Find Child：" << widget->objectName();
                widget->deleteLater();
            }
        }
    });

    connect(scanFileButton,&ElaPushButton::clicked,[=](){
        scanFileButton->setDisabled(true);
        scanFileButton->setText(tr("扫描中.."));
        QStringList RET=scanExistProject();
        _DBG("ScanExistProject",RET);
        //QTimer::singleShot(100,this,[=](){
        scanFileButton->setText(tr("扫描"));

        // QCoreApplication::processEvents(QEventLoop::AllEvents,5);
        if(!RET.isEmpty()){
            QStringList nameDir;
            foreach(QString Dir,RET){
                QDir DF(Dir);
                // DF.dirName();
                nameDir.append(DF.dirName());
            }
            QString buffer=QString(tr("存在配置:[%1]")).arg(nameDir.join(","));
            // parentWindow->succTips("扫描",buffer);
            QTimer::singleShot(50,this,[=](){
                ElaMessageBar::success(ElaMessageBarType::Top,tr("扫描"),buffer,getReadTime(buffer),parent);
            });
        }else showInfo(tr("扫描"),tr("未找到存在的配置"));
        scanFileButton->setDisabled(false);
    });
    // W_DragFrame *selectDragFrame = new W_DragFrame(this);

    // connect(selectDragFrame, &W_DragFrame::fileDropped, this, [=](const QString &val){
    //     parentWindow->normTips("File",val);
    //     QLOG_DEBUG()<<val;
    // });
    // selectFileLayout->addWidget(selectDragFrame);
    // selectDragFrame->setMinimumHeight(60);
    // addTopWidget(selectDragFrame);
    // DragFrame未完善
    // selectDragFrame->hide();
    connect(selectFileButton,&ElaPushButton::clicked,[=](){
        QLOG_DEBUG()<<"[Deploy_Func] selectFile START";
        QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("打开 CFG包 文件"),
            // QDir::homePath(),
            ".",
            tr("CFG包 文件 (*.zip *.tar *.gz *.bz2 *.rar *.7z)")
            );
        _DBG("selectFileLocation",fileName);
        if (!fileName.isEmpty()) {
            // selectedFilePath->setText(fileName);
            ZipFilePath=fileName;
            // QLOG_DEBUG()<<"ZipFilePath"<<ZipFilePath;
            QString onlyFileName;
            foreach(QString val,ZipFilePath.split("/")){
                onlyFileName=val;
            }
            onlyFileName=onlyFileName.split(".")[0];
            AnalyseFile(onlyFileName);
        }
        else {
            // parentWindow->normTips(tr("文件"),tr("取消选择"));
        }
        QLOG_DEBUG()<<"[Deploy_Func] selectFile END";
    });



    this->addTopWidget(GenerateArea(this,new ElaText(tr("功能"),this),selectFileLayout));


    centerVLayout->addStretch();
    flowLayout=new ElaFlowLayout(0,5,5);
    flowLayout->setIsAnimation(true);
    flowLayout->setContentsMargins(10,10,10,10);
    // flowLayout->setContentsMargins(5,5,20,30);
    centerVLayout->addLayout(flowLayout,2);


    //init deploywindow
    //scan ExistProjectInCFG
    // scanExistProject();
    // QTimer::singleShot(50,this,[=](){
    //     this->scanExistProject();
    // });
    
}

void T_Deploy::AnalyseFile(QString FileName){
    QLOG_DEBUG()<<"[Deploy_Func] AnalyseFile START";
    Asul *window=parentWindow;
    QTemporaryDir *gLocation=gSettings->getGLocation();
    QDir GlobalDir(gLocation->path());

    if(!GlobalDir.exists("Analy")){
        GlobalDir.mkdir("Analy");
    }
    QDir AnalyDir(gLocation->path()+"/Analy");
    if(AnalyDir.exists(FileName)){
        QDir FileDir(gLocation->path()+"/Analy/"+FileName);
        FileDir.removeRecursively();
    }
    QFileInfo zipFileInfo(ZipFilePath);
    if(zipFileInfo.size()>500*1024*1024){

        window->warnTips(tr("文件"),QString(tr("包过大 %1MB (超过 500 MB)")).arg(zipFileInfo.size()/1024/1024));
        QLOG_DEBUG()<<"[Err] "<<"File Error : Package Too Heavy!! (More than 500 MB)";
        return;
    }
    QString UncompressFileLocation=gLocation->path()+"/Analy/"+FileName;


    UnCompressFiles(ZipFilePath.replace("\\\\","/"),UncompressFileLocation.replace("\\\\","/"));

    //Read Info

    static bool hasConnected=false;
    if(!hasConnected){
        hasConnected=true;
        connect(this,&T_Deploy::generateError,this,[=](QString content){
            ElaContentDialog *errorResult=new ElaContentDialog(this);
            QWidget *errorWidget=new QWidget(this);
            ElaLineEdit *errorLine=new ElaLineEdit(this);
            ElaText *errorText=new ElaText(tr("错误"),this);
            errorText->setTextPixelSize(MainFont);
            QVBoxLayout *errorlayout=new QVBoxLayout(errorWidget);
            errorWidget->setLayout(errorlayout);
            errorLine->setMinimumHeight(MiddleHeight);
            errorLine->setReadOnly(true);
            errorLine->setText(content);
            errorlayout->addSpacing(topSpacing);
            errorlayout->addWidget(errorText);
            errorlayout->addSpacing(normSpacing);
            errorlayout->addWidget(errorLine);
            errorResult->setLeftButtonText(tr("取消"));
            errorResult->setMiddleButtonText(tr("了解格式"));
            errorResult->setRightButtonText(tr("重选"));
            errorResult->setCentralWidget(errorWidget);
            connect(errorResult,&ElaContentDialog::rightButtonClicked,[=](){
                // this->selectedFilePath->clear();
                this->selectFileButton->click();
            });
            connect(errorResult,&ElaContentDialog::middleButtonClicked,[=](){
                // this->selectedFilePath->clear();
                // static_cast<FluentHiper *>(this->parent())->targetToFormatHelp();
                emit targetToFormatHelp();
            });
            connect(errorResult,&ElaContentDialog::leftButtonClicked,[=](){
                // this->selectedFilePath->clear();
            });
            errorResult->exec();
            errorResult->deleteLater();
        });
    }
    // QLOG_DEBUG()<<"connect End";

    QString CFGMainPath=checkIfOnlyOneDirectory(UncompressFileLocation);
    //error catch end
    //Get Generate ElaScrollPage;

    // data->insert("CFGMainPath",CFGMainPath);
    if(!CFGMainPath.isEmpty())
        generateScrollPageLayout(CFGMainPath,flowLayout);
    QLOG_DEBUG()<<"[Deploy_Func] AnalyseFile END";
}

#include <QDir>
#include <QDebug>

QString T_Deploy::checkIfOnlyOneDirectory(const QString &folderPath) {
    QLOG_DEBUG()<<"[Deploy_Func] "<<"checkIfOnlyOneDirectory ";
    QDir dir(folderPath);
    if (!dir.exists()) {
        emit generateError(tr("没有相关文件夹"));
        return "";
    }

    // 获取目录中所有条目的列表，包括文件和目录
    QStringList entries = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::SortFlags(QDir::Name | QDir::IgnoreCase));

    // 检查是否恰好有一个目录
    if (entries.size() == 1) {
        QString subDirPath = dir.filePath(entries.first());
        return subDirPath;
    } else if (entries.isEmpty()) {
        emit generateError(tr("文件包为空"));
        return "";
    } else {
        emit generateError(tr("文件包格式错误"));
        return "";
    }

}
bool T_Deploy::UnCompressFiles2(const QString &archivePath, const QString &extractDir){
    QLOG_DEBUG()<<"[Deploy_Func] UncompressFiles";
    QString program;
#ifdef Q_OS_WIN
    QTemporaryDir *gLocation=gSettings->getGLocation();
    program = gLocation->path()+"/7z.exe"; // 默认安装路径
    if (!QFile::exists(program)) {
        program = QStandardPaths::findExecutable("7z.exe");

    }
#else
    program = QStandardPaths::findExecutable("7z");
#endif

    if (program.isEmpty()) {
        qWarning() << "7-Zip not found!";
        return false;
    }

    QProcess process;
    process.setProgram(program);
    process.setArguments({
        "x",
        archivePath,
        "-o" + extractDir,
        "-y" // 覆盖已存在文件
    });
    _DBG("Process",process.program());
    _DBG("Process",process.arguments());
    process.start();
    if (!process.waitForFinished(30000)) { // 30秒超时
        qWarning() << "[Process] timeout:" << process.errorString();
        return false;
    }

    if (process.exitCode() != 0) {
        qWarning() << "Extraction failed. Exit code:"
                   << process.exitCode()
                   << "Error:"
                   << process.readAllStandardError();
        return false;
    }

    return true;
}
bool T_Deploy::UnCompressFiles(QString zipFile, QString sDestPath)
{
    // waitingWidget("UnCompress");

    QTemporaryDir* gLocation=gSettings->getGLocation();
    emit pushTextToWaitingDialog(QString("解压").arg(zipFile));
    QLOG_DEBUG()<<"getIn UnCompressFiles";
    QStringList sParams;
    sParams.append("x");												// 在当前目录中，使用完整路径从压缩档案中释放文件.或者到指定的输出文件夹
    sParams.append(zipFile);
    sParams.append(QString("-o%1").arg(sDestPath));						// 指定压缩文件格式
    sParams.append("-aoa");												// 不提示，直接覆盖同名文件

    QProcess process;

    QString strPath =gLocation->path()+"\\7z.exe";
    process.start(strPath, sParams);
    QCoreApplication::processEvents(QEventLoop::AllEvents,5);
    process.waitForFinished();
    QString A=process.readAllStandardError();
    QString B=process.readAllStandardOutput();
    // QLOG_DEBUG()<<A;
    // QLOG_DEBUG()<<B;
    process.close();
    QLOG_DEBUG()<<"Out UnConpressFiles";
    return (A+B).isEmpty()?false:true;
    // cancelWidget("UnCompress");
}
#include <ElaApplication.h>
QStringList T_Deploy::findProjectsWithConfig(const QString& steamPath) {
    QLOG_DEBUG()<<"[Deploy_Func] findProjectsWithConfig START";
    QCoreApplication::processEvents(QEventLoop::AllEvents,5);
    QStringList AllExistProjects;

    // 检查目录是否存在
    DBG(steamPath);
    QDir parentDir(steamPath);
    if (!parentDir.exists()) {
        qWarning() << "目录不存在：" << steamPath;
        return AllExistProjects;
    }

    // 遍历所有直接子目录（不递归）
    QDirIterator it(steamPath,
                    QDir::Dirs | QDir::NoDotAndDotDot,
                    QDirIterator::NoIteratorFlags);

    while (it.hasNext()) {
        QString subDir = it.next();
        QFileInfo configFile(QDir(subDir).absoluteFilePath("config.asuls"));

        // 检查是否是普通文件且存在
        if (configFile.isFile()) {
            AllExistProjects.append(subDir);
        }
    }

    return AllExistProjects;
}
QStringList T_Deploy::scanExistProject(){
    emit pushTextToWaitingDialog(tr("寻找本地CFG管理文件"));
    for (int i = 0; i < flowLayout->count(); ++i) {
        QLayoutItem* item = flowLayout->itemAt(i);
        if (!item) continue;

        // 检查是否是控件（QWidget）
        if (QWidget* widget = item->widget()) {
            QLOG_DEBUG() << "[Clean_Layout] Find Child：" << widget->objectName();
            widget->deleteLater();
        }
    }
    ExistDir=findProjectsWithConfig(CFGPath);
    foreach (QString dirval,ExistDir){
        generateScrollPageLayout(dirval,flowLayout);
    }

    // flowLayout= new ElaFlowLayout(0,5,5);
    emit cancelWidget("scan");
    _DBG("scanExist_Result",ExistDir);
    return ExistDir;
}
// QStringList T_Deploy::parseLine(QString line){
//     QStringList result;
//     bool getIn=false,skip=false;
//     QString temp;
//     for(int i=0;i<=line.length();i++){
//         if(skip){
//             temp.append(line[i]);
//             skip=false;
//             continue;
//         }
//         if()
//     }
// }
void T_Deploy::generateScrollPageLayout(QString CFGFileLocation, ElaFlowLayout *cVLayout) {
    // FluentHiper *window=dynamic_cast<FluentHiper *>(parent());
    QLOG_DEBUG()<<"[Deploy_Func] generateScrollPageLayout START it's a important part of the program, may cause MORE CRASH!!!";
    bool showable=true;
    if (CFGFileLocations.contains(ZipFilePath)) {
        // parentWindow->errTips("文件", "已存在或无效的包: " + ZipFilePath);
        // return;
    }

    CFGFileLocations.append(ZipFilePath + "\n");
    DBG(CFGFileLocation);
    QFile *asulsFile=new QFile(CFGFileLocation +"/config.asuls",this);
    QString asulsFileContent;
    if(asulsFile->open(QIODevice::ReadOnly)){
        asulsFileContent=asulsFile->readAll();
        asulsFile->close();
    }
    else{
        emit generateError(tr("Asuls 文件:")+asulsFile->errorString());
        QLOG_DEBUG()<<"[DEBUGABLE_INFORMATION] Attmpts to operate "<<CFGFileLocation +"/config.asuls";
        QLOG_DEBUG()<<"[FILE] Asuls File:"<<asulsFile->errorString();
        return;
    }
    asulsFile->deleteLater();

    QStringList fileNames;
    QStringList filePresects;

    QStringList singleNames;
    QString exeFile,dirName,helpFile;
    QString CFGName,CFGAuthor,CFGVersion,CFGDetails;
    QString pngFilePath;
    QString updateVersion;
    bool INI=false,DIR=false,EXE=false,HELP=false,PNG=false, UPDATE=false;
    foreach(QString File,asulsFileContent.split("\n")){
        QLOG_DEBUG()<<"[Foreach] "<<asulsFileContent<<" at "<<File;


        if(File.startsWith("[INI]")){
            QLOG_DEBUG()<<"[INI] Part Parsing";
            QStringList info=File.replace("\r","").replace("\n","").split(" ");
            if(info.length()<5){
                QLOG_DEBUG()<<"[DEBUGABLE_INFORMATION] Parsing "<<File<<" : less than 5 arguments";
                QLOG_DEBUG()<<"[INI] Missing Arguments";
                emit generateError(tr("Asuls 文件(config.asuls): [INI]错误"));
                showable=false;
                return;
            }
            CFGName = info[1];
            CFGVersion = info[2];
            CFGAuthor = info[3];
            CFGDetails = info[4];
            INI=true;
            continue;
        }
        else if(File.startsWith("[EXE]")){
            QLOG_DEBUG()<<"[EXE] Part Parsing";
            QStringList exeList=File.split(" ");
            if(exeList.length()<2){
                QLOG_DEBUG()<<"[DEBUGABLE_INFORMATION] Parsing "<<File<<" : less than 2 arguments";
                QLOG_DEBUG()<<"[EXE] Missing Arguments";
                emit generateError(tr("Asuls 文件(config.asuls): [EXE]错误"));
                showable=false;
                return;
            }
            exeFile=exeList[1];
            EXE=true;
            continue;
        }
        else if(File.startsWith("[DIR]")){
            QLOG_DEBUG()<<"[DIR] Part Parsing";
            QStringList dirList=File.split(" ");
            if(dirList.length()<2){
                QLOG_DEBUG()<<"[DEBUGABLE_INFORMATION] Parsing "<<File<<" : less than 2 arguments";
                QLOG_DEBUG()<<"[DIR] Missing Arguments";
                emit generateError(tr("Asuls 文件(config.asuls): [DIR]错误"));
                showable=false;
            }
            dirName=dirList[1];
            DIR=true;
            continue;
        }
        else if(File.startsWith("[HEP]")||File.startsWith("[HELP]")||File.startsWith("[HLP]")){
            QStringList helpList=File.split(" ");
            QLOG_DEBUG()<<"[HLP] Part Parsing";
            if(helpList.length()<2){
                QLOG_DEBUG()<<"[DEBUGABLE_INFORMATION] Parsing "<<File<<" : less than 2 arguments";
                QLOG_DEBUG()<<"[HLP] Missing Arguments";
                emit generateError(tr("Asuls 文件(config.asuls): [HLP]错误"));
                showable=false;
            }
            helpFile=helpList[1];
            QLOG_DEBUG()<<"HelpFileName:"<<helpFile;
            HELP=true;
            continue;
        }
        else if(File.startsWith("[PNG]")){
            QStringList pngList=File.split(" ");
            QLOG_DEBUG()<<"[PNG] Part Parsing";
            if(pngList.length()<2){
                QLOG_DEBUG()<<"[DEBUGABLE_INFORMATION] Parsing "<<File<<" : less than 2 arguments";
                QLOG_DEBUG()<<"[PNG] Missing Arguments";
                emit generateError(tr("Asuls 文件(config.asuls): [PNG]错误"));
            }
            pngFilePath=pngList[1].replace("\r","");
            PNG=true;
            continue;
        }
        else if(File.startsWith("[UPD]")||File.startsWith("[UPDATE]")){
            QStringList updateList=File.split(" ");
            QLOG_DEBUG()<<"[UPD] Part Parsing";
            if(updateList.length()<2){
                QLOG_DEBUG()<<"[DEBUGABLE_INFORMATION] Parsing "<<File<<" : less than 2 arguments";
                QLOG_DEBUG()<<"[INI] Missing Arguments";
                emit generateError(tr("Asuls 文件(config.asuls): [UPD]错误"));
            }
            updateVersion=updateList[1].replace("\r","");
            UPDATE=true;
            continue;
        }
        else if(File.isEmpty()) continue;
        QString fileN=File.replace("\r","").replace("\n","");
        QStringList fileNameAfilePresects=fileN.split(" ");
        QString first=fileN,last;
        if(fileNameAfilePresects.length()==2){
            first=fileNameAfilePresects[0];
            last=fileNameAfilePresects[1];
            fileNames.append(first.append(".asul"));
            filePresects.append(last);
        }
        else{
            fileNames.append(fileN.append(".asul"));
            filePresects.append("");
        }

        if(first.contains("/")){
            QString tplet;
            foreach(QString val,first.split("/")){

                tplet=val;
            }
            //createDir
            QDir mkp(CFGFileLocation);
            mkp.mkpath(first);
            // createDirectories();

            singleNames.append(tplet);
        }else{
            singleNames.append(first);
        }
    }

    DBG(CFGName);DBG(CFGVersion);DBG(CFGAuthor);DBG(CFGDetails);

    //generate Window
    if(!INI||!DIR){
        emit generateError(QString(tr("Asuls 文件(config.asuls): 未找到%1 %2")).arg(!INI?"[INI]":"").arg(!DIR?"[DIR]":""));
        showable=false;
    }

    QLOG_DEBUG()<<"[Generate] pageNum will be "<<fileNames.count();

    ElaPopularCard *CFGBreifInfo = new ElaPopularCard(this);
    ElaWindow *DeployWindow=new ElaWindow(CFGBreifInfo);
    // DeployWindow->setAttribute(Qt::WA_AlwaysStackOnTop);
    DeployWindow->setUserInfoCardPixmap(QPixmap(QString(":/pic/Pic/favicon.png").replace("favicon.png",eTheme->getThemeMode()==ElaThemeType::Light?"favicon_dark.png":"favicon.png")));
    DeployWindow->setNavigationBarDisplayMode(ElaNavigationType::Minimal);
    DeployWindow->setFixedSize(QSize(1000,600));
    // DeployWindow->makeIndependence();
    DeployWindow->setIsNavigationBarEnable(false);
    DeployWindow->setWindowButtonFlags(ElaAppBarType::CloseButtonHint | ElaAppBarType::MaximizeButtonHint | ElaAppBarType::MinimizeButtonHint);
    DeployWindow->setIsDefaultClosed(false);

    connect(DeployWindow,&ElaWindow::closeButtonClicked,[=](){
        bool ask=askDialog(DeployWindow,tr("关闭"),tr("是否退出配置")+" **"+CFGName.simplified()+"** ["+CFGVersion+"]"+"-"+CFGAuthor+" ?");
        if (ask) DeployWindow->close();
    });
    // CFGBreifInfo->setCardPixmap(QPixmap(":/pic/Pic/CS2.png"));
    if(PNG){
        DeployWindow->setUserInfoCardPixmap(QPixmap(CFGFileLocation+"/"+pngFilePath));
    }
    DeployWindow->setUserInfoCardTitle(CFGName);
    // DeployWindow->setUserInfoCardTitle("CFG LinkWard");
    // DeployWindow->setUserInfoCardSubTitle("by Alivn@Asul.top");
    DeployWindow->setUserInfoCardSubTitle("@"+CFGAuthor);
    DeployWindow->setUserInfoCardVisible(true);
    DeployWindow->setWindowTitle(tr("部署 CFG"));

    // DeployWindow->setWindowButtonFlag(ElaAppBarType::ThemeChangeButtonHint,false);
    // DeployWindow->setWindowButtonFlag(ElaAppBarType::StayTopButtonHint,false);

    //generate end


    CFGBreifInfo->setCardPixmap(QPixmap(QString(":/pic/Pic/favicon.png").replace("favicon.png",eTheme->getThemeMode()==ElaThemeType::Light?"favicon_dark.png":"favicon.png")));

    if(PNG){
        CFGBreifInfo->setCardPixmap(QPixmap(CFGFileLocation+"/"+pngFilePath));
        QLOG_DEBUG()<<"[ShowPic] QPixMap"<<CFGFileLocation+"/"+pngFilePath;
        QLOG_DEBUG()<<"[ShowPic] isNull?"<<QPixmap(CFGFileLocation+"/"+pngFilePath).isNull();
    }
    CFGBreifInfo->setTitle(CFGName);
    CFGBreifInfo->setSubTitle("@" + CFGAuthor);
    CFGBreifInfo->setInteractiveTips(tr("队列中"));

    if(ExistDir.contains(CFGFileLocation)){
        CFGBreifInfo->setCardButtontext(tr("管理"));
        CFGBreifInfo->setInteractiveTips(tr("已安装"));
    }
    CFGBreifInfo->setDetailedText(tr("版本:")+CFGVersion+"\n"+CFGDetails);
    CFGBreifInfo->setCardFloatPixmap(QPixmap(QString(":/pic/Pic/favicon.png").replace("favicon.png",eTheme->getThemeMode()==ElaThemeType::Light?"favicon_dark.png":"favicon.png")));

    connect(eTheme,&ElaTheme::themeModeChanged,[=](){
        QPixmap pngLo=QPixmap(QString(":/pic/Pic/favicon.png").replace("favicon.png",eTheme->getThemeMode()==ElaThemeType::Light?"favicon_dark.png":"favicon.png"));
        DeployWindow->setUserInfoCardPixmap(pngLo);
        if(!PNG) CFGBreifInfo->setCardPixmap(pngLo);
        CFGBreifInfo->setCardFloatPixmap(pngLo);
    });
    this->selectedFilePath->setText(ZipFilePath);
    cVLayout->addWidget(CFGBreifInfo);

    connect(CFGBreifInfo, &ElaPopularCard::popularCardButtonClicked, this, [=](){
        DeployWindow->show();
    });

    QRegularExpression regex("(\\w+)|(\"(?:\\\\.|[^\\\\\"])*\")");
    //QRegularExpression regex2("\"((?:\\\\.|[^\"])*)\"\\s*and\\s*\"((?:\\\\.|[^\"])*)\"");
    QRegularExpression regex2("\"((?:[^\"\\\\]|\\\\.)*?)\"");
    QRegularExpression regex3("\"([^\"]+)\"\\s*\"([^\"]+)\"");
    QRegularExpression regex4("\"((?:[^\"\\\\]|\\\\.)*?)\"\\s*\"((?:[^\"\\\\]|\\\\.)*?)\"\\s*\"((?:[^\"\\\\]|\\\\.)*?)\"\\s*\"((?:[^\"\\\\]|\\\\.)*?)\"");
    pageNumMap[CFGBreifInfo]=fileNames.count();
    QQueue<QString> *ElaPageNodeS=new QQueue<QString>;

    dirName=dirName.replace("\r","");
    QFile prevAsuls(QString(CFGPath + "/" + dirName+"/config.asuls"));
    QLOG_DEBUG()<<"[UPDATE_Module] prevAsulsFile:"<<CFGPath+"/"+dirName+"/config.asuls";
    QString prevAsulsContent;
    QString prevUpdateVersion;
    if(prevAsuls.exists())
        if(prevAsuls.open(QIODevice::ReadOnly))
            prevAsulsContent=prevAsuls.readAll();
    if(!prevAsulsContent.isEmpty()){
        foreach(QString File,prevAsulsContent.split("\n")){
            if(File.startsWith("[UPD]")|File.startsWith("[UPDATE]")){
                QStringList updateList=File.split(" ");
                prevUpdateVersion=updateList[1].replace("\r","");
                break;
            }
        }
    }
    bool enableUpdateMode=false;
    QLOG_DEBUG()<<"[UPDATE_Module] UPDATE STATE:"<<UPDATE<<"\nprevUpdateVersion:"<<prevUpdateVersion<<"\nupdateVersion:"<<updateVersion;
    if(UPDATE&&prevUpdateVersion==updateVersion&&!ExistDir.contains(CFGFileLocation)){
        enableUpdateMode=askDialog(tr("更新"),tr("检测到 CFG 目录下有可覆盖的更新 是否启用?"));
    }
    QString prevCFGFileLocation;
    if(enableUpdateMode){
        prevCFGFileLocation=CFGFileLocation;
        CFGFileLocation=CFGPath+"/"+dirName;
        CFGBreifInfo->setTitle(CFGBreifInfo->getTitle()+tr(" [升级模式]"));

    }
    DBG(fileNames);
    foreach (QString asulFile, fileNames) {
        int index=fileNames.indexOf(asulFile);
        QString asulFileName=singleNames[index];
        Collector * sumContent=new Collector(CFGBreifInfo);
        Collector * sumAsul=new Collector(CFGBreifInfo);

        BaseScrollPage *CFGSettingPage=new BaseScrollPage(this);

        CFGSettingPage->initWidget(asulFileName,asulFileName+tr(" 项目"),tr("解析完成 "));
        // ElaText *tips=new ElaText(tr("若键盘映射无所需按键或确认后与所需不符\n请在第二个空中手动输入"));
        // tips->setTextPixelSize(20);
        // tips->hide();
        // QFont fontTip(tips->font());
        // fontTip.setBold(true);
        // tips->setFont(fontTip);
        // CFGSettingPage->addTopWidget(tips);
        CFGSettingPage->setWindowTitle(asulFileName+tr(" 配置"));
        QVBoxLayout *Allkey=new QVBoxLayout();
        QFile * configFile=new QFile(CFGFileLocation+"/"+asulFile);
        QString contents;

        if(configFile->open(QIODevice::ReadOnly)){
            contents=configFile->readAll();
            configFile->close();
        }
        else{
            emit generateError(tr("Asul 文件(")+asulFile+"): "+configFile->errorString());
            // CFGBreifInfo->deleteLater();
            showable=false;
            return;
        }
        configFile->deleteLater();
        QStringList lines = contents.split("\n");
        for (int i = 0; i < lines.size(); ++i) {

            QString arguPure="";
            QString line = lines[i];
            line=line.replace("\r","").replace("\n","");
            if(line.startsWith("key") || line.startsWith("must")){
                QLOG_DEBUG()<<"[KEY|MUST] Part Parsing";
                QStringList result;

                // 匹配第一个不带引号的单词或引号内的字符串

                QRegularExpressionMatchIterator it = regex.globalMatch(line);

                while (it.hasNext()) {
                    QRegularExpressionMatch match = it.next();
                    if (match.hasMatch()) {
                        // 捕获组1：第一个不带引号的单词
                        if (!match.captured(1).isEmpty()) {
                            result.append(match.captured(1));
                        }
                        // 捕获组2：引号内的字符串
                        else if (!match.captured(2).isEmpty()) {
                            result.append(match.captured(2).mid(1, match.captured(2).length() - 2)); // 移除引号
                        }
                    }else{
                        QLOG_DEBUG()<<"[DEBUGABLE_INFORMATION] Parsing "<<line<<" : char["+QString::number(i)+"]";
                        QLOG_DEBUG()<<"[KEY|MUST] Missing Arguments";
                        emit generateError(tr("在 ")+asulFile+tr(" 中")+QString::number(i)+tr("[key] 无法找到完整参数"));
                        showable=false;
                        return;
                    }
                }
                QString type=result[0];
                QString defaultKey=result[1];
                QString func=result[2];
                QString title=result[3];
                QString details=result[4];
                DBG(type);DBG(defaultKey);DBG(func);DBG(title);DBG(details);
                arguPure=type+" \"%1\""+" \""+func+"\" \""+title+"\" \""+details+"\"";
                // QLOG_DEBUG()<<"arguPure"<<arguPure;
                //addButtonToDeployWindow
                ElaScrollPageArea *configArea=new ElaScrollPageArea(DeployWindow);
                QHBoxLayout * configHLayout=new QHBoxLayout(configArea);
                QVBoxLayout* configVLayout=new QVBoxLayout();
                if(line.startsWith("must")) title=title+tr("(根据作者定义此项无法修改)");
                ElaText *titleText=new ElaText(title);
                ElaText *detailText=new ElaText(details.replace("\\n","\n"));
                // trans->translateText(   title,"auto",targetLang);

                QFont font;
                font.setBold(true);
                titleText->setFont(font);
                detailText->setTextPixelSize(10);
                configVLayout->addWidget(titleText);
                configVLayout->addWidget(detailText);

                ElaKeyBinder *configKeyBinder=new ElaKeyBinder(DeployWindow);
                configKeyBinder->setDisabled(line.startsWith("must"));
                configKeyBinder->setBinderKeyText(defaultKey);
                configKeyBinder->setMaximumWidth(200);
                configKeyBinder->setMinimumHeight(MiddleHeight);
                configHLayout->addLayout(configVLayout);
                configHLayout->addWidget(configKeyBinder);
                ElaLineEdit *configKeyLine=new ElaLineEdit(DeployWindow);
                configKeyLine->setDisabled(line.startsWith("must"));
                configKeyLine->setText(defaultKey);
                configHLayout->addWidget(configKeyLine);
                configKeyLine->setMaximumWidth(200);
                configKeyLine->setMinimumHeight(MiddleHeight);
                Allkey->addWidget(configArea);

                Allkey->addSpacing(5);

                int colNum=sumContent->addLabel(detailText);
                sumContent->setValue(colNum,QString("bind %1 %2").arg(defaultKey).arg(func).replace("\\\"","\""));
                sumAsul->addLabel(detailText);
                sumAsul->setValue(colNum,arguPure.arg(defaultKey));
                detailText->setProperty("num",colNum);
                connect(configKeyLine,&ElaLineEdit::textChanged,[=](QString val){
                    configKeyBinder->setBinderKeyText(val);
                    QLOG_DEBUG()<<"get in configKeyLine";
                    if(val=="Up") val="uparrow";
                    else if(val=="Down") val="downarrow";
                    else if(val=="Left") val="leftarrow";
                    else if(val=="Right") val="rightarrow";
                    else if(val=="Return") val="enter";
                    else if(val=="PgDown") val="pgdn";
                    val=val.toLower();

                    int myNum=detailText->property("num").toInt();
                    sumAsul->setValue(myNum,arguPure.arg(val));
                    sumContent->setValue(myNum , QString("bind %1 %2").arg(val).arg(func));

                });
                connect(configKeyBinder,&ElaKeyBinder::binderKeyTextChanged,this,[=](QString val){
                    QLOG_DEBUG()<<"get in binderKeyTextChanged";
                    if(val=="Up") val="uparrow";
                    else if(val=="Down") val="downarrow";
                    else if(val=="Left") val="leftarrow";
                    else if(val=="Right") val="rightarrow";
                    else if(val=="Return") val="enter";
                    else if(val=="PgDown") val="pgdn";
                    val=val.toLower();

                    int myNum=detailText->property("num").toInt();
                    sumContent->setValue(myNum,QString("bind %1 %2\n").arg(val).arg(func));
                    configKeyLine->setText(val);
                });

                //checkForNonRepetit


            }
            else if(line.startsWith("func")){
                QStringList result;
                QLOG_DEBUG()<<"[FUNC] Part Parsing";
                // 匹配第一个不带引号的单词或引号内的字符串

                QRegularExpressionMatchIterator it = regex.globalMatch(line);

                while (it.hasNext()) {
                    QRegularExpressionMatch match = it.next();
                    if (match.hasMatch()) {
                        // 捕获组1：第一个不带引号的单词
                        if (!match.captured(1).isEmpty()) {
                            result.append(match.captured(1));
                        }
                        // 捕获组2：引号内的字符串
                        else if (!match.captured(2).isEmpty()) {
                            result.append(match.captured(2).mid(1, match.captured(2).length() - 2)); // 移除引号
                        }
                    }
                    else{
                        QLOG_DEBUG()<<"[DEBUGABLE_INFORMATION] Parsing "<<line<<" : char["+QString::number(i)+"]";
                        QLOG_DEBUG()<<"[FUNC] Missing Arguments";
                        emit generateError("在 "+asulFile+" 中"+QString::number(i)+":func无法找到完整参数");
                        showable=false;
                        return;
                    }
                }
                //func "默认输出方式" "某些调试信息将会被输出" "HorizonMessage_team" 4
                // "HorizonMessage_team" "输出至队伍聊天"
                // "HorizonMessage_all" "输出至全局聊天"
                // "HorizonMessage_console" "输出至控制台"
                // "HorizonMessage_off" "输出关闭"
                QString title = result[1];
                QString details = result[2];
                QString defaultSwitch = result[3];
                DBG(title);DBG(details);DBG(defaultSwitch);
                int switchNums=result[4].toInt();
                arguPure=result[0]+" \""+title+"\" \""+details+"\" \"%1\" "+QString::number(switchNums);
                QStringList switchContent,switchDetail;
                for(int j=1;j<switchNums+1;j++){
                    int index=i+j;
                    arguPure+="\n"+lines[index];
                    if(lines[index].startsWith("func")||lines[index].startsWith("text")||lines[index].startsWith("key")){
                        emit generateError("在 "+asulFile+" 中"+QString::number(i)+":func参数过多 当前参数:"+QString::number(switchNums));
                        showable=false;
                        return;
                    }
                    // switchContent.append(lines[index].split(" ")[0].replace("\"",""));
                    // switchDetail.append(lines[index].split(" ")[1].replace("\"",""));
                    QRegularExpressionMatch match = regex3.match(lines[index]);
                    if(match.hasMatch()) {
                        QString key = match.captured(1);   // 第一个引号中的内容 (如 HorizonMessage_team)
                        QString value = match.captured(2);  // 第二个引号中的内容 (如 输出至队伍聊天)
                        // QLOG_DEBUG() << "键:" << key << "值:" << value;
                        switchContent.append(key);
                        switchDetail.append(value);
                    }
                }
                ElaScrollPageArea *configArea=new ElaScrollPageArea(DeployWindow);
                QHBoxLayout * configHLayout=new QHBoxLayout(configArea);
                QVBoxLayout* configVLayout=new QVBoxLayout();
                ElaText *titleText=new ElaText(title);
                ElaText *detailText=new ElaText(details.replace("\\n","\n"));
                QFont font;
                font.setBold(true);
                titleText->setFont(font);
                detailText->setTextPixelSize(10);
                configVLayout->addWidget(titleText);
                configVLayout->addWidget(detailText);
                configHLayout->addLayout(configVLayout);
                BanElaComboBox *configComboBox=new BanElaComboBox(DeployWindow);
                configComboBox->addItems(switchDetail);
                // configComboBox->
                int defaultCount=0;
                for (int i = 0; i < switchContent.count(); ++i) {
                    if (switchContent[i] == defaultSwitch) {
                        defaultCount=i;
                        break;
                    }
                }
                configComboBox->setCurrentText(switchDetail[defaultCount]);
                configComboBox->setMinimumHeight(MiddleHeight);
                configComboBox->setMaximumWidth(300);
                configHLayout->addWidget(configComboBox);
                Allkey->addWidget(configArea);
                Allkey->addSpacing(5);

                int colNum=sumContent->addLabel(detailText);
                detailText->setProperty("num",colNum);
                sumContent->setValue(colNum,defaultSwitch);
                sumAsul->addLabel(detailText);
                sumAsul->setValue(colNum,arguPure.arg(defaultSwitch));

                connect(configComboBox,&BanElaComboBox::currentIndexChanged,this,[=](int index) mutable{
                    // const_cast<QString&>(sumContent[pageNum][myNumber])=switchContent[index];
                    // QLOG_DEBUG()<<"get in ";
                    int myNum=detailText->property("num").toInt();
                    // QLOG_DEBUG()<<"mynum"<<myNum;
                    QLOG_DEBUG()<<"[Deploy] "<<title<<" Change to "<<switchContent[index];
                    sumContent->setValue(myNum,switchContent[index]);
                    sumAsul->setValue(myNum,arguPure.arg(switchContent[index]));
                });
            }
            else if(line.startsWith("text")){
                QLOG_DEBUG()<<"[TEXT] Part Parsing";
                DBG(line);
                arguPure=line;
                QRegularExpressionMatch match = regex2.match(line);
                if(match.hasMatch()) {
                    QString quotedContent = match.captured(1);  // 获取第一个捕获组(引号内的内容)
                    ElaText *tplat=new ElaText(quotedContent);
                    tplat->hide();
                    sumContent->addLabel(tplat);
                    sumAsul->setValue(sumAsul->addLabel(tplat),line);

                }else{
                    QLOG_DEBUG()<<"[DEBUGABLE_INFORMATION] Parsing "<<line<<" : char["+QString::number(i)+"]";
                    QLOG_DEBUG()<<"[TEXT] Missing Arguments";
                    emit generateError("在 "+asulFile+" 中"+QString::number(i)+":text无法找到完整参数");
                    showable=false;
                    return;
                }
            }
            else if(line.startsWith("line")){
                QLOG_DEBUG()<<"[LINE] Part Parsing";
                QString title;
                QString detail;
                QString A;
                QString B;

                QRegularExpressionMatch match = regex4.match(line);
                if(match.hasMatch()) {
                    title = match.captured(1);   // 第一个引号中的内容 (如 title)
                    detail = match.captured(2);  // 第二个引号中的内容 (如 detail)
                    A = match.captured(3);       // 第三个引号中的内容 (如 A)
                    B = match.captured(4);       // 第四个引号中的内容 (如 B)

                    QLOG_DEBUG() << "标题:" << title << "详情:" << detail << "A:" << A << "B:" << B;
                }else{
                    QLOG_DEBUG()<<"[DEBUGABLE_INFORMATION] Parsing "<<line<<" : char["+QString::number(i)+"]";
                    QLOG_DEBUG()<<"[LINE] Missing Arguments";
                    emit generateError("在 "+asulFile+" 中"+QString::number(i)+":line无法找到完整参数");
                    showable=false;
                    return;
                }
                arguPure=QString("line \""+title+"\" \""+detail+"\" \""+A+"\" ").replace("%1","%WAITING_REPLACE%")+"\"%1\"";
                ElaScrollPageArea *configArea=new ElaScrollPageArea(DeployWindow);
                QHBoxLayout * configHLayout=new QHBoxLayout(configArea);
                QVBoxLayout* configVLayout=new QVBoxLayout();
                ElaText *titleText=new ElaText(title);
                ElaText *detailText=new ElaText(detail.replace("\\n","\n"));
                QFont font;
                font.setBold(true);
                titleText->setFont(font);
                detailText->setTextPixelSize(10);
                configVLayout->addWidget(titleText);
                configVLayout->addWidget(detailText);
                configHLayout->addLayout(configVLayout);
                ElaLineEdit *configLine=new ElaLineEdit(DeployWindow);
                configLine->setText(B);
                configLine->setMinimumHeight(MiddleHeight);
                configLine->setMaximumWidth(300);
                int colNum=sumContent->addLabel(detailText);
                sumAsul->addLabel(detailText);
                detailText->setProperty("num",colNum);

                sumContent->setValue(colNum,A.arg(B));
                sumAsul->setValue(colNum,arguPure.arg(B).replace("%WAITING_REPLACE%","\%1"));
                connect(configLine,&ElaLineEdit::textChanged,[=](QString val){
                    int myNum=detailText->property("num").toInt();
                    sumContent->setValue(myNum,A.arg(val));
                    sumAsul->setValue(myNum,arguPure.arg(val).replace("%WAITING_REPLACE%","\%1"));

                });

                configHLayout->addWidget(configLine);

                Allkey->addWidget(configArea);
                Allkey->addSpacing(5);
            }
        }

        QLOG_DEBUG()<<"[GENERATE]: "<<asulFileName<<" End ";
        QString CFGFileName=filePresects[index]+singleNames[index].replace("\u0000","").replace(".asul",".cfg");
        ElaText * previewText=new ElaText(QString(tr("预览 %1 文件")).arg(asulFileName.replace(".asul",".cfg")));
        ElaText * preview2Text=new ElaText(QString(tr("预览 %1 文件")).arg(asulFileName));

        previewText->setTextPixelSize(25);
        preview2Text->setTextPixelSize(25);
        QVBoxLayout * previewLayout=new QVBoxLayout();
        ElaPlainTextEdit * previewPlainText=new ElaPlainTextEdit(sumContent->getValues().join("\n").replace("\\\"","\""),DeployWindow);
        ElaPlainTextEdit * preview2PlainText=new ElaPlainTextEdit(sumAsul->getValues().join("\n"),DeployWindow);
        previewPlainText->setMinimumHeight(400);
        preview2PlainText->setMinimumHeight(400);
        connect(sumAsul,&Collector::valuesChanged,[=](){
            preview2PlainText->setPlainText(sumAsul->getValues().join("\n"));
        });
        connect(sumContent,&Collector::valuesChanged,[=](){
            previewPlainText->setPlainText(sumContent->getValues().join("\n").replace("\\\"","\""));
        });
        previewPlainText->setReadOnly(true);
        preview2PlainText->setReadOnly(true);
        previewLayout->addWidget(previewText);
        previewLayout->addWidget(previewPlainText);
        previewLayout->addWidget(preview2Text);
        previewLayout->addWidget(preview2PlainText);
        QWidget *previewWidget=new QWidget(DeployWindow);
        previewWidget->setLayout(previewLayout);
        CFGSettingPage->centerVLayout->addLayout(Allkey,10);
        CFGSettingPage->centerVLayout->addWidget(previewWidget);
        if(!gSettings->getPreviewShow()){
            previewWidget->hide();
        }
        connect(gSettings,&GlobalSettings::pPreviewShowChanged,[=](){
            gSettings->getPreviewShow()?previewWidget->show():previewWidget->hide();
        });
        ElaPushButton *deployButton=new ElaPushButton(tr("部署"),DeployWindow);
        deployButton->setMinimumHeight(HugeHeight);
        deployButton->setProperty("CFGFileLocation",CFGFileLocation);
        deployButton->setProperty("CFGFileName",CFGFileName);
        deployButton->setProperty("CFGAuthor",CFGAuthor);
        deployButton->setProperty("CFGName",CFGName);
        deployButton->setProperty("CFGVersion",CFGVersion);
        deployButton->setProperty("CFGDirName",dirName);
        deployButton->setProperty("exeFile",exeFile);
        deployButton->setProperty("helpFile",helpFile);
        deployButton->setProperty("EXE",EXE);
        deployButton->setProperty("HELP",HELP);
        deployButton->setProperty("AsulFileName",asulFileName.replace(".cfg",".asul"));
        deployButton->setProperty("prevCFGFileLocation",prevCFGFileLocation);

        connect(deployButton,&ElaPushButton::clicked,[=](){
            QString CFGFileName=deployButton->property("CFGFileName").toString();
            QString CFGFileLocation=deployButton->property("CFGFileLocation").toString();
            QString CFGAuthor=deployButton->property("CFGAuthor").toString();
            QString CFGName=deployButton->property("CFGName").toString();
            QString CFGVersion=deployButton->property("CFGVersion").toString();
            QString CFGHelpFile=deployButton->property("helpFile").toString().replace("\r","");
            QString AsulFileName=deployButton->property("AsulFileName").toString();


            ElaContentDialog *dialog=new ElaContentDialog(DeployWindow);
            dialog->setLeftButtonText(tr("返回"));
            dialog->setMiddleButtonText(tr("取消"));
            dialog->setRightButtonText(tr("确认"));

            CFGFileName.append(".cfg");

            connect(dialog,&ElaContentDialog::rightButtonClicked,this,[=](){
                QString prevCFGFileLocation=deployButton->property("prevCFGFileLocation").toString();
                QString CFGDirName=deployButton->property("CFGDirName").toString().replace("\r","");
                QString CFGExeFile=deployButton->property("exeFile").toString().replace("\r","");
                QString CFGFileLocation=deployButton->property("CFGFileLocation").toString();




                if(enableUpdateMode) CFGFileLocation=prevCFGFileLocation;

                DBG(CFGFileName);DBG(CFGFileLocation);DBG(CFGDirName);DBG(CFGPath);DBG(AsulFileName);DBG(asulFile);
                QLOG_DEBUG()<<"[Deploy] write to "<<CFGFileLocation+"/"+AsulFileName+".asul";

                QFile *preferrenceFile=new QFile(CFGFileLocation+"/"+CFGFileName);
                QFile *asulFi=new QFile(CFGFileLocation+"/"+AsulFileName+".asul");
                if(asulFi->exists()){
                    if (!asulFi->remove()) {
                        qWarning() << "[Deploy] Can't delete file:" << asulFi->fileName();
                        return;
                    }
                }
                if (preferrenceFile->exists()) {
                    // 删除文件
                    if (!preferrenceFile->remove()) {
                        qWarning() << "[Deploy] Can't delete file:" << preferrenceFile->fileName();
                        return;
                    }
                }

                if(!preferrenceFile->open(QIODevice::ReadWrite)){
                    QLOG_DEBUG()<<"[Deploy] Error has occured"<<CFGFileLocation+"/"+CFGFileName;
                }
                if(!asulFi->open(QIODevice::ReadWrite)){
                    QLOG_DEBUG()<<"[Deploy] Error has occured"<< CFGFileLocation+"/"+AsulFileName;
                }
                QFileInfo *CFGFileInfo=new QFileInfo(CFGFileLocation+"/"+CFGFileName);
                QFileInfo *AsulFileInfo=new QFileInfo(CFGFileLocation+"/"+AsulFileName);

                QTextStream *preferrenceStream=new QTextStream(preferrenceFile);
                QTextStream *asulStream=new QTextStream(asulFi);
                *asulStream<<sumAsul->getValues().join("\n");
                asulFi->close();
                delete asulStream;

                QLOG_DEBUG()<<"[Deploy] Ready to generate file: "<<CFGFileLocation+"/"+CFGFileName;
                QLOG_DEBUG()<<"[Deploy] ALL INFORMATION COLLECT TABLE:";
                DBG(CFGFileLocation);DBG(CFGFileName);DBG(CFGAuthor);DBG(CFGName);DBG(CFGVersion);
                DBG(dirName);DBG(exeFile);DBG(helpFile);DBG(EXE);DBG(HELP);DBG(asulFileName)

                *preferrenceStream<<tr("// ====== 生成的文件 =======")+"\n";
                *preferrenceStream<<QString(tr("//=这个 %1 文件由 Asul-CFGManager(AM) 根据配置自动生成 ")+"\n").arg(CFGFileInfo->fileName());
                *preferrenceStream<<tr("\n//==这个 配置文件 从哪儿来的?")+"\n";
                *preferrenceStream<<tr("//CFG 制作者: ")<<CFGAuthor<<"\n";
                *preferrenceStream<<tr("//CFG 名称: ")<<CFGName<<"\n";
                *preferrenceStream<<tr("//CFG 版本: ")<<CFGVersion<<"\n";
                *preferrenceStream<<tr("//==CFG 详细 结束")+"\n\n";

                foreach (QString val, sumContent->getValues()) {
                    *preferrenceStream<<replaceArgs(val.replace("\\\"","\""))<<"\n";
                }

                *preferrenceStream<<"\n\n"+tr("//==参数结束")+"\n";
                *preferrenceStream<<tr("//AM 是由 Alivn开发的部署 CS2 CFG 的程序,旨在为CFG制作者提供更方便的分发服务 以及 使用者提供方便的配置服务")+"\n";
                *preferrenceStream<<tr("//开发者:Github(https://github.com/AsulTop),网站(http://www.asul.top)")+"\n";
                *preferrenceStream<<tr("//配置时间: ")<<QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")+"\n\n";

                preferrenceFile->close();
                delete preferrenceStream;

                CFGBreifInfo->hide();
                CFGSettingPage->hide();

                // CFGBreifInfo->deleteLater();
                // CFGSettingPage->deleteLater();

                // parentWindow->succTips(DeployWindow,"CFG","一项配置完成部署");
                // QDesktopServices::openUrl(CFGFileLocation+"/"+CFGFileName);

                pageNumMap[CFGBreifInfo]--;
                QLOG_DEBUG()<<"[Deploy] then page num:"<<pageNumMap[CFGBreifInfo];
                if(pageNumMap[CFGBreifInfo]==0){
                    //配置结束：转移文件夹到CFGFile下
                    QLOG_DEBUG()<<"Page==0!";
                    if(gSettings->getAutoDir())
                        QDesktopServices::openUrl(QUrl::fromLocalFile(CFGPath+"/"+CFGDirName));
                    if(HELP&&gSettings->getAutoHelp()){
                        QLOG_DEBUG()<<"[Deploy] getReady CFGHelpFile"<<QString::fromUtf8(CFGHelpFile.toUtf8());
                        QDesktopServices::openUrl(QUrl::fromLocalFile(CFGPath+"/"+CFGDirName+"/"+CFGHelpFile));
                    }
                    QString REF="";
                    if(!ExistDir.contains(CFGFileLocation))
                        REF=manageDirectory(CFGPath,CFGDirName.replace("\r",""),CFGFileLocation);

                    if(REF!=""){
                        // parentWindow->warnTips(parentWindow,"配置出错","在复制"+REF+"文件中出错");

                        CFGBreifInfo->hide();
                        DeployWindow->setAttribute(Qt::WA_DeleteOnClose);
                        DeployWindow->close();
                        DeployWindow->deleteLater();

                        return;
                    }
                    if(!exeFile.isEmpty()){
                        if(EXE&&gSettings->getAutoExe()){
                            QLOG_DEBUG()<<"[Deploy_EXE] 将运行:"<<exeFile<<" at "<<CFGPath+"/"+CFGDirName+"/"+exeFile;
                            // ElaMessageBar::information(ElaMessageBarType::Top,"运行","运行 "+CFGPath+"/"+CFGDirName+"/"+exeFile,0x3f3f3f3f);
                            QString execFinalFile=CFGPath+"/"+CFGDirName+"/"+exeFile;
                            QProcess *process =new QProcess();
                            QFileInfo fileInfo(execFinalFile);
                            QFile *AutoInstall=new QFile(CFGFileLocation+"/"+"AUTOINSTALL.bat");
                            if(!AutoInstall->open(QIODevice::ReadWrite)){
                                parentWindow->warnTips(DeployWindow,tr("错误"),tr("AUTOINSTALL.bat 创建失败"));
                            }
                            QTextStream *AutoStream=new QTextStream(AutoInstall);
                            *AutoStream<<"cd "<<fileInfo.absolutePath()<<"\n";
                            *AutoStream<<fileInfo.absolutePath().split("/")[0]<<"\n";
                            *AutoStream<<"start \"\" "<<fileInfo.fileName()<<"\n";
                            // *AutoStream<<"exit"<<"\n";
                            AutoInstall->close();

                            process->start(CFGFileLocation+"/AUTOINSTALL.bat");
                            QLOG_DEBUG()<<"[Process] ===";
                            if (!process->waitForFinished()) {
                                QLOG_DEBUG()<<"Error() "<<process->readAllStandardError();
                                QLOG_DEBUG()<<"Output() "<<process->readAllStandardOutput();
                                qWarning() << "无法运行文件:" << fileInfo.absoluteFilePath()<<process->errorString();
                                parentWindow->warnTips(parentWindow,"[EXE]",tr("自动")+exeFile+tr("运行失败，请查看帮助文档"));
                            }
                            connect(process,&QProcess::finished,[=](){
                                process->deleteLater();
                            });
                        }

                        delete CFGFileInfo;
                    }

                    QFile autoExecFile(CFGPath+"/"+"autoexec.cfg");
                    if(!autoExecFile.exists()){
                        bool createAutoExecFile=askDialog(tr("文件"),tr("没有存在的 AutoExec.cfg 文件 是否创建?")+"\n"+tr("(不创建将导致管理失效!)"));
                        if(createAutoExecFile){
                            autoExecFile.open(QIODevice::ReadWrite);
                            autoExecFile.close();
                        }
                    }
                    //配置段结束
                    DeployWindow->close();
                    DeployWindow->deleteLater();
                    delete ElaPageNodeS;

                }else{
                    DeployWindow->navigation(ElaPageNodeS->dequeue());
                    // ElaNavigationRouter::getInstance()->navigationRouteBack();
                    QString buffer=tr("配置中还有 ")+QString::number(pageNumMap[CFGBreifInfo])+tr(" 个");
                    // parentWindow->succTips(DeployWindow,tr("完成"),buffer);
                    ElaMessageBar::success(ElaMessageBarType::Top,tr("完成"),buffer,getReadTime(buffer),DeployWindow);
                }
            });
            QWidget *dialogWidget=new QWidget(DeployWindow);
            ElaLineEdit *dialogLine=new ElaLineEdit(DeployWindow);
            ElaText *dialogText=new ElaText(tr("部署"),DeployWindow);
            dialogText->setTextPixelSize(MainFont);
            QVBoxLayout *dialoglayout=new QVBoxLayout(dialogWidget);
            dialogWidget->setLayout(dialoglayout);
            dialogLine->setMinimumHeight(MiddleHeight);
            dialogLine->setReadOnly(true);
            dialogLine->setText(tr("确认配置?"));
            dialoglayout->addSpacing(topSpacing);
            dialoglayout->addWidget(dialogText);
            dialoglayout->addSpacing(normSpacing);
            dialoglayout->addWidget(dialogLine);
            dialog->setCentralWidget(dialogWidget);
            dialog->exec();
        });
        // CFGSettingPage->addTopWidget(deployButton);
        if(!showable){
            CFGBreifInfo->hide();
        }
        CFGSettingPage->centerVLayout->addWidget(deployButton);


        // QString pageKey;
        DeployWindow->addPageNode(asulFile,CFGSettingPage,ElaIconType::ArrowDownToBracket);
        DeployWindow->navigation(CFGSettingPage->property("ElaPageKey").toString());
        ElaPageNodeS->enqueue(CFGSettingPage->property("ElaPageKey").toString());
    }
    eApp->setParent(parent());
    DeployWindow->clearFocus();
    DeployWindow->setProperty("ElaBaseClassName","FakeElaWindow");
    // parentWindow->setFocus();
}


QString T_Deploy::getPath(QString vdfFile){
    QFile file(vdfFile);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString buffer(file.readAll());
    QString path;
    QStringList reader = buffer.split("\n");
    foreach(QString var,reader){
        if(var.contains("path")){
            path=var.split("\"")[3];
        }
        if(var.contains("730")) break;
    }

    //path -> steamapps -> common -> Counter-Strike Global Offensive ->game ->csgo -> cfg
    path=path.replace("\\\\","\\");
    path = path+"\\steamapps\\common\\Counter-Strike Global Offensive\\game\\csgo\\cfg";
    path.replace("\\","/");
    return path;
}
void T_Deploy::getAllPath(){
    QCoreApplication::processEvents(QEventLoop::AllEvents,5);
    QString regPath = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\steam\\Shell\\Open\\Command";
    QSettings *reg=new QSettings (regPath,QSettings::NativeFormat);
    steamPath = reg->value(".").toString();
    steamPath =QString(steamPath.split("\"")[1])+QString("\\..\\");
    //获取到cfg目录
    gSettings->setSteamUserPath(QString(steamPath).replace("\\","/").replace("steam.exe/..","userdata"));
    gSettings->setSteamConfPath(QString(steamPath).replace("\\","/").replace("steam.exe/..","config"));
    QString libraryFoldersFile = steamPath + "/steamapps/libraryfolders.vdf";
    if(steamPath.isEmpty()){
        return;
    }
    CFGPath = getPath(libraryFoldersFile);
    //获取完美目录
    // 方案1
    // QProcess execBat;
    // execBat.start("cmd.exe",QString("/c %1/getPerfectPath.bat").arg(gLocation->path()).split(" "));
    // QLOG_DEBUG()<<"ZipFilePath : "<<gLocation->path();
    // execBat.waitForFinished();

    // perfectPath=execBat.readAllStandardOutput();

    // bool perfectOnProcess=false;
    // foreach(QString var,perfectPath.split("\n")){
    //     if(var.contains("perfectworldarena")) perfectPath=var,perfectOnProcess=true;
    // }
    // QLOG_DEBUG()<<"perfectPath:"<<perfectPath;
    // if(perfectOnProcess){
    //     // ui->perfectPathLine->setText();
    //     QString beforePath="";
    //     perfectPath=perfectPath.replace("\\","/");
    //     // QLOG_DEBUG()<<"perfectPath in perfectonProcess:"<<perfectPath;
    //     QStringList perfectPathSplitList=perfectPath.split("/");
    //     for(int i=0;i<perfectPathSplitList.length()-1;i++){
    //         beforePath.append(perfectPathSplitList[i]);
    //         beforePath.append("/");
    //     }
    //     beforePath=beforePath+"plugin/resource";
    //     perfectPath=beforePath;
    // }else{
    //     perfectPath="";
    //     parentWindow->warnTips(parentWindow,"完美平台","未启动，无法自动获取完美平台地址");
    // }
    // 方案2
    QString processName = "完美世界竞技平台.exe"; // 替换为你要查找的进程名称
    perfectPath = getProcessPath(processName);
    if (!perfectPath.isEmpty()) {
        // ui->perfectPathLine->setText();
        QString beforePath="";
        perfectPath=perfectPath.replace("\\","/");
        // QLOG_DEBUG()<<"perfectPath in perfectonProcess:"<<perfectPath;
        QStringList perfectPathSplitList=perfectPath.split("/");
        for(int i=0;i<perfectPathSplitList.length()-1;i++){
            beforePath.append(perfectPathSplitList[i]);
            beforePath.append("/");
        }
        beforePath=beforePath+"plugin/resource";
        perfectPath=beforePath;
        QLOG_DEBUG() << "[GetPath] perfectArea_Platform location : " << perfectPath;
    } else {
        QLOG_DEBUG() << "[GetPath] The process is not running";
        // parentWindow->warnTips(parentWindow,"完美平台","未启动，无法自动获取完美平台地址");
    }

}
void T_Deploy::createDirectories(const QString &path) {
    QDir dir;
    QStringList pathParts = path.split('/');

    // Start from the root and create each directory in the path
    for (int i = 0; i < pathParts.size(); ++i) {
        QString currentPath = pathParts.mid(0, i + 1).join('/');
        if (!dir.exists(currentPath)) {
            dir.mkpath(currentPath);
        }
    }
}
QString T_Deploy::manageDirectory(const QString &CFGPath, const QString &CFGDirName, const QString &CFGFileLocation) {
    QDir targetDir(CFGPath);
    QString targetDirPath = targetDir.filePath(CFGDirName);

    // 检查目标路径是否存在
    if (!targetDir.exists()) {
        // qWarning() << "目标路径不存在:" << CFGPath;
        return "目标路径不存在:";
    }

    // 检查目标路径中是否存在指定的文件夹
    if (targetDir.exists(CFGDirName)) {
        // 删除该文件夹
        QDir dirToRemove(targetDirPath);
        dirToRemove.removeRecursively();
    }

    // 创建新的文件夹
    if (!targetDir.mkdir(CFGDirName)) {
        // qWarning() << "无法创建文件夹:" << targetDirPath;
        return "无法创建文件夹:";
    }

    // 复制源文件夹中的所有文件到目标文件夹

    return copyRecursively(CFGFileLocation, targetDirPath);
}
QString T_Deploy::copyRecursively(const QString &sourcePath, const QString &targetPath) {
    QDir sourceDir(sourcePath);
    if (!sourceDir.exists()) {
        qWarning() << "源文件夹不存在:" << sourcePath;
        return QString("源文件夹不存在:"+sourcePath);

    }

    QDir targetDir(targetPath);
    if (!targetDir.exists()) {
        targetDir.mkpath(targetPath);
    }
    QString errorList="";
    foreach (QString fileName, sourceDir.entryList(QDir::Files)) {
        QString sourceFilePath = sourceDir.filePath(fileName);
        QString targetFilePath = targetDir.filePath(fileName);
        if (!QFile::copy(sourceFilePath, targetFilePath)) {
            qWarning() << "无法复制文件:" << sourceFilePath << "到" << targetFilePath;
            errorList.append(sourceFilePath+" ");
        }
    }

    foreach (QString dirName, sourceDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString sourceDirPath = sourceDir.filePath(dirName);
        QString targetDirPath = targetDir.filePath(dirName);
        errorList.append(copyRecursively(sourceDirPath, targetDirPath)+"");
    }
    return errorList;
}
QString T_Deploy::getProcessPath(const QString &processName) {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return "";
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcessSnap, &pe32)) {
        CloseHandle(hProcessSnap);
        return "";
    }

    do {
        if (QString::fromWCharArray(pe32.szExeFile) == processName) {
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
            if (hProcess) {
                TCHAR buffer[MAX_PATH];
                if (GetModuleFileNameEx(hProcess, NULL, buffer, MAX_PATH)) {
                    CloseHandle(hProcess);
                    CloseHandle(hProcessSnap);
                    return QString::fromWCharArray(buffer);
                }
                CloseHandle(hProcess);
            }
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return "";
}
QString T_Deploy::replaceArgs(QString val){
    return val.replace("%CPU_VENDOR%",CPUVendor)
        .replace("%CPU_NAME%",CPUName)
        .replace("%GPU_VENDOR%",GPUVendor)
        .replace("%GPU_NAME%",GPUName)
        ;
}
T_Deploy::~T_Deploy(){

}
