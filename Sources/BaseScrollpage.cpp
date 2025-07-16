#include "Headers/BaseScrollpage.h"
#include "ElaContentDialog.h"
#include "ElaMessageBar.h"
#include "Headers/BaseInclude.h"
#include "markdownhighlighter.h"
#include "QsLog.h"

using namespace QsLogging;
BaseScrollPage::BaseScrollPage(QWidget *parent)
    : ElaScrollPage{parent}
{
    debugWidget = new QWidget(this);
    centralWidget = new QWidget(this);
    // centralWidget->show();

    centerVLayout = new QVBoxLayout(centralWidget);

    centerVLayout->setContentsMargins(5,15,20,30);
    // centerVLayout->setContentsMargins(0,0,0,0);


}
BaseScrollPage::~BaseScrollPage(){

}

void BaseScrollPage::showSuccess(QString title,QString content,int mSec){
    ElaMessageBar::success(ElaMessageBarType::Top,title,content,mSec==-1?BaseScrollPage::getReadTime(content):mSec);
}
void BaseScrollPage::showErr(QString title,QString content,int mSec){
    ElaMessageBar::error(ElaMessageBarType::Top,title,content,mSec==-1?BaseScrollPage::getReadTime(content):mSec);
}
void BaseScrollPage::showInfo(QString title,QString content,int mSec){
    ElaMessageBar::information(ElaMessageBarType::Top,title,content,mSec==-1?BaseScrollPage::getReadTime(content):mSec);
}
void BaseScrollPage::showWarn(QString title,QString content,int mSec){
    ElaMessageBar::warning(ElaMessageBarType::Top,title,content,mSec==-1?BaseScrollPage::getReadTime(content):mSec);
}

double BaseScrollPage::getReadTime(QString context){
    QFont font("Microsoft YaHei"); // 获取字体
    QFontMetrics metrics(font); // 使用字体创建字体度量对象
    int textWidth = metrics.horizontalAdvance(context); // 获取文本宽度
    int screenWidth = window()->width(); // 获取屏幕宽度（或阅读区域宽度）

    // 计算阅读时间（秒）
    double readingTimeInSeconds = static_cast<double>(textWidth) / screenWidth * 60 * charactersPerMinute;
    QLOG_DEBUG()<<"[Read_Time]  Context: "<<context<<" Time(ms): "<<readingTimeInSeconds;
    return readingTimeInSeconds;
}
void BaseScrollPage::initWidget(QString title,QString subTitle,QString content){
    setWindowTitle(title);
    centralWidget->setWindowTitle(subTitle);
    createCustomWidget(content);
    addCentralWidget(centralWidget,true,true,0);
}

void BaseScrollPage::showSuccEx(QString title, QString content, int mSec)
{
    BaseScrollPage *TipFunc=new BaseScrollPage();
    TipFunc->showSuccess(title,content,mSec);
    TipFunc->deleteLater();
    delete TipFunc;
}

void BaseScrollPage::showErrEx(QString title, QString content, int mSec)
{
    BaseScrollPage *TipFunc=new BaseScrollPage();
    TipFunc->showErr(title,content,mSec);
    TipFunc->deleteLater();
    delete TipFunc;
}

void BaseScrollPage::showInfoEx(QString title, QString content, int mSec)
{
    BaseScrollPage *TipFunc=new BaseScrollPage();
    TipFunc->showInfo(title,content,mSec);
    TipFunc->deleteLater();
    delete TipFunc;
}

void BaseScrollPage::showWarnEx(QString title, QString content, int mSec)
{
    BaseScrollPage *TipFunc=new BaseScrollPage();
    TipFunc->showWarn(title,content,mSec);
    TipFunc->deleteLater();
    delete TipFunc;
}

