#include "Headers/T_Manage.h"
#include "Headers/T_Deploy.h"
#include "AsulCFGManager.h"
#include <ElaToggleSwitch.h>
#include "Headers/F_Collector.h"
#include <ElaMessageBar.h>
#include <QFileSystemWatcher>

#define parentWindow dynamic_cast<Asul *>(this->parent())
T_Manage::T_Manage(QWidget *parent)
    : BaseScrollPage{parent}
{
    initWidget("管理","管理","开关已安装的 CFG");



    qDebug()<<"============T_Manage===========";
    T_Deploy *funcDeploy=new T_Deploy(this);
    funcDeploy->getAllPath();
    CFGPath=funcDeploy->CFGPath;
    Collector *sumAutoExec=new Collector(this);
    connect(this,&T_Manage::updateData,[&](){
        qDebug()<<"getInUpdateData";

        for (int i = 0; i < centerVLayout->count(); ++i) {
            QLayoutItem* item = centerVLayout->itemAt(i);

            if (!item) continue;
            // 检查是否是控件（QWidget）

            if (QWidget* widget = item->widget()) {
                qDebug() << "找到控件：" << widget->objectName();
                widget->deleteLater();
                delete widget;
            }
        }

        QFile autoexecCfg(CFGPath+"/autoexec.cfg");
        qDebug()<<"FIleLocation"<<CFGPath+"/autoexec.cfg";
        QString autoExecFileContent;

        if(autoexecCfg.exists()){
            QFileSystemWatcher *watcher=new QFileSystemWatcher(this);
            if(watcher->addPath(CFGPath+"/autoexec.cfg")){
                connect(watcher,&QFileSystemWatcher::fileChanged,[=](){

                    if(myChange){
                        myChange=false;
                    }else{
                        emit lockSwitch();
                        if(!lock){
                            lock=true;
                            ElaMessageBar::error(ElaMessageBarType::Top,"文件","AutoExec.cfg 已被外部修改！请重启面板管理",0x3f3f3f);
                        }
                    }

                });
            }else ElaMessageBar::warning(ElaMessageBarType::Top,"文件","无法监视 AutoExec.cfg 的变化",1000);

            if(autoexecCfg.open(QIODevice::ReadOnly)){
                autoExecFileContent=autoexecCfg.readAll();
            }else{
                parentWindow->errTips("管理","AutoExec文件无法打开");
                return;
            }
        }else{
            parentWindow->warnTips("管理","AutoEXEC 文件不存在!");
            return;
        }
        autoexecCfg.close();
        qDebug()<<autoExecFileContent;
        sumAutoExec = new Collector(this);
        connect(this,&T_Manage::textHasChanged,[=](){
            QFile saveTo(CFGPath+"/autoexec.cfg");
            saveTo.open(QIODevice::WriteOnly);
            QTextStream *saveStream=new QTextStream(&saveTo);
            *saveStream << sumAutoExec->getValues().join("\n");
            saveTo.close();
            delete saveStream;
            ElaMessageBar::success(ElaMessageBarType::Top,"成功","保存至AutoExec.cfg",1000);
        });
        foreach(QString line,autoExecFileContent.split("\n")){
            int col=sumAutoExec->addLabel(new ElaText(this));
            sumAutoExec->setValue(col,line);
            if(line.startsWith("exec")||line.startsWith("//exec")){

                qDebug()<<"HereIsExec"<<line;
                bool skip=false;
                QString funcName;
                for(int i=0;i<line.length();i++){
                    if(line[i]==' ') skip=true;
                    if(skip){
                        funcName.append(line[i]);
                    }
                }
                ElaToggleSwitch *funcSwitch=new ElaToggleSwitch(this);
                connect(this,&T_Manage::lockSwitch,[=](){
                    funcSwitch->setDisabled(true);
                });
                funcSwitch->setIsToggled(line.startsWith("exec"));
                QString pureArgu=line.replace("//exec","exec");
                ElaText *funcText=new ElaText(QString("开启 %1").arg(funcName),this);
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
                qDebug()<<"funcFile:"<<CFGPath+"/"+funcDirName+"/config.asuls";
                if(funcFile.exists()){
                    qDebug()<<"exist!";
                    if(funcFile.open(QIODevice::ReadOnly)){
                        content=funcFile.readAll();
                        funcFile.close();
                    }else ElaMessageBar::error(ElaMessageBarType::Top,"文件",funcDirName+"/config.asuls 打开失败",1000);
                }
                qDebug()<<"close!";
                QString CFGName,CFGVersion,CFGAuthor/*,CFGDetails*/;

                if(!content.isEmpty()){
                    foreach(QString File,content.split("\n")){
                        if(File.startsWith("[INI]")){
                            QStringList info=File.replace("\r","").replace("\n","").split(" ");
                            CFGName = info[1];
                            CFGVersion = info[2];
                            CFGAuthor = info[3];
                        }
                        // CFGDetails = info[4];
                    }
                    qDebug()<<"reserved!";
                    funcText->setText(funcText->text()+QString(" [名称:%1,版本:%2,作者:%3]").arg(CFGName).arg(CFGVersion).arg(CFGAuthor));
                }

                ElaScrollPageArea *funcArea=(GenerateArea(this,funcText,funcSwitch,false));
                centerVLayout->addWidget(funcArea);
            }
        }
        ElaText *previewT=new ElaText("预览 AutoExec.cfg 文件",this);
        QPlainTextEdit * previewText=new QPlainTextEdit(sumAutoExec->getValues().join("\n"),this);
        previewText->setMinimumHeight(400);
        previewText->setReadOnly(true);
        // ElaScrollPageArea *previewArea=GenerateArea(this,new ElaText("预览 AutoExec.cfg",this),previewText,false);
        connect(sumAutoExec,&Collector::valuesChanged,[=](){
            previewText->setPlainText(sumAutoExec->getValues().join("\n"));
        });
        QWidget *previewWidget=new QWidget(this);
        QVBoxLayout *previewLayout=new QVBoxLayout(previewWidget);
        previewLayout->addWidget(previewT);
        previewLayout->addWidget(previewText);
        previewShow?previewWidget->show():previewWidget->hide();
        connect(this,&T_Manage::changePreviewHide,[=](bool show){
            show?previewWidget->show():previewWidget->hide();
        });
        centerVLayout->addWidget(previewWidget);
        centerVLayout->addStretch();


    });
    emit updateData();
}
void T_Manage::update(){
    emit updateData();
}
void T_Manage::changeOccured(){
    emit changePreviewHide(previewShow);
    // update();
}
T_Manage::~T_Manage(){

}
