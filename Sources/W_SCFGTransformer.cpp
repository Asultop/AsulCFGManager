#include "Headers/W_SCFGTransformer.h"
#include "Headers/BaseScrollpage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include "Headers/BaseInclude.h"
#include "qapplication.h"
#include <QsLog.h>
#include <MarkdownHighlighter.h>
#include <ElaIconButton.h>

#define DBG(x) QLOG_DEBUG()<<"[Key] "<<#x<<" : "<<x;
using namespace QsLogging;
W_SCFGTransformer::W_SCFGTransformer(QWidget *parent)
    : ElaWidget{parent}
{
    this->setWindowButtonFlags(ElaAppBarType::CloseButtonHint|ElaAppBarType::MinimizeButtonHint);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    this->setIsFixedSize(true);\

    // this->setFixedSize(QSize(488,600));
    this->setWindowTitle("[SCFG] -> [CFG]");
    // connect(this,)
    this->setselfMove(false);
    connect(gSettings,&GlobalSettings::pParentRectChanged,[=](){
        if(this->getselfMove()==true) return;
        programMove=true;
        QRect * rect=gSettings->getParentRect();
        this->move(rect->x()+rect->width()+15,rect->y());
        this->setFixedSize(QSize(488,rect->height()));
        programMove=false;
    });
    geneFunc=new BaseScrollPage();
    geneFunc->hide();

    QVBoxLayout * centerVLayout=new QVBoxLayout(this);


    ElaPushButton *authorBtn=new ElaPushButton("elecCap1taL",this);
    authorBtn->setIcon(QIcon(":/pic/Pic/elecCap1tal_Github.png"));

    authorBtn->setToolTip("GitHub: https://github.com/eLecCap1taL/scfg");
    connect(authorBtn,&ElaPushButton::clicked,[=](){
        QDesktopServices::openUrl(QUrl("https://github.com/eLecCap1taL/scfg"));
    });

    centerVLayout->addWidget(geneFunc->GenerateArea(this,new ElaText(tr("此功能来自"),this),authorBtn));

    ElaPushButton *selectFileBtn=new ElaPushButton(tr("选择"),this);
    ElaPlainTextEdit *showEdit=new ElaPlainTextEdit(this);
    showEdit->setFixedHeight(240);
    showEdit->setReadOnly(true);

    connect(selectFileBtn,&ElaPushButton::clicked,[=](){
        QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("打开 SCFG 文件"),
            // QDir::homePath(),
            ".",
            tr("SCFG 文件 (*.scfg)")
            );
        if(!fileName.isEmpty()){

            auto gLocation=QApplication::applicationDirPath();
            if(QFile(gLocation+"/Instance.scfg").exists())
                QFile::remove(gLocation+"/Instance.scfg");
            if(QDir(gLocation+"/Instance").exists()){
                QDir rmd(gLocation+"/Instance");
                rmd.removeRecursively();
            }

            QFile::copy(fileName,gLocation+"/Instance.scfg");
            QProcess *process=new QProcess(this);
            process->setProgram(gLocation+"/scfg.exe");
            process->setWorkingDirectory(gLocation);
            process->setArguments(QStringList{
                "Instance.scfg"
            });
            process->start();
            process->waitForFinished(5000);
            QFile *CFGFile=new QFile(gLocation+"/Instance/cmd_1.cfg",this);
            DBG(gLocation+"/Instance/cmd_1.cfg");
            if(CFGFile->open(QIODevice::ReadWrite)){
                showEdit->clear();
                showEdit->appendPlainText(CFGFile->readAll());
                geneFunc->showSuccess(tr("成功"),tr("转换一个 SCFG 为 CFG"));
            }else{geneFunc->showErr(tr("失败"),CFGFile->errorString());}
        }
    });
    centerVLayout->addWidget(geneFunc->GenerateArea(this,new ElaText(tr("SCFG:"),this),selectFileBtn));
    centerVLayout->addWidget(showEdit);

    // QTextEdit *tipsText=new QTextEdit(tr("测试**Markdown**在**ElaText**上的~~应用~~"),this);
    // MarkdownHighlighter *hl=new MarkdownHighlighter(tipsText->document());
    // centerVLayout->addWidget(tipsText);
    centerVLayout->addStretch();
}

void W_SCFGTransformer::resizeEvent(QResizeEvent *event)
{
    badges++;
    if(!programMove && badges>5){
        this->setselfMove(true);
    }
    QWidget::resizeEvent(event);

}

void W_SCFGTransformer::moveEvent(QMoveEvent *event)
{
    badges++;
    if(!programMove && badges>4){
        this->setselfMove(true);
    }
    QWidget::moveEvent(event);
}

W_SCFGTransformer::~W_SCFGTransformer()
{
    delete geneFunc;
}
