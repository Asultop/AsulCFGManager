#include "AsulCFGManager.h"
#include "Headers/BaseInclude.h"
#include "Headers/T_Home.h"
#include "Headers/T_Home2.h"
#include "Headers/T_Manage.h"
#include <QStandardPaths>
#include <ElaApplication.h>
#include <QDesktopServices>
#include <ElaContentDialog.h>
#include <ElaMessageBar.h>
Asul::Asul(QWidget *parent)
    : ElaWindow(parent)
{
    this->setUserInfoCardPixmap(QPixmap(":/pic/Pic/favicon.png"));
    this->setUserInfoCardTitle("CFG LinkWard");
    this->setUserInfoCardSubTitle("by Alivn@Asul.top");
    this->setUserInfoCardVisible(true);
    this->setWindowTitle("AsulCFGManager");
    // this->setWindowButtonFlag(ElaAppBarType::ThemeChangeButtonHint,false);
    this->setWindowButtonFlag(ElaAppBarType::StayTopButtonHint,false);


    this->setAcceptDrops(true);
    // this->setFixedSize(QSize(800,600));
    //SaveFile To
    gLocation=new QTemporaryDir();
    GlobalLocation=gLocation->path();
    // Use For Debug...
    // QDesktopServices::openUrl(gLocation->path());
    //
    saveResourceFile("/Tools","Tools/7z.exe",GlobalLocation+"/7z.exe");
    saveResourceFile("/Tools","Tools/7z.dll",GlobalLocation+"/7z.dll");
    saveResourceFile("/Tools","Tools/getPerfectPath.bat",GlobalLocation+"/getPerfectPath.bat");
    // normTips("located",GlobalLocation->path());
    //SaveFile End

    QScreen *screen = QGuiApplication::primaryScreen();
    QString filePath = GlobalLocation+"/mica.png";
    if (screen) {
        // 截取全屏幕图像
        QPixmap screenshot = screen->grabWindow(0);

        // 设置截图保存的文件名

        // 保存截图到文件
        if (screenshot.save(filePath, "PNG")) {
            qDebug() << "Screenshot saved as" << filePath;
        } else {
            qDebug() << "Failed to save screenshot";
        }
    }
    eApp->setIsEnableMica(true);
    eApp->setMicaImagePath(filePath);
    // // this->setIsEnableMica(false);
    // this->setMicaImagePath(filePath);
    // this->setIsEnableMica(true);
    ElaTheme::getInstance()->setThemeMode(ElaThemeType::Dark);
    this->update();
    this->setNavigationBarDisplayMode(ElaNavigationType::Compact);

    //
    // UI END
    //
    T_Home2 *homePage=new T_Home2();
    // T_Deploy *deployPage=new T_Deploy(parent,gLocation);
    T_Deploy *deployPage=new T_Deploy(parent,gLocation);
    T_Manage *managePage=new T_Manage();
    deployPage->getAllPath();
    if(deployPage->perfectPath.isEmpty()){
        warnTips("完美平台","未启动无法自动获取路径");
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

    settingPage=new T_Setting(parent);
    settingPage->steamPath=deployPage->CFGPath;
    settingPage->perfectPath=deployPage->perfectPath;
    settingPage->updateText();
    connect(settingPage,&T_Setting::steamPathHasChanged,[=](){
        deployPage->CFGPath=settingPage->steamPath;
    });
    connect(settingPage,&T_Setting::perfectPathHasChanged,[=](){
        deployPage->perfectPath=settingPage->perfectPath;
    });
    connect(settingPage,&T_Setting::toggleAreaHasChanged,[=](){
        deployPage->autoDir=settingPage->autoDir;
        deployPage->autoExe=settingPage->autoExe;
        deployPage->autoHelp=settingPage->autoHelp;
    });
    connect(settingPage,&T_Setting::changeAll,[=](){
        deployPage->autoDir=settingPage->autoDir;
        deployPage->autoExe=settingPage->autoExe;
        deployPage->autoHelp=settingPage->autoHelp;
        deployPage->previewShow=settingPage->previewShow;
        managePage->previewShow=settingPage->previewShow;
        managePage->changeOccured();
        deployPage->changeOccured();
    });

    // connect(deployPage,&T_Deploy::complete,[=](){
    //     managePage->changeOccured();
    // });
    aboutpage=new T_About();
    aboutpage->hide();

    // connect(this,&ElaWindow::userInfoCardClicked,[=](){

    // });


    this->addPageNode("主菜单",homePage,ElaIconType::House);
    this->addPageNode("安装 | 管理配置",deployPage,ElaIconType::ArrowDownToBracket);
    this->addPageNode("管理 | 开关",managePage,ElaIconType::PowerOff);
    this->addPageNode("接入",formatPage,ElaIconType::NetworkWired);
    this->addFooterNode("关于",nullptr, _aboutKey,0,ElaIconType::User);
    this->addFooterNode("设置", settingPage, _settingKey, 0, ElaIconType::GearComplex);



    connect(this, &ElaWindow::navigationNodeClicked, this, [=](ElaNavigationType::NavigationNodeType nodeType, QString nodeKey) {
        if (_aboutKey == nodeKey)
        {
            aboutpage->setFixedSize(400, 400);
            aboutpage->moveToCenter();
            aboutpage->show();
        }
        if(deployPage->property("ElaPageKey").toString() == nodeKey){
            if(settingPage->steamPath.isEmpty()){
                ElaContentDialog *errorResult=new ElaContentDialog(this);
                QWidget *errorWidget=new QWidget(this);
                ElaLineEdit *errorLine=new ElaLineEdit(this);
                ElaText *errorText=new ElaText("错误",this);
                errorText->setTextPixelSize(25);
                QVBoxLayout *errorlayout=new QVBoxLayout(errorWidget);
                errorWidget->setLayout(errorlayout);
                errorLine->setMinimumHeight(45);
                errorLine->setReadOnly(true);
                errorLine->setText("CFG 文件目录未配置");
                errorlayout->addSpacing(5);
                errorlayout->addWidget(errorText);
                errorlayout->addSpacing(10);
                errorlayout->addWidget(errorLine);
                errorResult->setLeftButtonText("返回");
                errorResult->setMiddleButtonText("前往配置");
                errorResult->setRightButtonText("确定");
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
    if(type==ElaMessageBarType::Error)
        ElaMessageBar::error(ElaMessageBarType::TopRight,title,context,1000,parent);
    else if(type==ElaMessageBarType::Information)
        ElaMessageBar::information(ElaMessageBarType::TopRight,title,context,1000,parent);
    else if(type==ElaMessageBarType::Success)
        ElaMessageBar::success(ElaMessageBarType::TopRight,title,context,1000,parent);
    else if(type==ElaMessageBarType::Warning)
        ElaMessageBar::warning(ElaMessageBarType::TopRight,title,context,1000,parent);
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
            qDebug()<<filename;
            update();
        }
    }
}

Asul::~Asul() {
    delete gLocation;
    delete aboutpage;
}
