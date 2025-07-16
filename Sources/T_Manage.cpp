#include "Headers/T_Manage.h"
#include "Headers/T_Deploy.h"
#include "AsulCFGManager.h"
#include <ElaToggleSwitch.h>
#include "Headers/F_Collector.h"
#include <ElaMessageBar.h>
#include <QFileSystemWatcher>
#include <ElaListView.h>
#include <QsLog.h>
using namespace QsLogging;
// #define parentWindow dynamic_cast<Asul *>(this->parent())
T_Manage::T_Manage(QWidget *parent,bool readPolicy)
    : BaseScrollPage{parent}
{
    initWidget(tr("管理"),tr("管理"),tr("开关已安装的 CFG"));
    if(!readPolicy){
        centerVLayout->addWidget(GenerateArea(this,new ElaText(tr("请重启面板同意我们阅读 AutoExec.CFG 来管理"),this),new QWidget(this)));
        return;
    }

    T_Deploy *funcDeploy=new T_Deploy(this);
    funcDeploy->getAllPath();
    CFGPath=funcDeploy->CFGPath;

    Collector *sumAutoExec=new Collector(this);
    QVBoxLayout *funcLayout=new QVBoxLayout();

    autoexecCfg = new QFile(CFGPath+"/autoexec.cfg");
    centerVLayout->addLayout(funcLayout);
    QLOG_DEBUG()<<"[Manage] FIleLocation"<<CFGPath+"/autoexec.cfg";

    if(autoexecCfg->exists()){

        QFileSystemWatcher *watcher=new QFileSystemWatcher(this);
        if(watcher->addPath(CFGPath+"/autoexec.cfg")){
            connect(watcher,&QFileSystemWatcher::fileChanged,[&](){
                // myChange=true;
                if(myChange){
                    myChange=false;
                }else{
                    emit lockSwitch();
                    if(!lock){
                        lock=true;
                        //ElaMessageBar::error(ElaMessageBarType::Top,tr("文件"),tr("AutoExec.cfg 已被外部修改！请重启面板管理"),3000,parent);
                        showErr(tr("文件"),tr("AutoExec.cfg 已被外部修改！请重启面板管理"),0x3f3f3f3f);
                    }
                    // emit updateData();
                }

            });
        }else showWarn(tr("文件"),tr("无法监视 AutoExec.cfg 的变化"));

        if(autoexecCfg->open(QIODevice::ReadOnly)){
            autoExecFileContent=autoexecCfg->readAll();
        }else{
            // parentWindow->errTips(tr("管理"),tr("AutoExec文件无法打开"));
            showErr(tr("管理"),tr("AutoExec文件无法打开"));
        }
    }else{
        centerVLayout->addWidget(GenerateArea(this,new ElaText(tr("未找到存在的 AutoExec 文件"),this),new QWidget(this)));
    }
    if(autoexecCfg->exists()){
        autoexecCfg->close();
        QLOG_DEBUG()<<"[Manage] autoExecFileContent:"<<autoExecFileContent;
        funcLayout=new QVBoxLayout();
        funcWidget=new QWidget(this);
        funcWidget->setLayout(funcLayout);
        ElaText *previewT=new ElaText(tr("预览 AutoExec.cfg 文件"),this);
        ElaPlainTextEdit * previewText=new ElaPlainTextEdit(this);

        previewText->setMinimumHeight(400);
        previewText->setReadOnly(true);
        // ElaScrollPageArea *previewArea=GenerateArea(this,new ElaText("预览 AutoExec.cfg",this),previewText,false);

        QWidget *previewWidget=new QWidget(this);
        QVBoxLayout *previewLayout=new QVBoxLayout(previewWidget);
        previewLayout->addWidget(previewT);
        previewLayout->addWidget(previewText);
        gSettings->getPreviewShow()?previewWidget->show():previewWidget->hide();
        connect(gSettings,&GlobalSettings::pPreviewShowChanged,[=](){
            gSettings->getPreviewShow()?previewWidget->show():previewWidget->hide();
        });

        sumAutoExec = new Collector(this);
        connect(sumAutoExec,&Collector::valuesChanged,[=](){
            previewText->setPlainText(sumAutoExec->getValues().join("\n"));
        });
        connect(this,&T_Manage::textHasChanged,[=](){
            QFile saveTo(CFGPath+"/autoexec.cfg");
            saveTo.open(QIODevice::WriteOnly);
            QTextStream *saveStream=new QTextStream(&saveTo);
            foreach(QString val,sumAutoExec->getValues()){
                if(!val.isEmpty()){
                    *saveStream<<val<<"\n";
                }
            }
            // *saveStream << sumAutoExec->getValues().join("\n");
            saveTo.close();
            delete saveStream;
            ElaMessageBar::success(ElaMessageBarType::Top,tr("成功"),tr("保存至AutoExec.cfg"),getReadTime(tr("保存至AutoExec.cfg")),parent);
        });
        foreach(QString line,autoExecFileContent.split("\n")){
            line=line.replace("\r","");

            int col=sumAutoExec->addLabel(new ElaText(this));
            sumAutoExec->setValue(col,line);

            if(line.startsWith("exec")||line.startsWith("//exec")){
                QLOG_DEBUG()<<"[EXEC] Part Parsing";

                bool skip=false;
                QString funcName;
                for(int i=0;i<line.length();i++){
                    if(line[i]==' ') skip=true;
                    if(skip){
                        funcName.append(line[i]);
                    }
                }
                funcName=funcName.simplified();
                QLOG_DEBUG()<<"[EXEC] funcName:"<<funcName.simplified();
                if(funcName.simplified().isEmpty()) continue;
                ElaToggleSwitch *funcSwitch=new ElaToggleSwitch(this);
                ElaPushButton *deleteButton=new ElaPushButton(tr("删除"),this);
                connect(this,&T_Manage::lockSwitch,[=](){
                    funcSwitch->setDisabled(true);
                    deleteButton->setDisabled(true);
                });
                funcSwitch->setIsToggled(line.startsWith("exec"));
                QString pureArgu=line.replace("//exec","exec");
                ElaText *funcText=new ElaText(QString(tr(": %1")).arg(funcName),this);
                funcText->setTextPixelSize(20);
                funcText->setProperty("num",col);
                connect(funcSwitch,&ElaToggleSwitch::toggled,[=](bool toggle){
                    int colNum=funcText->property("num").toInt();
                    myChange=true;
                    QString pureArgu1=pureArgu;
                    if(!toggle){
                        sumAutoExec->setValue(colNum,pureArgu1.replace("exec","//exec"));
                    }else sumAutoExec->setValue(colNum,pureArgu);
                    emit textHasChanged();

                });
                QString funcDirName=funcName.split("/")[0].replace(" ","");
                QFile funcFile(CFGPath+"/"+funcDirName+"/config.asuls");
                QString content;
                QLOG_DEBUG()<<"[EXEC] funcFile:"<<CFGPath+"/"+funcDirName+"/config.asuls";
                if(funcFile.exists()){
                    QLOG_DEBUG()<<"exist!";
                    if(funcFile.open(QIODevice::ReadOnly)){
                        content=funcFile.readAll();
                        funcFile.close();
                    }else ElaMessageBar::error(ElaMessageBarType::Top,tr("文件"),funcDirName+tr("/config.asuls 打开失败"),getReadTime(funcDirName+tr("/config.asuls 打开失败")),parent);
                }
                QString CFGName,CFGVersion,CFGAuthor,CFGIcon/*,CFGDetails*/;

                if(!content.isEmpty()){
                    foreach(QString File,content.split("\n")){
                        if(File.startsWith("[INI]")){
                            QStringList info=File.replace("\r","").replace("\n","").split(" ");
                            CFGName = info[1];
                            CFGVersion = info[2];
                            CFGAuthor = info[3];
                        }
                        if(File.startsWith("[PNG]")){
                            CFGIcon=CFGPath+"/"+funcDirName+"/"+File.remove("\r").remove("\n").split(" ")[1];
                        }
                        // CFGDetails = info[4];
                    }
                    funcText->setText(funcText->text()+QString(tr(" [名称:%1,版本:%2,作者:%3]")).arg(CFGName).arg(CFGVersion).arg(CFGAuthor));
                }
                ElaText *imageCard = new ElaText(this);
                imageCard->setAutoFillBackground(true);
                imageCard->setFixedSize(QSize(50,50));
                QPixmap img(":/pic/Pic/CS2.png");
                if(!CFGIcon.isEmpty()){
                    qDebug()<<"CFGIcon: "<<CFGIcon;
                    img=QPixmap(CFGIcon);
                }
                QPixmap simg = img.scaled(imageCard->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);


                imageCard->setPixmap(simg);
                QHBoxLayout *titleLayout=new QHBoxLayout();
                titleLayout->addWidget(imageCard);
                titleLayout->addWidget(funcText);
                QHBoxLayout *funcLayout1=new QHBoxLayout();
                funcLayout1->addWidget(deleteButton);
                funcLayout1->addWidget(funcSwitch);

                deleteButton->setMaximumWidth(funcSwitch->width()*2);
                ElaScrollPageArea *funcArea=(GenerateArea(this,titleLayout,funcLayout1,false));
                connect(deleteButton,&ElaPushButton::clicked,[=](){
                    bool ret=askDialog(tr("条目"),"**"+tr("删除后将导致这个 CFG 需要重新启用且无法在此面板中开关")+"**");
                    if(ret){
                        funcArea->hide();
                        sumAutoExec->setValue(funcText->property("num").toInt(),"");
                        myChange=true;
                        emit textHasChanged();
                    }
                });
                if(/*!allKeyHere.contains(line)*/true){
                    funcLayout->addWidget(funcArea);
                    allKeyHere.append(line);
                }
                else {
                    funcSwitch->setIsToggled(false);
                    funcArea->hide();
                    sumAutoExec->setValue(funcText->property("num").toInt(),pureArgu.replace("exec","//exec"));
                }
                QLOG_DEBUG()<<"[Manage] AllKeyHere: "<<allKeyHere;
            }
        }
        centerVLayout->addWidget(funcWidget);
        centerVLayout->addWidget(previewWidget);
        centerVLayout->addStretch();

    }
    ElaPushButton *openFile=new ElaPushButton(tr("打开 AutoExec.cfg 文件"),this);
    openFile->setMaximumWidth(200);
    connect(openFile,&ElaPushButton::clicked,[=](){
        QDesktopServices::openUrl(QUrl::fromLocalFile(CFGPath+"/autoexec.cfg"));
    });
    this->addTopWidget(openFile);
}
void T_Manage::clearLayout(QVBoxLayout* layout) {
    QLayoutItem* item;
    while (item = layout->takeAt(0)) {  // 从索引 0 开始遍历
        if (QWidget* widget = item->widget()) {
            // 删除控件（如果动态分配必须手动删除）
            delete widget;
        } else if (QLayout* childLayout = item->layout()) {
            // 递归清理子布局
            clearLayout(qobject_cast<QVBoxLayout*>(childLayout));
        }
        // 删除布局项
        delete item;
    }
}

T_Manage::~T_Manage(){

}
