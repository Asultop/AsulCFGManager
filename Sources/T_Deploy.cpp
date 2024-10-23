#include "Headers/T_Deploy.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QTemporaryDir>
#include "FluentHiper.h"
#include <ElaContentDialog.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <ElaPopularCard.h>
#include <QDesktopServices>
#define parentWindow dynamic_cast<FluentHiper *>(this->parent())
T_Deploy::T_Deploy(QWidget *parent,QTemporaryDir *tempDir)
    : BaseScrollPage{parent}
{
    initWidget("部署 CFG","安装 CFG","选择符合格式的安装包\n我们会为您解析数据并自动安装到您的CFG目录下");
    gLocation=tempDir;
    getAllPath();
    qDebug()<<"\n=====getAllPath()======\nSteamPath:"
             <<steamPath
             <<"\nCFGPath:"<<CFGPath
             <<"\nperfectPath:"<<perfectPath;
    // QDesktopServices::openUrl(CFGPath);
    QString tempLocation=QStandardPaths::writableLocation(QStandardPaths::TempLocation);

    ElaScrollPageArea *selectFileArea=new ElaScrollPageArea(this);
    QHBoxLayout *selectFileLayout=new QHBoxLayout(selectFileArea);
    ElaText *selectFileText=new ElaText("选择文件:",this);
    selectedFilePath=new ElaLineEdit(this);
    selectFileButton=new ElaPushButton("选择",this);
    // ElaPushButton *analyFile=new ElaPushButton("分析文件",this);
    // analyFile->setMinimumHeight(HugeHeight);
    selectFileText->setTextPixelSize(15);
    selectedFilePath->setReadOnly(true);
    selectedFilePath->setMinimumHeight(MiddleHeight);
    selectedFilePath->setPlaceholderText("请选择文件");
    // selectFileLayout->addWidget(selectFileText);
    // selectFileLayout->addWidget(selectedFilePath);
    selectFileText->hide();
    selectedFilePath->hide();
    selectFileButton->setMinimumHeight(MiddleHeight);
    selectFileLayout->addWidget(selectFileButton);
    connect(selectFileButton,&ElaPushButton::clicked,[=](){
        QString fileName = QFileDialog::getOpenFileName(
            this,
            "打开 压缩包 文件",
            QDir::homePath(),
            "压缩包 文件 (*.zip *.tar *.gz *.bz2 *.rar *.7z)"
            );

        if (!fileName.isEmpty()) {
            // selectedFilePath->setText(fileName);
            ZipFilePath=fileName;
            AnalyseFile();
        }
        else {
            parentWindow->normTips("文件","取消选择");
        }
    });



    centerVLayout->addWidget(selectFileArea);
    centerVLayout->addStretch();
    flowLayout=new ElaFlowLayout(0,5,5);
    flowLayout->setIsAnimation(true);
    flowLayout->setContentsMargins(10,10,10,10);
    // flowLayout->setContentsMargins(5,5,20,30);
    centerVLayout->addLayout(flowLayout,2);


    //init deploywindow

}
void T_Deploy::AnalyseFile(){

    FluentHiper *window=parentWindow;
    QDir GlobalDir(gLocation->path());
    if(GlobalDir.exists("Analy")){
        QDir AnalyDir(gLocation->path()+"/Analy");
        AnalyDir.removeRecursively();
    }
    if(!GlobalDir.exists("Analy")){
        GlobalDir.mkdir("Analy");
    }
    QFileInfo zipFileInfo(ZipFilePath);
    if(zipFileInfo.size()>20*1024*1024){

        window->warnTips("文件",QString("包过大 %1MB (超过 20 MB)").arg(zipFileInfo.size()/1024/1024));
        return;
    }
    QString UncompressFileLocation=gLocation->path().replace("/","\\")+"\\Analy";
    UnCompressFiles(ZipFilePath,UncompressFileLocation);

    //Read Info

    static bool hasConnected=false;
    if(!hasConnected){
        hasConnected=true;
        connect(this,&T_Deploy::generateError,this,[&](QString content){
            ElaContentDialog *errorResult=new ElaContentDialog(this);
            QWidget *errorWidget=new QWidget(this);
            ElaLineEdit *errorLine=new ElaLineEdit(this);
            ElaText *errorText=new ElaText("错误",this);
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
            errorResult->setLeftButtonText("取消");
            errorResult->setMiddleButtonText("了解格式");
            errorResult->setRightButtonText("重选");
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

    QString CFGMainPath=checkIfOnlyOneDirectory(UncompressFileLocation);
    //error catch end
    //Get Generate ElaScrollPage;
    if(!CFGMainPath.isEmpty())
        generateScrollPageLayout(CFGMainPath,flowLayout);

}

#include <QDir>
#include <QDebug>

QString T_Deploy::checkIfOnlyOneDirectory(const QString &folderPath) {
    QDir dir(folderPath);
    if (!dir.exists()) {
        qDebug() << "The directory does not exist.";
        emit generateError("没有相关文件夹");
        return "";
    }

    // 获取目录中所有条目的列表，包括文件和目录
    QStringList entries = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::SortFlags(QDir::Name | QDir::IgnoreCase));

    // 检查是否恰好有一个目录
    if (entries.size() == 1) {
        QString subDirPath = dir.filePath(entries.first());
        return subDirPath;
    } else if (entries.isEmpty()) {
        emit generateError("文件包为空");
        qDebug()<<"package is null";
        return "";
    } else {
        emit generateError("文件包格式错误");
        qDebug()<<"Wrong package Format";
        return "";
    }
}
bool T_Deploy::UnCompressFiles(QString zipFile, QString sDestPath)
{
    QStringList sParams;
    sParams.append("x");												// 在当前目录中，使用完整路径从压缩档案中释放文件.或者到指定的输出文件夹
    sParams.append(zipFile);
    sParams.append(QString("-o%1").arg(sDestPath));						// 指定压缩文件格式
    sParams.append("-aoa");												// 不提示，直接覆盖同名文件

    QProcess process;

    QString strPath =gLocation->path()+"\\7z.exe";
    process.start(strPath, sParams);
    process.waitForFinished();
    QString A=process.readAllStandardError();
    QString B=process.readAllStandardOutput();
    qDebug()<<A;
    qDebug()<<B;
    process.close();
    return (A+B).isEmpty()?false:true;
}
void T_Deploy::generateScrollPageLayout(QString CFGFileLocation, ElaFlowLayout *cVLayout) {
    // FluentHiper *window=dynamic_cast<FluentHiper *>(parent());
    if (CFGFileLocations.contains(ZipFilePath)) {
        parentWindow->errTips("文件", "已存在或无效的包: " + ZipFilePath);
        return;
    }

    CFGFileLocations.append(ZipFilePath + "\n");
    QFile *infoFile = new QFile(CFGFileLocation + "/info.json");
    QString infoJsonContent;
    if (infoFile->open(QIODevice::ReadOnly)) {
        infoJsonContent = infoFile->readAll();
        infoFile->close();
    } else {
        emit generateError("info.json 文件:" + infoFile->errorString());
        return;
    }

    QJsonDocument infoDoc = QJsonDocument::fromJson(infoJsonContent.toUtf8());
    QJsonObject infoObj = infoDoc.object();

    QString Name = infoObj["name"].toString();
    QString webSite = infoObj["website"].toString();
    QString version = infoObj["version"].toString();
    QString connectJsonFile = infoObj["connect"].toString();
    QString resourceFile = infoObj["resource"].toString();
    QString MainCFGFile = infoObj["main"].toString();
    QString connectJsonFileLocation = CFGFileLocation + connectJsonFile;
    QFile *connectFile = new QFile(connectJsonFileLocation);

    if (!connectFile->open(QIODevice::ReadOnly)) {
        emit generateError("connect.json 文件:" + connectFile->errorString());
        return;
    }

    QTextStream ConnectStream(connectFile);
    QString connectJsonContent = ConnectStream.readAll();
    connectFile->close();

    QJsonDocument connectDoc = QJsonDocument::fromJson(connectJsonContent.toUtf8());
    QJsonObject connectObj = connectDoc.object();

    QString settings_usr = connectObj["settings-usr"].toString();
    QString settings_default = connectObj["settings-default"].toString();
    QString settings_file = connectObj["settings-file"].toString();
    QString connectDir;
    for(int i=0;i<connectJsonFile.split("/").length()-1;i++){
        connectDir.append(connectJsonFile.split("/")[i]);
    }
    qDebug()<<"\n==========ConnectDir===============\n"<<connectDir<<"\n";
    QString defaultJsonFileLocation=CFGFileLocation+"/"+connectDir+"/"+settings_default;
    QFile *defaultFile=new QFile(defaultJsonFileLocation);
    if(!defaultFile->open(QIODevice::ReadOnly)){
        emit generateError("default.json 文件:" + defaultFile->errorString());
        return;
    }
    QTextStream defaultStream(defaultFile);
    QString defaultJsonContent = defaultStream.readAll();
    defaultFile->close();
    QJsonDocument defaultDoc = QJsonDocument::fromJson(defaultJsonContent.toUtf8());
    QJsonObject defaultObj = defaultDoc.object();


    ElaPopularCard *CFGBreifInfo = new ElaPopularCard(this);
    CFGBreifInfo->setCardPixmap(QPixmap(":/pic/Pic/CS2.ico"));
    CFGBreifInfo->setTitle(Name + "  " + version);
    CFGBreifInfo->setSubTitle("@" + webSite);
    CFGBreifInfo->setInteractiveTips("队列中");
    CFGBreifInfo->setDetailedText(Name + " " + version + "\n" + webSite + " " + connectJsonFile + " " + resourceFile + " " + MainCFGFile + " " + ZipFilePath);
    CFGBreifInfo->setCardFloatPixmap(QPixmap(":/pic/Pic/favicon.ico"));
    this->selectedFilePath->setText(ZipFilePath);
    cVLayout->addWidget(CFGBreifInfo);


    connect(CFGBreifInfo, &ElaPopularCard::popularCardButtonClicked, this, [=]() {

        QJsonObject binds = defaultObj["binds"].toObject();
        QJsonObject settings=defaultObj["settings"].toObject();
        qDebug()<<binds<<"\n"<<settings;
        int keysCount = -1;
        static QStringList sumContent={};

        BaseScrollPage *CFGSettingPage=new BaseScrollPage(this);
        CFGSettingPage->initWidget("CFG 配置","解析项目","已自动解析存在的配置，请自行配置");
        CFGSettingPage->setWindowTitle("CFG 配置");
        // QVBoxLayout *CFGLayout=new QVBoxLayout(CFGSettingPage);
        // CFGSettingPage->setLayout(CFGLayout);
        ElaWindow *DeployWindow=new ElaWindow(this);
        DeployWindow->setUserInfoCardPixmap(QPixmap("://pic/Pic/favicon.ico"));
        DeployWindow->setUserInfoCardTitle("CFG LinkWard");
        DeployWindow->setUserInfoCardSubTitle("by Alivn@Asul.top");
        DeployWindow->setUserInfoCardVisible(true);
        DeployWindow->setWindowTitle("部署 CFG");
        DeployWindow->setWindowButtonFlag(ElaAppBarType::ThemeChangeButtonHint,false);
        DeployWindow->setWindowButtonFlag(ElaAppBarType::StayTopButtonHint,false);

        static int keyNum=0,placedKeyNum=0;
        ElaProgressBar *deployProgress=new ElaProgressBar(DeployWindow);
        QVBoxLayout *AllkeyLayout=new QVBoxLayout(DeployWindow);
        for (auto it = binds.constBegin(); it != binds.constEnd(); ++it) {
            keysCount++;
            sumContent.resize(keysCount + 1); // 确保 sumContent 的大小足够大

            QJsonObject bind = it.value().toObject();
            QString title = bind.value("title").toString();
            bool hide = bind.value("hide").toBool();
            QString key = bind.value("key").toString();
            QString content = bind.value("content").toString();
            QJsonArray ban = bind.value("ban").toArray();
            QString banText = bind.value("ban-text").toString();

            QString bans="Ban:[";
            for (const QJsonValue &value : ban) {

                bans.append(value.toString() + ",");
            }
            bans.append("]");
            ElaScrollPageArea *keyArea = new ElaScrollPageArea(DeployWindow);
            QHBoxLayout *keyLayout = new QHBoxLayout(keyArea);
            ElaText *keyText = new ElaText(title, DeployWindow);
            keyText->setTextPixelSize(15);
            keyText->setMinimumWidth(120);
            ElaLineEdit *keyLine = new ElaLineEdit(DeployWindow);
            keyLine->setFont(subFont);
            keyLine->setMinimumHeight(MiddleHeight);
            keyLine->setText(key);
            keyLine->setPlaceholderText(bans);
            QString keyCon=QString("bind \"%1\" %2").arg(keyLine->text()).arg(content);
            // sumContent[keysCount] = keyContent;
            // sumContent.operator [](keysCount).replace(keysCount, "somethingNew");
            sumContent[keysCount]=keyCon;
            keyLine->setProperty("hasChanged",false);
            connect(keyLine, &ElaLineEdit::editingFinished, [=]() {
                QString keyLineText=keyLine->text();


                if(!keyLine->property("hasChanged").toBool()){
                    placedKeyNum++;
                    keyLine->setProperty("hasChanged",true);
                }
                deployProgress->setValue(placedKeyNum);
                if(keyLineText.isEmpty()){
                    if(keyLine->property("hasChanged").toBool())
                        placedKeyNum--;
                    keyLine->setProperty("hasChanged",false);
                    deployProgress->setValue(placedKeyNum);
                    sumContent[keysCount]="";
                    return;
                }
                bool inBan=false;
                QString banList=bans.split("[")[1].split("]")[0];
                foreach(QString var,banList.split(",")){
                    if(keyLineText==var){
                        inBan=true;
                        break;
                    }
                }
                if (inBan) {
                    parentWindow->errTips(DeployWindow,title, "不允许的输入:" + banText);
                    keyLine->clear();
                } else {
                    QString keyContent=QString("bind \"%1\" %2").arg(keyLine->text()).arg(content);
                    // sumContent[keysCount] = keyContent;
                    // sumContent.operator [](keysCount).replace(keysCount, "somethingNew");
                    sumContent[keysCount]=keyContent;
                }
            });
            keyLayout->addWidget(keyText);
            keyLayout->addWidget(keyLine);
            // CFGLayout->addWidget(keyArea);
            //CFGSettingPage->centerVLayout->addWidget(keyArea);

            //CFGSettingPage->centerVLayout->addSpacing(10);
            AllkeyLayout->addWidget(keyArea);
            AllkeyLayout->addSpacing(5);
            keyNum++;
        }

        for (auto it = settings.constBegin(); it != settings.constEnd(); ++it) {
            keysCount++;

            sumContent.resize(keysCount + 1); // 确保 sumContent 的大小足够大

            QJsonObject setting = it.value().toObject();
            QString title = setting.value("title").toString();
            bool hide = setting.value("hide").toBool();
            QJsonArray optionsArray = setting["options"].toArray();
            QJsonArray textArray = setting["text"].toArray();
            int usingKey=setting["using"].toInt();
            QStringList options,texts;
            // qDebug()<<options[usingKey];
            // QString keyContent=QString(options[usingKey]);
            // sumContent[keysCount] = keyContent;
            // sumContent.operator [](keysCount).replace(keysCount, "somethingNew");
            // sumContent[keysCount]=keyContent;

            for (const QJsonValue &value : optionsArray) {
                options.append( value.toString());
            }
            for (const QJsonValue &value : textArray) {
                texts.append(value.toString());
            }
            sumContent[keysCount]=options[usingKey];

            ElaScrollPageArea *keyArea = new ElaScrollPageArea(DeployWindow);
            QHBoxLayout *keyLayout = new QHBoxLayout(keyArea);
            ElaText *keyText = new ElaText(title, DeployWindow);
            keyText->setTextPixelSize(15);
            keyText->setMinimumWidth(120);
            ElaComboBox *keyCombo=new ElaComboBox(DeployWindow);
            keyCombo->addItems(texts);
            keyCombo->setCurrentIndex(usingKey);
            keyCombo->setProperty("hasChanged",false);
            connect(keyCombo, &ElaComboBox::currentIndexChanged, [=](int CurrentIndex) {
                if(!keyCombo->property("hasChanged").toBool()){
                    placedKeyNum++;
                    keyCombo->setProperty("hasChanged",true);
                }
                deployProgress->setValue(placedKeyNum);
                QString keyContent=QString(options[CurrentIndex]);
                sumContent[keysCount]=keyContent;

            });
            keyLayout->addWidget(keyText);
            keyLayout->addWidget(keyCombo);
            // CFGLayout->addWidget(keyArea);
            //CFGSettingPage->centerVLayout->addWidget(keyArea);

            //CFGSettingPage->centerVLayout->addSpacing(10);
            AllkeyLayout->addWidget(keyArea);
            AllkeyLayout->addSpacing(5);
            keyNum++;
        }

        // CFGSettingPage->show();
        // CFGSettingPage->centerVLayout->addLayout(AllkeyLayout,10);

        CFGSettingPage->centerVLayout->addLayout(AllkeyLayout,10);
        deployProgress->setMinimum(0);
        deployProgress->setMaximum(keyNum);
        deployProgress->setMinimumHeight(TinyHeight);

        ElaPushButton *deployButton=new ElaPushButton("部署",DeployWindow);
        deployButton->setMinimumHeight(HugeHeight);
        connect(deployButton,&ElaPushButton::clicked,[=](){
            ElaContentDialog *dialog=new ElaContentDialog(DeployWindow);
            dialog->setLeftButtonText("返回");
            dialog->setMiddleButtonText("取消");
            dialog->setRightButtonText("确认");
            connect(dialog,&ElaContentDialog::rightButtonClicked,this,[=](){
                //Todo : 部署CFG包，此处可用参数:
                //CFGFileLocation + resourceFile; //解压CFG整体包文件夹,resource.zip文件地址，若无则跳过
                //CFGFileLocation + MainCFGFile; // info.json 中 Main.cfg的地址
                //perfectPath; //完美Resource路径
                //steamPath; //steam路径.replace("\\","/")
                //CFGPath; //CS2,CFG路径;
                // DeployWindow->close();
                //sumContent;  //存储了所有更改的值 可直接添加换行保存为preference.cfg
                //settings_file; //不是一个标准的文件目录，而是需要生成的配置文件

                QFile *preferrenceFile=new QFile(gLocation->path()+"/"+settings_file);
                preferrenceFile->open(QIODevice::ReadWrite);
                QTextStream preferrenceStream(preferrenceFile);
                preferrenceStream<<"// ====== 生成的文件 =======\n";
                preferrenceStream<<QString("//=这个 %1 文件由 Asul-cfgManager(AM) 根据配置自动生成 \n").arg(settings_file);
                preferrenceStream<<"\n//==这个 配置文件 从哪儿来的?\n";
                preferrenceStream<<"//CFG 制作者网站: "<<webSite<<"\n";
                preferrenceStream<<"//CFG 名称: "<<Name<<"\n";
                preferrenceStream<<"//CFG 版本:  "<<version<<"\n";
                preferrenceStream<<"//CFG 入口点文件(Main.cfg):  "<<MainCFGFile<<"\n";
                preferrenceStream<<"//==CFG 详细 结束\n\n";

                foreach(QString val,sumContent){
                    preferrenceStream<<val<<"\n";
                }
                preferrenceStream<<"\n\n//==参数结束\n";
                preferrenceStream<<"//AM 是由 Alivn开发的部署 CS2 CFG 的程序,旨在为CFG制作者提供更方便的分发服务";
                preferrenceStream<<"//开发者:Github(https://github.com/AsulTop),WebSite(http://www.asul.top)";

                preferrenceFile->close();

                CFGBreifInfo->hide();
                CFGSettingPage->hide();
                parentWindow->succTips("CFG","一项配置完成部署");
                qDebug()<<"\n\n\n\n=========SumStart=======\n"<<sumContent<<"\n========SumEnd===========";

                DeployWindow->close();
                DeployWindow->deleteLater();
            });
            QWidget *dialogWidget=new QWidget(DeployWindow);
            ElaLineEdit *dialogLine=new ElaLineEdit(DeployWindow);
            ElaText *dialogText=new ElaText("部署",DeployWindow);
            dialogText->setTextPixelSize(MainFont);
            QVBoxLayout *dialoglayout=new QVBoxLayout(dialogWidget);
            dialogWidget->setLayout(dialoglayout);
            dialogLine->setMinimumHeight(MiddleHeight);
            dialogLine->setReadOnly(true);
            dialogLine->setText("确认配置?");
            dialoglayout->addSpacing(topSpacing);
            dialoglayout->addWidget(dialogText);
            dialoglayout->addSpacing(normSpacing);
            dialoglayout->addWidget(dialogLine);
            dialog->setCentralWidget(dialogWidget);
            dialog->exec();
        });
        //ElaProgressBar;
        CFGSettingPage->addTopWidget(deployProgress);
        CFGSettingPage->centerVLayout->addWidget(deployButton,3);



        DeployWindow->show();

        DeployWindow->setNavigationBarDisplayMode(ElaNavigationType::Compact);
        DeployWindow->setFixedSize(QSize(800,600));
        // QString pageKey;
        DeployWindow->addPageNode("进行中",CFGSettingPage,ElaIconType::ArrowDownToBracket);
        DeployWindow->navigation(CFGSettingPage->property("ElaPageKey").toString());
    });
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
    QString regPath = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Classes\\steam\\Shell\\Open\\Command";
    QSettings *reg=new QSettings (regPath,QSettings::NativeFormat);
    steamPath = reg->value(".").toString();
    qDebug()<<QString(steamPath.split("\"")[1])+QString("\\..\\");
    steamPath =QString(steamPath.split("\"")[1])+QString("\\..\\");
    //获取到cfg目录
    QString libraryFoldersFile = steamPath + "/steamapps/libraryfolders.vdf";
    CFGPath = getPath(libraryFoldersFile);
    //获取完美目录
    QProcess execBat;
    execBat.start("cmd.exe",QString("/c %1/getPerfectPath.bat").arg(gLocation->path()).split(" "));
    qDebug()<<"ZipFilePath : "<<gLocation->path();
    execBat.waitForFinished();

    perfectPath=execBat.readAllStandardOutput();
    qDebug()<<"perfectPath:"<<perfectPath;
    bool perfectOnProcess=false;
    foreach(QString var,perfectPath.split("\n")){
        if(var.contains("perfectworldarena")) perfectPath=var,perfectOnProcess=true;
    }
    if(perfectOnProcess){
        // ui->perfectPathLine->setText();
        QString beforePath="";
        perfectPath=perfectPath.replace("\\","/");
        // qDebug()<<"perfectPath in perfectonProcess:"<<perfectPath;
        QStringList perfectPathSplitList=perfectPath.split("/");
        for(int i=0;i<perfectPathSplitList.length()-1;i++){
            beforePath.append(perfectPathSplitList[i]);
            beforePath.append("/");
        }
        beforePath=beforePath+"plugin/resource";
        perfectPath=beforePath;
    }else{
        parentWindow->warnTips("完美平台","未启动，无法自动获取完美平台地址");
    }
}
T_Deploy::~T_Deploy(){

}
