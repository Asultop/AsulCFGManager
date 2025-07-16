#include "Headers/T_SplashScreen.h"
#include <windows.h>
#include <QThreadPool>
T_SplashScreen::T_SplashScreen(QWidget *parent)
    : ElaWindow{parent}
{

    BaseScrollPage* splashPage=new BaseScrollPage(this);
    splashPage->initWidget("",tr("请等待..."),"");

    // this->setFixedSize(QSize(400,244));
    QHBoxLayout * icoLayout=new QHBoxLayout();
    ElaText *ico=new ElaText(this);
    ico->setPixmap(QPixmap(QString(":/pic/Pic/favicon.png").replace("favicon.png",eTheme->getThemeMode()==ElaThemeType::Light?"favicon_dark.png":"favicon.png")));
    ElaText *icoText=new ElaText("AsulCFGManager",this);
    icoText->setTextStyle(ElaTextType::Title);
    icoLayout->addWidget(ico);
    icoLayout->addWidget(icoText);
    icoLayout->addSpacing(15);
    QHBoxLayout * launcherLayout=new QHBoxLayout();
    ElaText * launchingText=new ElaText(tr("正在启动 ..."));
    launchingText->setTextStyle(ElaTextType::Subtitle);
    ElaProgressBar *launchingBar=new ElaProgressBar(this);
    launchingBar->setRange(0, 0);
    launchingBar->setMinimumHeight(50);


    launcherLayout->addWidget(launchingText);
    launcherLayout->addWidget(launchingBar);

    // launchingBar->hide();
    splashPage->centerVLayout->addLayout(icoLayout);
    splashPage->centerVLayout->addLayout(launcherLayout);
    // centerVLayout->addWidget(launchingText);


    this->setWindowButtonFlags(ElaAppBarType::CloseButtonHint);
    this->setWindowButtonFlag(ElaAppBarType::CloseButtonHint,false);
    this->setUserInfoCardVisible(false);
    // this->resize(QSize(400,280));
    this->setIsStayTop(false);
    this->setIsFixedSize(true);
    this->setFixedSize(QSize(400,300));
    // this->makeIndependence();
    this->setIsNavigationBarEnable(false);

    this->setNavigationBarDisplayMode(ElaNavigationType::Minimal);
    this->setWindowButtonFlags(ElaAppBarType::CloseButtonHint | ElaAppBarType::MinimizeButtonHint);

    this->addPageNode("None",splashPage,ElaIconType::AddressCard);
}
T_SplashScreen::~T_SplashScreen(){

}
