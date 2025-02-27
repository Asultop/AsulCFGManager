#include "Headers/T_FormatHelp.h"

T_FormatHelp::T_FormatHelp(QWidget *parent)
    : BaseScrollPage{parent}
{
    initWidget("加入我们","格式要求","在这里获取我们需要的格式");
    QTextBrowser *textBrowser = new QTextBrowser(this);
    QFile *mdFile=new QFile(":/md/Md/formatHelp.md");
    mdFile->open(QIODevice::ReadOnly);
    QTextStream mdStream(mdFile);
    QString mdContent=mdStream.readAll();
    mdFile->close();

    QFile *cssFile=new QFile(":/md/Md/formatHelp.css");
    cssFile->open(QIODevice::ReadOnly);
    QTextStream cssStream(cssFile);
    QString cssContent=cssStream.readAll();
    cssFile->close();

    textBrowser->setMarkdown(mdContent);
    // textBrowser->setHtml(mdContent);
    textBrowser->setMinimumHeight(1200);
    textBrowser->setOpenExternalLinks(true);
    textBrowser->setStyleSheet(cssContent);
    centerVLayout->addWidget(textBrowser);
}
T_FormatHelp::~T_FormatHelp(){

}
