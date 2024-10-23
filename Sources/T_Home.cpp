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

    ElaPopularCard* homeCard2 = new ElaPopularCard(this);
    connect(homeCard2, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        QDesktopServices::openUrl(QUrl("https://cap1tal.top/CSRM"));
    });
    homeCard2->setCardPixmap(QPixmap(":/pic/Pic/CSRM.png"));
    homeCard2->setTitle("CSRM");
    homeCard2->setSubTitle("5.0⭐ CSRM");
    homeCard2->setInteractiveTips("免费获取");
    homeCard2->setDetailedText("CSRM 一站式解决CFG加载问题");
    homeCard2->setCardFloatPixmap(QPixmap("://pic/Pic/favicon.ico"));
    flowLayout->addWidget(homeCard2);
    homeCard = new ElaPopularCard(this);

    homeCard->setCardPixmap(QPixmap(":/pic/Pic/CS2.ico"));
    homeCard->setTitle("接入我们");
    homeCard->setSubTitle("使用我们的格式");
    homeCard->setInteractiveTips("JSON");
    homeCard->setDetailedText("达到CFG快速分发与部署");
    homeCard->setCardFloatPixmap(QPixmap("://pic/Pic/favicon.ico"));
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
