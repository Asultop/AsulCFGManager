#include "Headers/T_About.h"
#include <QHBoxLayout>
#include <QIcon>
#include <QVBoxLayout>
#include <QApplication>

#include "ElaImageCard.h"
#include "ElaText.h"
#include "ElaTheme.h"

T_About::T_About (QWidget* parent)
    : ElaWidget (parent)
{
    // 清空窗口标题，避免标题栏显示
    this->setWindowTitle ("");

    // setWindowModality(Qt::ApplicationModal);
    setWindowButtonFlags(ElaAppBarType::CloseButtonHint);

    // 创建内容区域
    ElaImageCard* pixCard = new ElaImageCard (this);
    pixCard->setFixedSize (100, 100); // 增大图标尺寸
    pixCard->setIsPreserveAspectCrop (false);
    pixCard->setCardImage (QImage (QString (":/pic/Pic/favicon.png").replace ("favicon.png",
                                                                          eTheme->getThemeMode () == ElaThemeType::Light ? "favicon_dark.png" : "favicon.png")));

    // 主题变化时更新图标
    connect (eTheme, &ElaTheme::themeModeChanged, [=]() {
        QString pngLo = QString(":/pic/Pic/favicon.png").replace("favicon.png",
                                                                 eTheme->getThemeMode() == ElaThemeType::Light ? "favicon_dark.png" : "favicon.png");
        pixCard->setCardImage(QImage(pngLo));
    });

    // 版本信息
    ElaText* versionText = new ElaText ("AsulCFGManager [V1.0.0]", this);
    QFont versionTextFont = versionText->font ();
    versionTextFont.setWeight (QFont::Bold);
    versionText->setFont (versionTextFont);
    versionText->setWordWrap (false);
    versionText->setTextPixelSize (22); // 增大字体尺寸

    // 授权信息
    ElaText* licenseText = new ElaText (tr ("MIT 授权协议"), this);
    licenseText->setWordWrap (false);
    licenseText->setTextPixelSize (16);

    // 构建日期
    QString buildDate = QString (tr("构建日期:")+" %1 - %2").arg (__DATE__).arg(__TIME__);
    ElaText* supportText = new ElaText(buildDate, this);
    supportText->setWordWrap(false);
    supportText->setTextPixelSize(16);

    // 作者信息
    ElaText* authorText = new ElaText (tr ("作者: 3191522462@qq.com"), this);
    authorText->setWordWrap (false);
    authorText->setTextInteractionFlags (Qt::TextSelectableByMouse);
    authorText->setTextPixelSize (16);

    // 交流群信息 - 新建独立 ElaText
    ElaText* groupText = new ElaText (tr ("交流群: 1025515317 (QQ)"), this);
    groupText->setWordWrap (false);
    groupText->setTextInteractionFlags (Qt::TextSelectableByMouse);
    groupText->setTextPixelSize (16);

    // 技术支持
    ElaText* helperText = new ElaText (tr ("提供额外的专业技术支持"), this);
    helperText->setWordWrap (false);
    helperText->setTextPixelSize (16);

    // 支付二维码
    ElaText *wxpay = new ElaText (this);
    wxpay->setWordWrap (false);
    wxpay->setPixmap (QPixmap (":/pic/Pic/wxpay-removebg-preview.png"));
    wxpay->setScaledContents (true);
    wxpay->setFixedSize (180, 180); // 增大二维码尺寸

    ElaText *alipay = new ElaText (this);
    alipay->setWordWrap (false);
    alipay->setPixmap (QPixmap (":/pic/Pic/alipay-removebg-preview.png"));
    alipay->setScaledContents (true);
    alipay->setFixedSize (180, 180); // 增大二维码尺寸

    // 版权信息
    ElaText* copyrightText = new ElaText (tr ("版权所有 © 2025 AsulTop"), this);
    copyrightText->setWordWrap (false);
    copyrightText->setTextPixelSize (16);

    // 支付布局
    QHBoxLayout* payLayout = new QHBoxLayout ();
    payLayout->addWidget (wxpay);
    payLayout->addSpacing (30); // 增加二维码间距
    payLayout->addWidget (alipay);
    payLayout->setAlignment (Qt::AlignCenter);

    // 文本内容布局 - 优化间距和对齐
    QVBoxLayout* textLayout = new QVBoxLayout ();
    textLayout->setSpacing (20); // 增大整体间距
    textLayout->addWidget (pixCard, 0, Qt::AlignCenter);
    textLayout->addWidget (versionText, 0, Qt::AlignCenter);
    textLayout->addWidget (licenseText, 0, Qt::AlignCenter);
    textLayout->addWidget (supportText, 0, Qt::AlignCenter);
    textLayout->addWidget (authorText, 0, Qt::AlignCenter);
    textLayout->addWidget (groupText, 0, Qt::AlignCenter); // 添加独立的交流群文本
    textLayout->addWidget (helperText, 0, Qt::AlignCenter);
    textLayout->addSpacing (20);
    textLayout->addLayout (payLayout);
    textLayout->addSpacing (20);
    textLayout->addWidget (copyrightText, 0, Qt::AlignCenter);
    textLayout->addStretch (1); // 添加伸缩项，使内容居上

    // 主内容布局
    QHBoxLayout* contentLayout = new QHBoxLayout ();
    contentLayout->addStretch (1); // 左侧间距
    contentLayout->addLayout (textLayout);
    contentLayout->addStretch (1); // 右侧间距

    // 主布局
    QVBoxLayout* mainLayout = new QVBoxLayout (this);
    mainLayout->setContentsMargins (50, 30, 50, 30); // 增大边距
    mainLayout->addLayout (contentLayout);
    mainLayout->addStretch (1); // 底部间距

    // 设置固定大小
    this->setIsFixedSize(true);
    this->setFixedSize (QSize (500, 750));
    // this->setMinimumSize (QSize (600, 1000));
}

T_About::~T_About()
{
}