void BaseScrollPage::createCustomWidget(QString desText)
{
    // 顶部元素
    QWidget* customWidget = new QWidget(this);
    ElaText* subTitleText = new ElaText(this);
    subTitleText->setText("AsulCFGManager @ Asul.top");
    subTitleText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    subTitleText->setTextPixelSize(11);

    ElaText* descText = new ElaText(this);
    descText->setText(desText);
    descText->setTextPixelSize(13);

    QVBoxLayout* topLayout = new QVBoxLayout(customWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->addWidget(subTitleText);
    topLayout->addSpacing(5);
    topLayout->addWidget(descText);
    setCustomWidget(customWidget);
}
void BaseScrollPage::addTopWidget(QWidget *widget){
    QWidget *nowWidget=getCustomWidget();
    QLayout *topLayout=nowWidget->layout();
    topLayout->addWidget(widget);
    nowWidget->setLayout(topLayout);
    setCustomWidget(nowWidget);
}
ElaScrollPageArea * BaseScrollPage::GenerateArea(QWidget *parent,ElaText * title, QWidget * widget, bool convert){
    ElaScrollPageArea * Area=new ElaScrollPageArea(parent);
    QHBoxLayout * HLayout=new QHBoxLayout(Area);
    title->setMinimumHeight(45);
    title->setTextPixelSize(15);

    if(convert){
        HLayout->addWidget(title,7);
        HLayout->addWidget(widget,3);
        widget->setMinimumHeight(45);
        widget->setMinimumWidth(300);
    }else{
        HLayout->addWidget(title);
        HLayout->addWidget(widget);
    }

    return Area;
}
bool BaseScrollPage::askDialog(QWidget *parent,QString title,QString content){
    ElaContentDialog *askWatch=new ElaContentDialog(parent,false);
    QWidget * askWatchWidget=new QWidget(parent);
    ElaText *askText=new ElaText(title,parent);
    ElaPlainTextEdit *askLine =new ElaPlainTextEdit(parent);
    QVBoxLayout *askLayout=new QVBoxLayout();
    askLayout->addSpacing(5);
    askLayout->addWidget(askText,40);
    askLayout->addSpacing(10);
    askLayout->addWidget(askLine,60);
    askWatchWidget->setLayout(askLayout);
    askLine->setMinimumHeight(45);
    QFont tmFnt(askLine->font());
    tmFnt.setPixelSize(15);
    askLine->setFont(tmFnt);

    askLine->setReadOnly(true);
    askLine->appendPlainText(content);

    askText->setTextPointSize(20);
    askText->setTextPixelSize(20);

    askWatch->setRightButtonText(tr("是"));
    askWatch->setMiddleButtonText("");

    askWatch->setLeftButtonText(tr("否"));

    MarkdownHighlighter *highLighter=new MarkdownHighlighter(askLine->document(),MarkdownHighlighter::None);
    // highLighter->setDocument(textBrowser->document());
    highLighter->clearDirtyBlocks();
    highLighter->initHighlightingRules();
    highLighter->blockSignals(false);
    bool RET;
    connect(askWatch,&ElaContentDialog::rightButtonClicked,[&]{
        askWatch->close();
        RET=true;
    });
    connect(askWatch,&ElaContentDialog::leftButtonClicked,[&](){
        askWatch->close();

    });
    askWatch->setCentralWidget(askWatchWidget);
    askWatch->exec();

    return RET;
}
bool BaseScrollPage::askDialog(QString title,QString content){
    return askDialog(this,title,content);
}
// bool BaseScrollPage::askDialog(QString title,QString content){
//     ElaContentDialog *askWatch=new ElaContentDialog(this,false);
//     QWidget * askWatchWidget=new QWidget(this);
//     ElaText *askText=new ElaText(title,this);

//     ElaPlainTextEdit *askLine =new ElaPlainTextEdit();
//     QVBoxLayout *askLayout=new QVBoxLayout();
//     askLayout->addSpacing(5);
//     askLayout->addWidget(askText,40);
//     askLayout->addSpacing(10);
//     askLayout->addWidget(askLine,60);
//     askWatchWidget->setLayout(askLayout);
//     askLine->setMinimumHeight(45);
//     askLine->setReadOnly(true);
//     askLine->setPlainText(content);

//     askText->setTextPointSize(20);
//     askText->setTextPixelSize(20);

//     askWatch->setRightButtonText(tr("是"));
//     askWatch->setMiddleButtonText("");

//     askWatch->setLeftButtonText(tr("否"));

//     MarkdownHighlighter *highLighter=new MarkdownHighlighter(askLine->document(),MarkdownHighlighter::None);
//     // highLighter->setDocument(textBrowser->document());
//     highLighter->clearDirtyBlocks();
//     highLighter->initHighlightingRules();
//     highLighter->blockSignals(false);

//     askLine->update();
//     bool RET;
//     connect(askWatch,&ElaContentDialog::rightButtonClicked,[&]{
//         askWatch->close();
//         RET=true;
//     });
//     connect(askWatch,&ElaContentDialog::leftButtonClicked,[&](){
//         askWatch->close();

//     });
//     askWatch->setCentralWidget(askWatchWidget);
//     askWatch->exec();
//     askText->deleteLater();
//     highLighter->deleteLater();
//     askLine->deleteLater();
//     askWatch->deleteLater();
//     askWatchWidget->deleteLater();
//     return RET;
// }
ElaScrollPageArea * BaseScrollPage::GenerateArea(QWidget *parent,ElaText * title, QHBoxLayout * widget, bool convert){
    ElaScrollPageArea * Area=new ElaScrollPageArea(parent);
    QHBoxLayout * HLayout=new QHBoxLayout(Area);
    if(convert){
        title->setMinimumHeight(45);
        title->setTextPixelSize(15);
        for (int i = 0; i < widget->count(); ++i) {
            QLayoutItem* item = widget->itemAt(i);
            if (QWidget* wget = item->widget()) {
                wget->setMinimumHeight(45);
            }
        }
    }
    HLayout->addWidget(title);
    HLayout->addLayout(widget);
    return Area;
}

ElaScrollPageArea *BaseScrollPage::GenerateArea(QWidget *parent, QWidget *title, QHBoxLayout *widget, bool convert)
{
    ElaScrollPageArea * Area=new ElaScrollPageArea(parent);
    QHBoxLayout * HLayout=new QHBoxLayout(Area);
    if(convert){
        for (int i = 0; i < widget->count(); ++i) {
            QLayoutItem* item = widget->itemAt(i);
            if (QWidget* wget = item->widget()) {
                wget->setMinimumHeight(45);
            }
        }
    }
    HLayout->addWidget(title);
    HLayout->addLayout(widget);
    return Area;
}

ElaScrollPageArea *BaseScrollPage::GenerateArea(QWidget *parent, QHBoxLayout *title, QHBoxLayout *widget, bool convert)
{
    ElaScrollPageArea * Area=new ElaScrollPageArea(parent);
    QHBoxLayout * HLayout=new QHBoxLayout(Area);
    if(convert){
        for (int i = 0; i < widget->count(); ++i) {
            QLayoutItem* item = widget->itemAt(i);
            if (QWidget* wget = item->widget()) {
                wget->setMinimumHeight(45);
            }
        }
    }
    HLayout->addLayout(title);
    HLayout->addLayout(widget);
    return Area;
}


