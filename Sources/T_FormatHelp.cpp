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
    textBrowser->setMinimumHeight(1200);
    textBrowser->setOpenExternalLinks(true);
    textBrowser->setStyleSheet(cssContent);

    // textBrowser->setStyleSheet(R"(
    //     QTextBrowser {
    //         background-color: #f6f8fa;
    //         color: #333;
    //         font-family: 'SFMono-Regular', Consolas, 'Liberation Mono', Menlo, Courier, monospace;
    //     }
    //     QTextBrowser h1 {
    //         font-size: 24pt;
    //         color: #333;
    //     }
    //     QTextBrowser h2 {
    //         font-size: 18pt;
    //         color: #333;
    //     }
    //     QTextBrowser pre {
    //         background-color: #f0f0f0;
    //         border: 1px solid #dcdcdc;
    //         padding: 10px;
    //     }
    //     QTextBrowser code {
    //         background-color: #f0f0f0;
    //         color: #d14;
    //     }
    // )");
    centerVLayout->addWidget(textBrowser);
}
T_FormatHelp::~T_FormatHelp(){

}
