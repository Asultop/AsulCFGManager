#include "Headers/T_FormatHelp.h"
#include "ElaTheme.h"
#include "markdownhighlighter.h"
#include <QSettings>
T_FormatHelp::T_FormatHelp(QWidget *parent)
    : BaseScrollPage{parent}
{
    initWidget("",tr("格式要求"),tr("在这里获取我们需要的格式"));
    ElaPlainTextEdit *textBrowser = new ElaPlainTextEdit(this);

    QSettings setting("HKEY_CURRENT_USER\\Software\\Asul\\AM",QSettings::NativeFormat);

    QString mdFileContent=QString(":/md/Md/formatHelp_%1.md").arg(setting.value("Language").toString());
    QFile *mdFile=new QFile(mdFileContent);

    mdFile->open(QIODevice::ReadOnly);
    QTextStream mdStream(mdFile);
    QString mdContent=mdStream.readAll();
    mdFile->close();

    // QFile *cssFile=new QFile(":/md/Md/formatHelp.css");
    // cssFile->open(QIODevice::ReadOnly);
    // QTextStream cssStream(cssFile);
    // QString cssContent=cssStream.readAll();
    // cssFile->close();
    textBrowser->setPlainText(mdContent);
    // textBrowser->setMarkdown(mdContent);
    // // textBrowser->setHtml(mdContent);
    textBrowser->setMinimumHeight(1200);
    // textBrowser->setOpenExternalLinks(true);
    // textBrowser->setStyleSheet("{background: #FCFCFC}");

    MarkdownHighlighter *highLighter=new MarkdownHighlighter(textBrowser->document(),MarkdownHighlighter::FullyHighlightedBlockQuote);
    // highLighter->setDocument(textBrowser->document());
    highLighter->clearDirtyBlocks();
    highLighter->initHighlightingRules();
    highLighter->blockSignals(false);

    textBrowser->setReadOnly(true);

    centerVLayout->addWidget(textBrowser);

}
T_FormatHelp::~T_FormatHelp(){

}
