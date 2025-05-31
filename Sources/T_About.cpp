#include "Headers/T_About.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QVBoxLayout>

#include "ElaImageCard.h"
#include "ElaText.h"
T_About::T_About(QWidget* parent)
    : ElaWidget(parent)
{
    setWindowTitle("关于 & 赞助..");
    setWindowIcon(QIcon(":/pic/Pic/favicon.png"));
    this->setIsFixedSize(true);
    setWindowModality(Qt::ApplicationModal);
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint);
    ElaImageCard* pixCard = new ElaImageCard(this);
    pixCard->setFixedSize(60, 60);
    pixCard->setIsPreserveAspectCrop(false);
    pixCard->setCardImage(QImage(":/pic/Pic/favicon.png"));

    QVBoxLayout* pixCardLayout = new QVBoxLayout();
    pixCardLayout->addWidget(pixCard);
    pixCardLayout->addStretch();

    QString compressDate=__DATE__;
    QString compressTime=__TIME__;

    ElaText* versionText = new ElaText("Asul-CFG-Manager [V0.1.8]", this);
    QFont versionTextFont = versionText->font();
    versionTextFont.setWeight(QFont::Bold);
    versionText->setFont(versionTextFont);
    versionText->setWordWrap(false);
    versionText->setTextPixelSize(18);

    ElaText* licenseText = new ElaText("MIT授权协议", this);
    licenseText->setWordWrap(false);
    licenseText->setTextPixelSize(14);
    ElaText* supportText = new ElaText("构建日期: "+compressDate+" - "+compressTime, this);
    supportText->setWordWrap(false);
    supportText->setTextPixelSize(14);
    ElaText* contactText = new ElaText("作者: 3191522462@qq.com\n交流群: 1025515317 (QQ)", this);
    contactText->setWordWrap(false);
    contactText->setTextInteractionFlags(Qt::TextSelectableByMouse);
    contactText->setTextPixelSize(14);
    ElaText* helperText = new ElaText("提供额外的专业技术支持", this);
    helperText->setWordWrap(false);
    helperText->setTextPixelSize(14);
    ElaText *wxpay=new ElaText(this);
    wxpay->setWordWrap(false);
    wxpay->setPixmap(QPixmap(":/pic/Pic/wxpay-removebg-preview.png"));
    wxpay->setScaledContents(true);
    wxpay->setFixedSize(130,130);

    ElaText *alipay=new ElaText(this);
    alipay->setWordWrap(false);
    alipay->setPixmap(QPixmap(":/pic/Pic/alipay-removebg-preview.png"));
    alipay->setScaledContents(true);
    alipay->setFixedSize(130,130);

    QHBoxLayout* payLayout=new QHBoxLayout();
    payLayout->addWidget(wxpay);
    payLayout->addWidget(alipay);
    ElaText* copyrightText = new ElaText("版权所有 © 2025 AsulTop", this);
    copyrightText->setWordWrap(false);
    copyrightText->setTextPixelSize(14);

    QVBoxLayout* textLayout = new QVBoxLayout();
    textLayout->setSpacing(15);
    textLayout->addWidget(versionText);
    textLayout->addWidget(licenseText);
    textLayout->addWidget(supportText);
    textLayout->addWidget(contactText);
    textLayout->addWidget(helperText);
    textLayout->addLayout(payLayout);
    textLayout->addWidget(copyrightText);
    textLayout->addStretch();

    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->addSpacing(30);
    contentLayout->addLayout(pixCardLayout);
    contentLayout->addSpacing(30);
    contentLayout->addLayout(textLayout);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 25, 0, 0);
    mainLayout->addLayout(contentLayout);
}

T_About::~T_About()
{
}
