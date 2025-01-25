#include "Headers/T_Deploy.h"

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
#define parentWindow dynamic_cast<Asul *>(this->parent())
T_Deploy::T_Deploy(QWidget *parent,QTemporaryDir *tempDir)
    : BaseScrollPage{parent}
{
    initWidget("部署 CFG","安装 CFG","选择符合格式的安装包\n我们会为您解析数据并自动安装到您的CFG目录下");
    if(tempDir!=nullptr)
        gLocation=tempDir;
    else
        gLocation=new QTemporaryDir();
    this->setAcceptDrops(true);
    //getAllPath();
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

    // ElaPushButton *analyFile=n\=]-[p0oijuhgbv c=-p809ou7yd xz765w ElaPushButton("分析文件",this);
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
    W_DragFrame *selectDragFrame = new W_DragFrame(this);

    connect(selectDragFrame, &W_DragFrame::fileDropped, this, [=](const QString &val){
        parentWindow->normTips("File",val);
        qDebug()<<val;
    });
    // selectFileLayout->addWidget(selectDragFrame);
    // selectDragFrame->setMinimumHeight(60);
    // addTopWidget(selectDragFrame);
    // DragFrame未完善
    selectDragFrame->hide();
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
            qDebug()<<"ZipFilePath"<<ZipFilePath;
            QString onlyFileName;
            foreach(QString val,ZipFilePath.split("/")){
                onlyFileName=val;
            }
            onlyFileName=onlyFileName.split(".")[0];
            AnalyseFile(onlyFileName);
        }
        else {
            parentWindow->normTips("文件","取消选择");
        }
    });



    this->addTopWidget(selectFileArea);

    centerVLayout->addStretch();
    flowLayout=new ElaFlowLayout(0,5,5);
    flowLayout->setIsAnimation(true);
    flowLayout->setContentsMargins(10,10,10,10);
    // flowLayout->setContentsMargins(5,5,20,30);
    centerVLayout->addLayout(flowLayout,2);


    //init deploywindow

}
void T_Deploy::AnalyseFile(QString FileName){

    Asul *window=parentWindow;
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

        window->warnTips("文件",QString("包过大 %1MB (超过 500 MB)").arg(zipFileInfo.size()/1024/1024));
        return;
    }
    QString UncompressFileLocation=gLocation->path().replace("/","\\")+"\\Analy\\"+FileName;

    UnCompressFiles(ZipFilePath,UncompressFileLocation);

    //Read Info

    static bool hasConnected=false;
    if(!hasConnected){
        hasConnected=true;
        connect(this,&T_Deploy::generateError,this,[=](QString content){
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
    // qDebug()<<A;
    // qDebug()<<B;
    process.close();
    return (A+B).isEmpty()?false:true;
}
void T_Deploy::generateScrollPageLayout(QString CFGFileLocation, ElaFlowLayout *cVLayout) {
    // FluentHiper *window=dynamic_cast<FluentHiper *>(parent());
    bool showable=true;
    if (CFGFileLocations.contains(ZipFilePath)) {
        parentWindow->errTips("文件", "已存在或无效的包: " + ZipFilePath);
        // return;
    }

    CFGFileLocations.append(ZipFilePath + "\n");
    qDebug()<<"CFGFileLocation:"<<CFGFileLocation;
    QFile *asulsFile=new QFile(CFGFileLocation +"/config.asuls");
    QString asulsFileNames;
    if(asulsFile->open(QIODevice::ReadOnly)){
        asulsFileNames=asulsFile->readAll();
        asulsFile->close();
    }
    else{
        emit generateError("Asuls 文件:"+asulsFile->errorString());
        return;
    }
    QStringList fileNames;
    QStringList singleNames;
    QString exeFile,dirName,helpFile;
    QString CFGName,CFGAuthor,CFGVersion,CFGDetails;
    bool INI=false,DIR=false,EXE=false,HELP=false;
    foreach(QString File,asulsFileNames.split("\n")){
        if(File.startsWith("[INI]")){
            QStringList info=File.replace("\r","").replace("\n","").split(" ");
            if(info.length()<5){
                emit generateError("Asuls 文件(config.asuls): [INI]错误");
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
            QStringList exeList=File.split(" ");
            if(exeList.length()<2){
                emit generateError("Asuls 文件(config.asuls): [EXE]错误");
                showable=false;
                return;
            }
            exeFile=exeList[1];
            EXE=true;
            continue;
        }
        else if(File.startsWith("[DIR]")){
            QStringList dirList=File.split(" ");
            if(dirList.length()<2){
                emit generateError("Asuls 文件(config.asuls): [DIR]错误");
                showable=false;
            }
            dirName=dirList[1];
            DIR=true;
            continue;
        }
        else if(File.startsWith("[HELP]")){
            QStringList helpList=File.split(" ");
            if(helpList.length()<2){
                emit generateError("Asuls 文件(config.asuls): [HELP]错误");
                showable=false;
            }
            helpFile=helpList[1];
            qDebug()<<"HelpFileName:"<<helpFile;
            HELP=true;
            continue;
        }
        QString fileN=File.replace("\r","").replace("\n","").append(".asul");
        fileNames.append(fileN);
        if(fileN.contains("/")){
            QString tplet;
            foreach(QString val,File.split("/")){

                tplet=val;
            }
            //createDir
            QDir mkp(CFGFileLocation);
            mkp.mkpath(fileN);
            // createDirectories();

            singleNames.append(tplet);
        }else{
            singleNames.append(fileN);
        }
    }
    qDebug()<<fileNames;
    qDebug()<<CFGName<<CFGVersion<<CFGAuthor<<CFGDetails;
    //generate Window
    if(!INI||!DIR){
        emit generateError(QString("Asuls 文件(config.asuls): 未找到%1 %2").arg(!INI?"[INI]":"").arg(!DIR?"[DIR]":""));
        showable=false;
    }
    qDebug()<<"page will be "<<fileNames.count();
    ElaWindow *DeployWindow=new ElaWindow(this);
    DeployWindow->setUserInfoCardPixmap(QPixmap("://pic/Pic/favicon.png"));
    DeployWindow->setUserInfoCardTitle("CFG LinkWard");
    DeployWindow->setUserInfoCardSubTitle("by Alivn@Asul.top");
    DeployWindow->setUserInfoCardVisible(true);
    DeployWindow->setWindowTitle("部署 CFG");
    DeployWindow->setWindowButtonFlag(ElaAppBarType::ThemeChangeButtonHint,false);
    DeployWindow->setWindowButtonFlag(ElaAppBarType::StayTopButtonHint,false);

    //generate end
    ElaPopularCard *CFGBreifInfo = new ElaPopularCard(this);
    CFGBreifInfo->setCardPixmap(QPixmap(":/pic/Pic/CS2.png"));
    CFGBreifInfo->setTitle(CFGName);
    CFGBreifInfo->setSubTitle("@" + CFGAuthor);
    CFGBreifInfo->setInteractiveTips("队列中");
    CFGBreifInfo->setDetailedText("版本:"+CFGVersion+"\n"+CFGDetails);
    CFGBreifInfo->setCardFloatPixmap(QPixmap(":/pic/Pic/favicon.png"));
    this->selectedFilePath->setText(ZipFilePath);
    cVLayout->addWidget(CFGBreifInfo);
    // DeployWindow->show();

    emit changeHideAShowState(false);
    connect(DeployWindow,&ElaWindow::windowClosed,[=](){
        emit changeHideAShowState(true);
        qDebug()<<"closed!";
    });
    connect(CFGBreifInfo, &ElaPopularCard::popularCardButtonClicked, this, [=](){
        DeployWindow->show();
        emit changeHideAShowState(false);
    });

    QRegularExpression regex("(\\w+)|(\"(?:\\\\.|[^\\\\\"])*\")");
    //QRegularExpression regex2("\"((?:\\\\.|[^\"])*)\"\\s*and\\s*\"((?:\\\\.|[^\"])*)\"");
    QRegularExpression regex2("\"((?:[^\"\\\\]|\\\\.)*?)\"");
    QRegularExpression regex3("\"([^\"]+)\"\\s*\"([^\"]+)\"");
    QRegularExpression regex4("\"([^\"]+)\"\\s*\"([^\"]+)\"\\s*\"([^\"]+)\"\\s*\"([^\"]+)\"");
    pageNumMap[CFGBreifInfo]=fileNames.count();
    QStringList ElaPageNodeS={};
    ElaPageNodeS.resize(fileNames.count()+1);
    foreach (QString asulFile, fileNames) {

        QString asulFileName=singleNames[fileNames.indexOf(asulFile)];
        Collector * sumContent=new Collector(CFGBreifInfo);

        BaseScrollPage *CFGSettingPage=new BaseScrollPage(this);

        CFGSettingPage->initWidget(asulFileName,asulFileName+" 项目","解析完成 ");
        ElaText *tips=new ElaText("若键盘映射无所需按键或确认后与所需不符\n请在第二个空中手动输入");
        tips->setTextPixelSize(20);
        tips->hide();
        QFont fontTip(tips->font());
        fontTip.setBold(true);
        tips->setFont(fontTip);
        CFGSettingPage->addTopWidget(tips);
        CFGSettingPage->setWindowTitle(asulFileName+" 配置");
        QVBoxLayout *Allkey=new QVBoxLayout(DeployWindow);
        QFile * configFile=new QFile(CFGFileLocation+"/"+asulFile);
        QString contents;

        if(configFile->open(QIODevice::ReadOnly)){
            contents=configFile->readAll();
            configFile->close();
        }
        else{
            emit generateError("Asul 文件("+asulFile+"): "+configFile->errorString());
            CFGBreifInfo->hide();
            showable=false;
            return;
        }

        QStringList lines = contents.split("\n");
        for (int i = 0; i < lines.size(); ++i) {

            QString line = lines[i];
            line=line.replace("\r","").replace("\n","");
            if(line.startsWith("key") || line.startsWith("must")){

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
                        emit generateError("在 "+asulFile+" 中"+QString::number(i)+":key无法找到完整参数");
                        showable=false;
                        return;
                    }
                }
                QString type=result[0];
                QString defaultKey=result[1];
                QString func=result[2];
                QString title=result[3];
                QString details=result[4];

                //addButtonToDeployWindow
                ElaScrollPageArea *configArea=new ElaScrollPageArea(DeployWindow);
                QHBoxLayout * configHLayout=new QHBoxLayout(configArea);
                QVBoxLayout* configVLayout=new QVBoxLayout();
                if(line.startsWith("must")) title=title+"(根据作者定义此项无法修改)";
                ElaText *titleText=new ElaText(title);
                ElaText *detailText=new ElaText(details.replace("\\n","\n"));
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
                detailText->setProperty("num",colNum);
                connect(configKeyLine,&ElaLineEdit::textChanged,[=](QString val){
                    configKeyBinder->setBinderKeyText(val);
                    qDebug()<<"get in configKeyLine";
                    if(val=="Up") val="uparrow";
                    else if(val=="Down") val="downarrow";
                    else if(val=="Left") val="leftarrow";
                    else if(val=="Right") val="rightarrow";
                    else if(val=="Return") val="enter";
                    val=val.toLower();

                    int myNum=detailText->property("num").toInt();

                    sumContent->setValue(myNum , QString("bind %1 %2").arg(val).arg(func));

                });
                connect(configKeyBinder,&ElaKeyBinder::binderKeyTextChanged,this,[=](QString val){
                    qDebug()<<"get in binderKeyTextChanged";
                    if(val=="Up") val="uparrow";
                    else if(val=="Down") val="downarrow";
                    else if(val=="Left") val="leftarrow";
                    else if(val=="Right") val="rightarrow";
                    else if(val=="Return") val="enter";
                    val=val.toLower();

                    int myNum=detailText->property("num").toInt();
                    sumContent->setValue(myNum,QString("bind %1 %2\n").arg(val).arg(func));
                    configKeyLine->setText(val);
                });

                //checkForNonRepetit


            }
            else if(line.startsWith("func")){
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
                    }
                    else{
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
                int switchNums=result[4].toInt();
                QStringList switchContent,switchDetail;
                for(int j=1;j<switchNums+1;j++){
                    int index=i+j;
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
                        // qDebug() << "键:" << key << "值:" << value;
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
                ElaComboBox *configComboBox=new ElaComboBox(DeployWindow);
                configComboBox->addItems(switchDetail);
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

                connect(configComboBox,&ElaComboBox::currentIndexChanged,this,[=](int index) mutable{
                    // const_cast<QString&>(sumContent[pageNum][myNumber])=switchContent[index];
                    qDebug()<<"get in ";
                    int myNum=detailText->property("num").toInt();
                    qDebug()<<"mynum"<<myNum;
                    sumContent->setValue(myNum,switchContent[index]);
                });
            }
            else if(line.startsWith("text")){

                QRegularExpressionMatch match = regex2.match(line);
                if(match.hasMatch()) {
                    QString quotedContent = match.captured(1);  // 获取第一个捕获组(引号内的内容)
                    ElaText *tplat=new ElaText(quotedContent);
                    tplat->hide();
                    sumContent->addLabel(tplat);
                }else{
                    emit generateError("在 "+asulFile+" 中"+QString::number(i)+":text无法找到完整参数");
                    showable=false;
                    return;
                }
            }
            else if(line.startsWith("line")){
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

                    qDebug() << "标题:" << title << "详情:" << detail << "A:" << A << "B:" << B;
                }else{
                    emit generateError("在 "+asulFile+" 中"+QString::number(i)+":line无法找到完整参数");
                    showable=false;
                    return;
                }
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
                detailText->setProperty("num",colNum);
                sumContent->setValue(colNum,A.arg(B));
                connect(configLine,&ElaLineEdit::textChanged,[=](QString val){
                    int myNum=detailText->property("num").toInt();
                    sumContent->setValue(myNum,A.arg(val));
                });

                configHLayout->addWidget(configLine);

                Allkey->addWidget(configArea);
                Allkey->addSpacing(5);
            }
        }
        qDebug()<<asulFileName<<"end";
        QString CFGFileName=asulFile.replace(".asul",".cfg");
        CFGSettingPage->centerVLayout->addLayout(Allkey,10);
        ElaPushButton *deployButton=new ElaPushButton("部署",DeployWindow);
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
        connect(deployButton,&ElaPushButton::clicked,[=](){
            QString CFGFileName=deployButton->property("CFGFileName").toString();
            QString CFGFileLocation=deployButton->property("CFGFileLocation").toString();
            QString CFGAuthor=deployButton->property("CFGAuthor").toString();
            QString CFGName=deployButton->property("CFGName").toString();
            QString CFGVersion=deployButton->property("CFGVersion").toString();
            QString CFGHelpFile=deployButton->property("helpFile").toString().replace("\r","");
            qDebug()<<"getin CFGHelpFile"<<CFGHelpFile;

            ElaContentDialog *dialog=new ElaContentDialog(DeployWindow);
            dialog->setLeftButtonText("返回");
            dialog->setMiddleButtonText("取消");
            dialog->setRightButtonText("确认");
            connect(dialog,&ElaContentDialog::rightButtonClicked,this,[=](){

                QString CFGDirName=deployButton->property("CFGDirName").toString().replace("\r","");
                QString CFGExeFile=deployButton->property("exeFile").toString().replace("\r","");
                //Todo : 部署CFG包，此处可用参数:
                //CFGFileLocation + resourceFile; //解压CFG整体包文件夹,resource.zip文件地址，若无则跳过
                //CFGFileLocation + MainCFGFile; // info.json 中 Main.cfg的地址
                //perfectPath; //完美Resource路径
                //steamPath; //steam路径.replace("\\","/")
                //CFGPath; //CS2,CFG路径;
                // DeployWindow->close();
                //sumContent[realNum];  //存储了所有更改的值 可直接添加换行保存为preference.cfg
                //settings_file; //不是一个标准的文件目录，而是需要生成的配置文件
                qDebug()<<"write to "<<CFGFileLocation+"/"+CFGFileName;;
                QFile *preferrenceFile=new QFile(CFGFileLocation+"/"+CFGFileName);
                if (preferrenceFile->exists()) {
                    // 删除文件
                    if (!preferrenceFile->remove()) {
                        qWarning() << "无法删除文件:" << preferrenceFile->fileName();
                        return;
                    }
                }
                qDebug()<<"CFGFileLocation: "<<CFGFileLocation;
                if(!preferrenceFile->open(QIODevice::ReadWrite)){
                    qDebug()<<"Error has occured"<<CFGFileLocation+"/"+CFGFileName;
                }
                QFileInfo *CFGFileInfo=new QFileInfo(CFGFileLocation+"/"+CFGFileName);

                QTextStream *preferrenceStream=new QTextStream(preferrenceFile);
                *preferrenceStream<<"// ====== 生成的文件 =======\n";
                *preferrenceStream<<QString("//=这个 %1 文件由 Asul-cfgManager(AM) 根据配置自动生成 \n").arg(CFGFileInfo->fileName());
                *preferrenceStream<<"\n//==这个 配置文件 从哪儿来的?\n";
                *preferrenceStream<<"//CFG 制作者: "<<CFGAuthor<<"\n";
                *preferrenceStream<<"//CFG 名称: "<<CFGName<<"\n";
                *preferrenceStream<<"//CFG 版本:  "<<CFGVersion<<"\n";
                *preferrenceStream<<"//==CFG 详细 结束\n\n";

                foreach (QString val, sumContent->getValues()) {
                    *preferrenceStream<<val<<"\n";
                }

                *preferrenceStream<<"\n\n//==参数结束\n";
                *preferrenceStream<<"//AM 是由 Alivn开发的部署 CS2 CFG 的程序,旨在为CFG制作者提供更方便的分发服务\n";
                *preferrenceStream<<"//开发者:Github(https://github.com/AsulTop),WebSite(http://www.asul.top)";
                *preferrenceStream<<"//"<<QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

                preferrenceFile->close();
                delete preferrenceStream;

                CFGBreifInfo->hide();
                CFGSettingPage->hide();
                // parentWindow->succTips(DeployWindow,"CFG","一项配置完成部署");
                // QDesktopServices::openUrl(CFGFileLocation+"/"+CFGFileName);

                pageNumMap[CFGBreifInfo]--;
                qDebug()<<"then page num:"<<pageNumMap[CFGBreifInfo];
                if(pageNumMap[CFGBreifInfo]==0){
                    //配置结束：转移文件夹到CFGFile下
                    if(autoDir)
                        QDesktopServices::openUrl(QUrl::fromLocalFile(CFGPath+"/"+CFGDirName));
                    if(HELP&&autoHelp){
                        qDebug()<<"getReady CFGHelpFile"<<QString::fromUtf8(CFGHelpFile.toUtf8());
                        QDesktopServices::openUrl(QUrl::fromLocalFile(CFGPath+"/"+CFGDirName+"/"+CFGHelpFile));
                    }
                    QString REF=manageDirectory(CFGPath,CFGDirName.replace("\r",""),CFGFileLocation);
                    if(REF!=""){
                        parentWindow->warnTips(parentWindow,"配置出错","在复制"+REF+"文件中出错");
                        CFGBreifInfo->hide();
                        DeployWindow->close();
                        DeployWindow->deleteLater();
                        return;
                    }
                    if(!exeFile.isEmpty()){
                        if(EXE&&autoExe){
                            qDebug()<<"将运行:"<<exeFile<<" at "<<CFGPath+"/"+CFGDirName+"/"+exeFile;

                            QString execFinalFile=CFGPath+"/"+CFGDirName+"/"+exeFile;
                            QProcess *process =new QProcess();
                            QFileInfo fileInfo(execFinalFile);
                            QFile *AutoInstall=new QFile(CFGFileLocation+"/"+"AUTOINSTALL.bat");
                            if(!AutoInstall->open(QIODevice::ReadWrite)){
                                parentWindow->warnTips(DeployWindow,"错误","AUTOINSTALL.bat 创建失败");
                            }
                            QTextStream *AutoStream=new QTextStream(AutoInstall);
                            *AutoStream<<"cd "<<fileInfo.absolutePath()<<"\n";
                            *AutoStream<<"start \"\" "<<fileInfo.fileName()<<"\n";
                            AutoInstall->close();

                            process->start(CFGFileLocation+"/AUTOINSTALL.bat");
                            qDebug()<<"fileInfo.fileName()"<<fileInfo.fileName();
                            if (!process->waitForFinished()) {
                                qDebug()<<"Error() "<<process->readAllStandardError();
                                qDebug()<<"Output() "<<process->readAllStandardOutput();
                                qWarning() << "无法运行文件:" << fileInfo.absoluteFilePath()<<process->errorString();
                                parentWindow->warnTips(parentWindow,"[EXE]","自动"+exeFile+"运行失败，请查看帮助文档");
                            }
                            connect(process,&QProcess::finished,[=](){
                                process->deleteLater();
                            });
                        }

                        delete CFGFileInfo;
                    }

                    //配置段结束
                    DeployWindow->close();
                    emit changeHideAShowState(true);
                    DeployWindow->deleteLater();
                }else{
                    qDebug()<<ElaPageNodeS;
                    DeployWindow->navigation(ElaPageNodeS[pageNumMap[CFGBreifInfo]]);
                    // ElaNavigationRouter::getInstance()->navigationRouteBack();
                    parentWindow->succTips(DeployWindow,"完成","配置中还有 "+QString::number(pageNumMap[CFGBreifInfo])+" 个");
                }
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
        // CFGSettingPage->addTopWidget(deployButton);
        if(!showable){
            CFGBreifInfo->hide();
        }
        CFGSettingPage->centerVLayout->addWidget(deployButton);

        DeployWindow->setNavigationBarDisplayMode(ElaNavigationType::Minimal);
        DeployWindow->setFixedSize(QSize(1000,600));
        DeployWindow->makeIndependence();
        // QString pageKey;
        DeployWindow->addPageNode(asulFile,CFGSettingPage,ElaIconType::ArrowDownToBracket);
        DeployWindow->navigation(CFGSettingPage->property("ElaPageKey").toString());
        ElaPageNodeS[pageNumMap[CFGBreifInfo]-1]=CFGSettingPage->property("ElaPageKey").toString();

    }



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
        perfectPath="";
        parentWindow->warnTips(parentWindow,"完美平台","未启动，无法自动获取完美平台地址");
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
    qDebug()<<CFGPath<<Qt::endl<<CFGDirName<<Qt::endl<<CFGFileLocation;
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
T_Deploy::~T_Deploy(){

}
