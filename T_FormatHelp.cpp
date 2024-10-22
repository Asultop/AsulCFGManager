#include "T_FormatHelp.h"

T_FormatHelp::T_FormatHelp(QWidget *parent)
    : BaseScrollPage{parent}
{
    initWidget("加入我们","格式要求","在这里获取我们需要的格式");
    QTextBrowser *textBrowser = new QTextBrowser(this);
    QFile *mdFile=new QFile(":/md/formatHelp.md");
    mdFile->open(QIODevice::ReadOnly);
    QTextStream mdStream(mdFile);
    QString mdContent=mdStream.readAll();
    mdFile->close();
    textBrowser->setMarkdown(mdContent);
    centerVLayout->addWidget(textBrowser);
}
T_FormatHelp::~T_FormatHelp(){

}
