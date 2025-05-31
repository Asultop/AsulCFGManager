#include "Headers/T_Home.h"
#include "Headers/BaseInclude.h"
#include <ElaFlowLayout.h>
#include <QDesktopServices>
T_Home::T_Home(QWidget *parent)
    : BaseScrollPage{parent}
{
    initWidget("主菜单","欢迎","这里是主菜单");

    ElaFlowLayout *flowLayout=new ElaFlowLayout(0,5,5);
    flowLayout->setContentsMargins(30, 0, 0, 0);
    flowLayout->setIsAnimation(true);
    flowLayout->setContentsMargins(QMargins(5,5,15,5));
    // ElaPopularCard* homeCard2 = new ElaPopularCard(this);
    // connect(homeCard2, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
    //     QDesktopServices::openUrl(QUrl("https://cap1tal.top/CSRM"));
    // });
    // homeCard2->setCardPixmap(QPixmap(":/pic/Pic/CSRM.png"));
    // homeCard2->setTitle("CSRM");
    // homeCard2->setSubTitle("5.0⭐ CSRM");
    // homeCard2->setInteractiveTips("免费获取");
    // homeCard2->setDetailedText("CSRM 一站式解决CFG加载问题");
    // homeCard2->setCardFloatPixmap(QPixmap("://pic/Pic/favicon.png"));
    // flowLayout->addWidget(homeCard2);

    ElaAcrylicUrlCard* urlCard1 = new ElaAcrylicUrlCard(this);
    urlCard1->setCardPixmapSize(QSize(62, 62));
    urlCard1->setFixedSize(195, 225);
    urlCard1->setTitlePixelSize(17);
    urlCard1->setTitleSpacing(25);
    urlCard1->setSubTitleSpacing(13);
    urlCard1->setUrl("https://github.com/AsulTop/AsulCFGManager");
    urlCard1->setCardPixmap(QPixmap(":/pic/Pic/github.png"));
    urlCard1->setTitle("Asul Github");
    urlCard1->setSubTitle("使用ElaWidgetTools\nUI框架开发的CS2 CFG管理工具");
    ElaToolTip* urlCard1ToolTip = new ElaToolTip(urlCard1);
    urlCard1ToolTip->setToolTip("https://github.com/AsulTop/AsulCFGManager");
    flowLayout->addWidget(urlCard1);

    ElaAcrylicUrlCard* urlCard2 = new ElaAcrylicUrlCard(this);
    urlCard2->setCardPixmapSize(QSize(62, 62));
    urlCard2->setFixedSize(195, 225);
    urlCard2->setTitlePixelSize(17);
    urlCard2->setTitleSpacing(25);
    urlCard2->setSubTitleSpacing(13);
    urlCard2->setUrl("https://github.com/Liniyous/ElaWidgetTools");
    urlCard2->setCardPixmap(QPixmap(":/pic/Pic/github.png"));
    urlCard2->setTitle("ElaTools Github");
    urlCard2->setSubTitle("Use ElaWidgetTools To Create A Cool Project");
    ElaToolTip* urlCard2ToolTip = new ElaToolTip(urlCard2);
    urlCard2ToolTip->setToolTip("https://github.com/Liniyous/ElaWidgetTools");
    flowLayout->addWidget(urlCard2);

    homeCard = new ElaPopularCard(this);
    homeCard->setCardPixmap(QPixmap(":/pic/Pic/favicon.png"));
    homeCard->setTitle("接入我们");
    homeCard->setSubTitle("使用我们的格式");
    homeCard->setInteractiveTips("JSON");
    homeCard->setDetailedText("达到CFG快速分发与部署");
    homeCard->setCardFloatPixmap(QPixmap("://pic/Pic/favicon.png"));
    flowLayout->addWidget(homeCard);
    // homeCard->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC/IARC_7+.svg.png"));


    ElaText* subWord=new ElaText("快捷 CFG 面板",this);
    QFont subFont=subWord->font();
    subFont.setPixelSize(20);
    subWord->setFont(subFont);

    centerVLayout->addSpacing(20);
    centerVLayout->addWidget(subWord);
    centerVLayout->addSpacing(20);
    centerVLayout->addLayout(flowLayout);
    centerVLayout->addStretch(1);

}
T_Home::~T_Home(){

}
