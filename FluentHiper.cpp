#include "FluentHiper.h"
#include "Headers/BaseInclude.h"
#include "Headers/T_Home.h"
#include <QStandardPaths>
#include <ElaApplication.h>
#include <QDesktopServices>
FluentHiper::FluentHiper(QWidget *parent)
    : ElaWindow(parent)
{
    this->setUserInfoCardPixmap(QPixmap(":/pic/Pic/favicon.ico"));
    this->setUserInfoCardTitle("CFG LinkWard");
    this->setUserInfoCardSubTitle("by Alivn@Asul.top");
    this->setUserInfoCardVisible(true);
    this->setWindowTitle("CFG - CSRM");
    this->setWindowButtonFlag(ElaAppBarType::ThemeChangeButtonHint,false);
    this->setWindowButtonFlag(ElaAppBarType::StayTopButtonHint,false);


    this->setAcceptDrops(true);
    this->setFixedSize(QSize(800,600));
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
    T_Home *homePage=new T_Home(parent);
    T_Deploy *deployPage=new T_Deploy(parent,gLocation);
    formatPage=new T_FormatHelp(parent);
    connect(deployPage,&T_Deploy::targetToFormatHelp,this,[=](){
        targetToFormatHelp();
    });
    connect(homePage->homeCard, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        // this->navigation(formatPage->property("ElaPageKey").toString());
        targetToFormatHelp();
    });

    this->addPageNode("主菜单",homePage,ElaIconType::House);
    this->addPageNode("安装",deployPage,ElaIconType::ArrowDownToBracket);
    this->addPageNode("接入",formatPage,ElaIconType::NetworkWired);
}
void FluentHiper::targetToFormatHelp(){
    this->navigation(formatPage->property("ElaPageKey").toString());
}
void FluentHiper::originTips(QString title,QString context,ElaMessageBarType::MessageMode type){
    originTips(this,title,context,type);
}
void FluentHiper::originTips(ElaWindow* parent,QString title,QString context,ElaMessageBarType::MessageMode type){
    ElaMessageButton * bt=new ElaMessageButton(parent);
    bt->setBarTitle(title);
    bt->setBarText(context);
    bt->setMessageMode(type);
    bt->setPositionPolicy(ElaMessageBarType::TopRight);
    bt->click();
    bt->deleteLater();
}
void FluentHiper::warnTips(ElaWindow *parent,QString title,QString context){
    originTips(parent,title,context,ElaMessageBarType::Warning);
}
void FluentHiper::warnTips(QString title,QString context){
    originTips(title,context,ElaMessageBarType::Warning);
}
void FluentHiper::succTips(ElaWindow *parent,QString title,QString context){
    originTips(parent,title,context,ElaMessageBarType::Success);
}
void FluentHiper::succTips(QString title,QString context){
    originTips(title,context,ElaMessageBarType::Success);
}
void FluentHiper::normTips(ElaWindow *parent,QString title,QString context){
    originTips(parent,title,context,ElaMessageBarType::Information);
}
void FluentHiper::normTips(QString title,QString context){
    originTips(title,context,ElaMessageBarType::Information);
}
void FluentHiper::errTips(ElaWindow *parent,QString title,QString context){
    originTips(parent,title,context,ElaMessageBarType::Error);
}
void FluentHiper::errTips(QString title,QString context){
    originTips(title,context,ElaMessageBarType::Error);
}
void FluentHiper::saveResourceFile(QString resProfiex,QString resFileName,QString destFullPathFileName) {

    //组装资源文件中的文件
    QString resFile;
    resFile=":/"+resProfiex+"/"+resFileName;

    QFile file;
    file.copy(resFile,destFullPathFileName);
}
void FluentHiper::dragEnterEvent(QDragEnterEvent* ev)
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

void FluentHiper::dropEvent(QDropEvent* ev)
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

FluentHiper::~FluentHiper() {
    delete gLocation;
}
