#include "AsulCFGManager.h"
#include "Headers/BaseInclude.h"
#include "Headers/T_Home.h"
#include "Headers/T_Home2.h"
#include "Headers/T_Manage.h"
#include "Headers/F_SystemInfo.h"
#include "Headers/T_CFGSettings.h"
#include "qapplication.h"
#include <QStandardPaths>
#include <ElaApplication.h>
#include <QDesktopServices>
#include <ElaContentDialog.h>
#include <ElaMessageBar.h>
#include <windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

#include "Headers/GlobalSettings.h"
#include <ElaLog.h>
#include <QsLog.h>
using namespace QsLogging;


#define DBG(x) QLOG_DEBUG()<<"[Key] "<<#x<<" : "<<x;
Asul::Asul(QWidget *parent)
    : AsulWindow(parent)
{
    gSettings->setAutoDir(true);
    gSettings->setAutoExe(true);
    gSettings->setAutoHelp(true);
    gSettings->setPreviewShow(false);
    gSettings->setCFGPath("");
    gSettings->setPerfectPath("");
    gSettings->setSteamConfPath("");

    this->getAllPath();

    QSettings setting("HKEY_CURRENT_USER\\Software\\Asul\\AM",QSettings::NativeFormat);
    gSettings->setSettings(&setting);

    readPolicy=setting.value("ReadPolicy").toBool();
    hwInfo=setting.value("ReadHWInfo").toBool();
    logMode=setting.value("LogMode").toBool();
    if(logMode){
        // ElaLog::getInstance()->initMessageLog(true);
        // ElaLog::getInstance()->setIsLogFileNameWithTime(true);
        // ElaLog::getInstance()->setLogSavePath(QApplication::applicationDirPath());
        // ElaLog::getInstance()->setLogFileName("AsulCFGManager");
        Logger& logger= Logger::instance();
        const QString sLogPath(QDir(QCoreApplication::applicationDirPath()).filePath("AsulCFGManager_log.txt"));
        DestinationPtr fileDestination(DestinationFactory::MakeFileDestination(
            sLogPath, EnableLogRotation, MaxSizeBytes(512*1024), MaxOldLogCount(5)));
        DestinationPtr debugDestination(DestinationFactory::MakeDebugOutputDestination());
        // DestinationPtr sigsSlotDestination(DestinationFactory::MakeFunctorDestination(this, SLOT(logSlot(QString,int))));
        logger.setIncludeLogLevel(true);
        logger.setIncludeTimestamp(true);
        logger.setLoggingLevel(QsLogging::TraceLevel);
        logger.addDestination(debugDestination);
        logger.addDestination(fileDestination);
    }
    if(!readPolicy){
        ElaContentDialog *askWatch=new ElaContentDialog(this);
        QWidget * askWatchWidget=new QWidget(this);
        ElaText *askText=new ElaText(tr("查看"),this);
        ElaLineEdit *askLine =new ElaLineEdit(this);
        QVBoxLayout *askLayout=new QVBoxLayout();
        askLayout->addSpacing(5);
        askLayout->addWidget(askText,40);
        askLayout->addSpacing(10);
        askLayout->addWidget(askLine,60);
        askWatchWidget->setLayout(askLayout);
        askLine->setMinimumHeight(45);
        askLine->setReadOnly(true);
        askLine->setText(tr("请允许我们查看 AutoExec.cfg 文件 ","查看文件"));
        askText->setTextPointSize(20);
        askWatch->setRightButtonText(tr("同意","同意"));
        askWatch->setMiddleButtonText(tr("不同意","不同意"));
        askWatch->setLeftButtonText(tr("忽略","忽略"));

        connect(askWatch,&ElaContentDialog::rightButtonClicked,[=]{
            QSettings settingRg("HKEY_CURRENT_USER\\Software\\Asul\\AM",QSettings::NativeFormat);
            settingRg.setValue("ReadPolicy",true);
            readPolicy=true;
            askWatch->close();
        });
        connect(askWatch,&ElaContentDialog::middleButtonClicked,[=]{
            askWatch->close();
        });
        connect(askWatch,&ElaContentDialog::leftButtonClicked,[=](){
            askWatch->close();
        });

        askWatch->setCentralWidget(askWatchWidget);
        askWatch->exec();
    }
    if(!hwInfo){
        hwInfo = BaseScrollPage::askDialog(this,tr("获取","Get"),tr("允许我们获取设备信息","GetHW"));
        setting.setValue("ReadHWInfo",hwInfo);
    }

    this->setUserInfoCardPixmap(QPixmap(QString(":/pic/Pic/favicon.png").replace("favicon.png",eTheme->getThemeMode()==ElaThemeType::Light?"favicon_dark.png":"favicon.png")));
    connect(eTheme,&ElaTheme::themeModeChanged,[=](){
        this->setUserInfoCardPixmap(QPixmap(QString(":/pic/Pic/favicon.png").replace("favicon.png",eTheme->getThemeMode()==ElaThemeType::Light?"favicon_dark.png":"favicon.png")));
    });

    this->setUserInfoCardTitle("CFG LinkWard");
    this->setUserInfoCardSubTitle("by Alivn@Asul.top");
    this->setUserInfoCardVisible(true);
    this->setWindowTitle("AsulCFGManager");
    this->setWindowButtonFlag(ElaAppBarType::StayTopButtonHint,false);
    this->setIsDefaultClosed(false);
    this->setAcceptDrops(true);
    this->update();
    this->setNavigationBarDisplayMode(ElaNavigationType::Compact);



    gLocation=new QTemporaryDir();
    GlobalLocation=gLocation->path();
    gSettings->setGLocation(gLocation);

    // Use For Debug...
    // QDesktopServices::openUrl(gLocation->path());
    // QDesktopServices::openUrl(QApplication::applicationDirPath());
    //

    saveResourceFile("/Tools","Tools/7z.exe",GlobalLocation+"/7z.exe");
    saveResourceFile("/Tools","Tools/7z.dll",GlobalLocation+"/7z.dll");
    saveResourceFile("/Tools","Tools/getPerfectPath.bat",GlobalLocation+"/getPerfectPath.bat");

    // saveResourceFile("/Tools","Tools/scfg.exe",GlobalLocation+"/scfg.exe");  //不提供
    // saveResourceFile("/Tools","Tools/lua54.dll",GlobalLocation+"/lua54.dll"); //不提供
    // normTips("located",GlobalLocation->path());
    //SaveFile End

    auto CaptureScreenShotAndEnableMica=[=](){
        QScreen *screen = QGuiApplication::primaryScreen();
        QString filePath = GlobalLocation+"/mica.png";
        if (screen) {
            // 截取全屏幕图像
            QPixmap screenshot = screen->grabWindow(0);

            // 设置截图保存的文件名

            // 保存截图到文件
            if (screenshot.save(filePath, "PNG")) {
                QLOG_DEBUG() << "[Mica] Screenshot saved as" << filePath;
            } else {
                QLOG_DEBUG() << "[Mica] Failed to save screenshot";
            }
        }
        eApp->setElaMicaImagePath(filePath);
        eApp->setWindowDisplayMode(ElaApplicationType::Mica);
    };
    //CaptureScreenShotAndEnableMica();
    eApp->setWindowDisplayMode(ElaApplicationType::Acrylic);


    // Build StackedPage;
    T_Home2 *homePage=new T_Home2();
    T_Deploy *deployPage=new T_Deploy(parent);
    T_Manage *managePage=new T_Manage(this,readPolicy);



    if(deployPage->perfectPath.isEmpty()){
        warnTips(tr("完美平台"),tr("未启动无法自动获取路径"));
    }
    formatPage=new T_FormatHelp(parent);
    connect(deployPage,&T_Deploy::targetToFormatHelp,this,[=](){
        targetToFormatHelp();
    });
    // connect(deployPage,&T_Deploy::changeHideAShowState,[=](bool state){
    //     if(state) this->show();
    //     else this->hide();
    // });
    connect(homePage->homeCard2, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        // this->navigation(formatPage->property("ElaPageKey").toString());
        targetToFormatHelp();
    });

    settingPage=new T_Setting(parent,hwInfo);

    settingPage->steamPath=deployPage->CFGPath;
    settingPage->perfectPath=deployPage->perfectPath;

    // gSettings->setCFGPath(deployPage->CFGPath);
    // gSettings->setPerfectPath(deployPage->perfectPath);
    settingPage->updateText();

    connect(settingPage,&T_Setting::steamPathHasChanged,[=](){
        deployPage->CFGPath=settingPage->steamPath;
        gSettings->setCFGPath(settingPage->steamPath);
    });
    connect(settingPage,&T_Setting::perfectPathHasChanged,[=](){
        deployPage->perfectPath=settingPage->perfectPath;
        gSettings->setPerfectPath(settingPage->perfectPath);
    });

    // connect(deployPage,&T_Deploy::complete,[=](){
    //     managePage->changeOccured();
    // });
    aboutpage=new T_About();
    aboutpage->hide();

    // connect(this,&ElaWindow::userInfoCardClicked,[=](){

    // });


    T_CFGSettings *CFGSettingPage;
    CFGSettingPage = new T_CFGSettings(this);

    this->addPageNode(tr("主菜单"),homePage,ElaIconType::House);
    this->addPageNode(tr("安装 | 管理配置"),deployPage,ElaIconType::ArrowDownToBracket);
    this->addPageNode(tr("管理 | 开关"),managePage,ElaIconType::PowerOff);
    this->addPageNode(tr("用户"),CFGSettingPage,ElaIconType::User);

    // this->addPageNode(tr("接入"),formatPage,ElaIconType::NetworkWired);
    this->addFooterNode(tr("接入"),formatPage,_formatKey, 0 ,ElaIconType::NetworkWired);
    this->addFooterNode(tr("关于"),nullptr, _aboutKey,0,ElaIconType::User);
    this->addFooterNode(tr("设置"), settingPage, _settingKey, 0, ElaIconType::GearComplex);



    connect(this, &ElaWindow::navigationNodeClicked, this, [=](ElaNavigationType::NavigationNodeType nodeType, QString nodeKey) {
        Q_UNUSED(nodeType);
        if (_aboutKey == nodeKey)
        {
            // aboutpage->setFixedSize(400, 400);
            aboutpage->moveToCenter();
            aboutpage->show();
        }
        if(deployPage->property("ElaPageKey").toString() == nodeKey){
            if(settingPage->steamPath.isEmpty()){
                ElaContentDialog *errorResult=new ElaContentDialog(this);
                QWidget *errorWidget=new QWidget(this);
                ElaLineEdit *errorLine=new ElaLineEdit(this);
                ElaText *errorText=new ElaText(tr("错误"),this);
                errorText->setTextPixelSize(25);
                QVBoxLayout *errorlayout=new QVBoxLayout(errorWidget);
                // errorWidget->setLayout(errorlayout);
                errorLine->setMinimumHeight(45);
                errorLine->setReadOnly(true);
                errorLine->setText(tr("CFG 文件目录未配置"));
                errorlayout->addSpacing(5);
                errorlayout->addWidget(errorText);
                errorlayout->addSpacing(10);
                errorlayout->addWidget(errorLine);
                errorResult->setLeftButtonText(tr("返回"));
                errorResult->setMiddleButtonText(tr("前往配置"));
                errorResult->setRightButtonText(tr("确定"));
                connect(errorResult,&ElaContentDialog::middleButtonClicked,[=](){
                    QTimer::singleShot(1000,this,[=](){
                        targetToSetting();
                    });

                    errorResult->accept();
                    errorResult->close();
                });
                connect(errorResult,&ElaContentDialog::leftButtonClicked,[=](){
                    QTimer::singleShot(1000,this,[=](){
                        ElaNavigationRouter::getInstance()->navigationRouteBack();
                    });
                });
                connect(errorResult,&ElaContentDialog::rightButtonClicked,[=](){
                    QTimer::singleShot(1000,this,[=](){
                        ElaNavigationRouter::getInstance()->navigationRouteBack();
                    });
                });
                errorResult->setCentralWidget(errorWidget);
                errorResult->exec();
            }
            return;
        }
    });
    QCoreApplication::processEvents(QEventLoop::AllEvents,5);
    if(hwInfo){
        QString CPUVendor=setting.value("CPUVendor").toString();
        QString CPUName=setting.value("CPUName").toString();
        QString GPUVendor=setting.value("GPUVendor").toString();
        QString GPUName=setting.value("GPUName").toString();
        if(CPUVendor.isEmpty()){
            CPUVendor=SystemInfo::getCpuVendor();

            setting.setValue("CPUVendor",CPUVendor);
        }
        if(CPUName.isEmpty()){
            CPUName=SystemInfo::getCpuName();

            setting.setValue("CPUName",CPUName);
        }
        if(GPUVendor.isEmpty()){
            GPUVendor=SystemInfo::getGpuVendor();

            setting.setValue("GPUVendor",GPUVendor);
        }
        if(GPUName.isEmpty()){
            GPUName=SystemInfo::getGpuName();

            setting.setValue("GPUName",GPUName);
        }
        settingPage->CPUVendor=CPUVendor;
        settingPage->CPUName=CPUName;
        settingPage->GPUVendor=GPUVendor;
        settingPage->GPUName=GPUName;
        settingPage->updateText();
    }
    // T_Deploy * debugDeploy=new T_Deploy(this);
    // debugDeploy->show();
    //debug for platform
    // QCoreApplication::processEvents(QEventLoop::AllEvents,5);

    QLOG_DEBUG() << "[SysInfo] CPU Vendor:" << SystemInfo::getCpuVendor();
    QLOG_DEBUG() << "[SysInfo] CPU Name:" << SystemInfo::getCpuName();
    QLOG_DEBUG() << "[SysInfo] GPU Vendor:" << SystemInfo::getGpuVendor();
    QLOG_DEBUG() << "[SysInfo] GPU Name:" << SystemInfo::getGpuName();

    static int badges=0;
    QTimer *timeShot=new QTimer(this);
    SCFGPage=new W_SCFGTransformer();
    SCFGPage->programMove=true;
    auto Tips(deployPage);
    timeShot->stop();
    connect(timeShot,&QTimer::timeout,[=](){
        badges=0;
        DBG(badges);
        timeShot->stop();
    });
    connect(this,&ElaWindow::userInfoCardClicked,[=](){
        timeShot->start(1000);
        badges++;
        if(badges>=5){
            badges=0;
            SCFGPage->show();
            SCFGPage->setselfMove(false);
            SCFGPage->badges=0;

            Tips->showSuccess(tr("成功"),tr("转换器启动中"));
        }
        DBG(badges);
    });
    QRect rect=this->geometry();
    gSettings->setParentRect(&rect);

    connect(this,&AsulWindow::sizeChanged,[=](QRect *size){
        gSettings->setParentRect(size);
    });
    SCFGPage->programMove=false;

    connect(this,&AsulWindow::closeButtonClicked,[=](){
        bool askClose=Tips->askDialog(this,tr("关闭"),tr("确定要关闭此程序?"));
        if(askClose){
            Asul::closeWindow();
            SCFGPage->close();
            SCFGPage->destroyed();
        }
    });




    connect(gSettings,&GlobalSettings::pAutoHelpChanged,[=](){
        QLOG_DEBUG()<<"[GlobalSettings] AutoHelpChanged, Now Is "<<gSettings->getAutoHelp();
    });
    connect(gSettings,&GlobalSettings::pAutoExeChanged,[=](){
        QLOG_DEBUG()<<"[GlobalSettings] AutoHelpChanged, Now Is "<<gSettings->getAutoHelp();
    });
    connect(gSettings,&GlobalSettings::pAutoHelpChanged,[=](){
        QLOG_DEBUG()<<"[GlobalSettings] AutoHelpChanged, Now Is "<<gSettings->getAutoHelp();
    });
    connect(gSettings,&GlobalSettings::pPreviewShowChanged,[=](){
        QLOG_DEBUG()<<"[GlobalSettings] PreviewShowChanged, Now Is "<<gSettings->getPreviewShow();
    });


}
void Asul::targetToFormatHelp(){
    this->navigation(formatPage->property("ElaPageKey").toString());
}
void Asul::targetToSetting(){
    this->navigation(settingPage->property("ElaPageKey").toString());
}
void Asul::originTips(QString title,QString context,ElaMessageBarType::MessageMode type){
    originTips(this,title,context,type);
}
void Asul::originTips(ElaWindow* parent,QString title,QString context,ElaMessageBarType::MessageMode type){
    // ElaMessageButton * bt=new ElaMessageButton(parent);
    // bt->setMessageTargetWidget(parent);
    // bt->setBarTitle(title);
    // bt->setBarText(context);
    // bt->setMessageMode(type);
    // bt->setPositionPolicy(ElaMessageBarType::TopRight);
    // bt->click();
    // bt->deleteLater();
    QFont font("Microsoft YaHei"); // 获取字体
    QFontMetrics metrics(font); // 使用字体创建字体度量对象
    int textWidth = metrics.horizontalAdvance(context); // 获取文本宽度
    int screenWidth = window()->width(); // 获取屏幕宽度（或阅读区域宽度）
    int charactersPerMinute = 200; // 每分钟阅读的字符数（可调整）

    // 计算阅读时间（秒）
    double readingTimeInSeconds = static_cast<double>(textWidth) / screenWidth * 60 * charactersPerMinute;
    double time=readingTimeInSeconds;
    QLOG_DEBUG()<<"[Read_Time] Context: "<<context<<"    Time(ms): "<<time;
    if(type==ElaMessageBarType::Error)
        ElaMessageBar::error(ElaMessageBarType::Top,title,context,time,parent);
    else if(type==ElaMessageBarType::Information)
        ElaMessageBar::information(ElaMessageBarType::Top,title,context,time,parent);
    else if(type==ElaMessageBarType::Success)
        ElaMessageBar::success(ElaMessageBarType::Top,title,context,time,parent);
    else if(type==ElaMessageBarType::Warning)
        ElaMessageBar::warning(ElaMessageBarType::Top,title,context,time,parent);
}
QString Asul::getPath(QString vdfFile)
{
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

QString Asul::getProcessPath(const QString &processName)
{
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

void Asul::getAllPath()
{
    QCoreApplication::processEvents(QEventLoop::AllEvents,5);
    QString regPath = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\steam\\Shell\\Open\\Command";
    QSettings *reg=new QSettings (regPath,QSettings::NativeFormat);
    QString steamPath = reg->value(".").toString();
    steamPath =QString(steamPath.split("\"")[1])+QString("\\..\\");
    //获取到cfg目录

    QString libraryFoldersFile = steamPath + "/steamapps/libraryfolders.vdf";
    if(steamPath.isEmpty()){
        return;
    }
    QString CFGPath = getPath(libraryFoldersFile);
    gSettings->setCFGPath(CFGPath);
    gSettings->setSteamPath(QString(steamPath).replace("\\","/").replace("steam.exe/..",""));
    gSettings->setSteamUserPath(QString(steamPath).replace("\\","/").replace("steam.exe/..","userdata"));
    gSettings->setSteamConfPath(QString(steamPath).replace("\\","/").replace("steam.exe/..","config"));
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
    QString perfectPath = getProcessPath(processName);
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



void Asul::warnTips(ElaWindow *parent,QString title,QString context){
    originTips(parent,title,context,ElaMessageBarType::Warning);
}
void Asul::warnTips(QString title,QString context){
    originTips(title,context,ElaMessageBarType::Warning);
}
void Asul::succTips(ElaWindow *parent,QString title,QString context){
    originTips(parent,title,context,ElaMessageBarType::Success);
}
void Asul::succTips(QString title,QString context){
    originTips(title,context,ElaMessageBarType::Success);
}
void Asul::normTips(ElaWindow *parent,QString title,QString context){
    originTips(parent,title,context,ElaMessageBarType::Information);
}
void Asul::normTips(QString title,QString context){
    originTips(title,context,ElaMessageBarType::Information);
}
void Asul::errTips(ElaWindow *parent,QString title,QString context){
    originTips(parent,title,context,ElaMessageBarType::Error);
}
void Asul::errTips(QString title,QString context){
    originTips(title,context,ElaMessageBarType::Error);
}
void Asul::saveResourceFile(QString resProfiex,QString resFileName,QString destFullPathFileName) {

    //组装资源文件中的文件
    QString resFile;
    resFile=":/"+resProfiex+"/"+resFileName;

    QFile file;
    file.copy(resFile,destFullPathFileName);
}
void Asul::dragEnterEvent(QDragEnterEvent* ev)
{
    if (ev->mimeData()->hasUrls())
    {
        ev->acceptProposedAction();
    }
    else
    {
        ev->ignore();
    }
}

void Asul::dropEvent(QDropEvent* ev)
{
    if (ev->mimeData()->hasUrls())
    {
        // QList<QUrl> url_list = ev->mimeData()->urls();
        // for (int i = 0; i < url_list.size(); i++)
        // {
        //     QString name = url_list.at(i).toLocalFile();
        //     QFileInfo info(name);
        //     if (info.suffix() != "ncm" || info.isDir())
        //     {
        //         continue;
        //     }
        //     this->addFile(name);
        // }
        // emit dropEnd();
        QList<QUrl> urls = ev->mimeData()->urls();
        if (!urls.isEmpty()) {
            QString filename = urls.first().toLocalFile();
            emit fileDropped(filename);
            QLOG_DEBUG()<<"[Drop] "<<filename;
            update();
        }
    }
}

Asul::~Asul() {
    Logger::destroyInstance();
    delete gLocation;
    delete aboutpage;
}



void Asul::showEvent(QShowEvent *event)
{
    if(!m_initialized) {
        event->ignore();
        return;
    }
    ElaWindow::showEvent(event);
}
