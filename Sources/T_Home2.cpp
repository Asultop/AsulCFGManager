#include "Headers/T_Home2.h"

#include <QDebug>
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>

#include "ElaAcrylicUrlCard.h"
#include "ElaFlowLayout.h"
#include "ElaImageCard.h"
#include "ElaMenu.h"
#include "ElaMessageBar.h"
#include "ElaNavigationRouter.h"
#include "ElaPopularCard.h"
#include "ElaScrollArea.h"
#include "ElaText.h"
#include "ElaToolTip.h"
#include "AsulCFGManager.h"
#define parentWindow dynamic_cast<Asul *>(this->parent())
T_Home2::T_Home2(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("主页");

    setTitleVisible(false);
    setContentsMargins(2, 2, 0, 0);
    // 标题卡片区域
    ElaText* desText = new ElaText("ACM", this);
    desText->setTextPixelSize(18);
    ElaText* titleText = new ElaText("Asul CFG Manager ", this);
    titleText->setTextPixelSize(35);
    ElaText * titleSubText=new ElaText("CS2 CFG管理工具",this);
    titleSubText->setTextPixelSize(25);
    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setContentsMargins(30, 60, 0, 0);
    titleLayout->setAlignment(Qt::AlignRight);
    titleLayout->addWidget(desText);
    titleLayout->addWidget(titleText);
    titleLayout->addWidget(titleSubText);

    ElaImageCard* backgroundCard = new ElaImageCard(this);
    backgroundCard->setBorderRadius(10);
    backgroundCard->setFixedHeight(400);
    backgroundCard->setMaximumAspectRatio(1.7);
    backgroundCard->setCardImage(QImage(":/pic/Pic/dream.png"));

    ElaAcrylicUrlCard* urlCard1 = new ElaAcrylicUrlCard(this);
    urlCard1->setCardPixmapSize(QSize(62, 62));
    urlCard1->setFixedSize(195, 225);
    urlCard1->setTitlePixelSize(17);
    urlCard1->setTitleSpacing(25);
    urlCard1->setSubTitleSpacing(13);
    urlCard1->setUrl("https://github.com/AsulTop");
    urlCard1->setCardPixmap(QPixmap(":/pic/Pic/github.png"));
    urlCard1->setTitle("Asul Github");
    urlCard1->setSubTitle("开发者 GitHub");
    ElaToolTip* urlCard1ToolTip = new ElaToolTip(urlCard1);
    urlCard1ToolTip->setToolTip("https://github.com/AsulTop/AsulCFGManager");
    ElaAcrylicUrlCard* urlCard2 = new ElaAcrylicUrlCard(this);
    urlCard2->setCardPixmapSize(QSize(62, 62));
    urlCard2->setFixedSize(195, 225);
    urlCard2->setTitlePixelSize(17);
    urlCard2->setTitleSpacing(25);
    urlCard2->setSubTitleSpacing(13);
    urlCard2->setUrl("https://github.com/AsulTop");
    urlCard2->setCardPixmap(QPixmap(":/Resource/Image/Moon.jpg"));
    urlCard2->setTitle("Asul");
    urlCard2->setSubTitle("3191522462@qq.com");
    urlCard2->hide();

    ElaScrollArea* cardScrollArea = new ElaScrollArea(this);
    cardScrollArea->setWidgetResizable(true);
    cardScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cardScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cardScrollArea->setIsGrabGesture(true, 0);
    cardScrollArea->setIsOverShoot(Qt::Horizontal, true);
    QWidget* cardScrollAreaWidget = new QWidget(this);
    cardScrollAreaWidget->setStyleSheet("background-color:transparent;");
    cardScrollArea->setWidget(cardScrollAreaWidget);
    QHBoxLayout* urlCardLayout = new QHBoxLayout();
    urlCardLayout->setSpacing(15);
    urlCardLayout->setContentsMargins(30, 0, 0, 6);
    urlCardLayout->addWidget(urlCard1);
    // urlCardLayout->addWidget(urlCard2);
    urlCardLayout->addStretch();
    QVBoxLayout* cardScrollAreaWidgetLayout = new QVBoxLayout(cardScrollAreaWidget);
    cardScrollAreaWidgetLayout->setContentsMargins(0, 0, 0, 0);
    cardScrollAreaWidgetLayout->addStretch();
    cardScrollAreaWidgetLayout->addLayout(urlCardLayout);

    QVBoxLayout* backgroundLayout = new QVBoxLayout(backgroundCard);
    backgroundLayout->setContentsMargins(0, 0, 0, 0);
    backgroundLayout->addLayout(titleLayout);
    backgroundLayout->addWidget(cardScrollArea);

    // 推荐卡片
    ElaText* flowText = new ElaText("功能", this);
    flowText->setTextPixelSize(20);
    QHBoxLayout* flowTextLayout = new QHBoxLayout();
    flowTextLayout->setContentsMargins(33, 0, 0, 0);
    flowTextLayout->addWidget(flowText);
    // ElaFlowLayout
    ElaPopularCard* homeCard = new ElaPopularCard(this);
    connect(homeCard, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        QDesktopServices::openUrl(QUrl("https://github.com/AsulTop/AsulCFGManager"));
    });
    homeCard->setCardPixmap(QPixmap(":/pic/Pic/favicon.png"));
    homeCard->setTitle("ACM");
    homeCard->setSubTitle("CFG管理器");
    homeCard->setInteractiveTips("使用中");
    homeCard->setDetailedText("一个图形化CS2 CFG管理模块");
    homeCard->setCardButtontext("GitHub");
    homeCard->setCardFloatPixmap(QPixmap(":/pic/Pic/CS2.ico"));

    ElaPopularCard* homeCard1 = new ElaPopularCard(this);
    connect(homeCard1, &ElaPopularCard::popularCardButtonClicked, this, [=]() { Q_EMIT elaScreenNavigation(); });
    homeCard1->setTitle("市场");
    homeCard1->setSubTitle("下载最新的CFG包");
    homeCard1->setCardPixmap(QPixmap(":/pic/Pic/favicon.png"));
    homeCard1->setInteractiveTips("下载");
    homeCard1->setDetailedText("下载 CFG 本体 （该功能计划中）");
    homeCard1->setCardButtontext("转到下载");
    connect(homeCard1,&ElaPopularCard::popularCardButtonClicked,[=](){
        parentWindow->warnTips("市场","开发中");
        qDebug()<<"C1";
    });
    // homeCard1->setCardFloatPixmap(QPixmap(""));

    homeCard2 = new ElaPopularCard(this);
    connect(homeCard2, &ElaPopularCard::popularCardButtonClicked, this, [=]() { Q_EMIT elaSceneNavigation(); });
    homeCard2->setTitle("接入我们");
    homeCard2->setSubTitle("提供格式");
    homeCard2->setCardPixmap(QPixmap(":/pic/Pic/favicon.png"));
    homeCard2->setInteractiveTips("功能");
    homeCard2->setDetailedText("跳转到 AM 接入详情界面");
    homeCard2->setCardButtontext("转到详情");
    // homeCard2->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC/IARC_7+.svg.png"));


    ElaFlowLayout* flowLayout = new ElaFlowLayout(0, 5, 5);
    flowLayout->setContentsMargins(30, 0, 0, 0);
    flowLayout->setIsAnimation(true);
    flowLayout->addWidget(homeCard);
    flowLayout->addWidget(homeCard1);
    flowLayout->addWidget(homeCard2);

    // 菜单
    // _homeMenu = new ElaMenu(this);
    // ElaMenu* checkMenu = _homeMenu->addMenu(ElaIconType::Cubes, "查看");
    // checkMenu->addAction("查看1");
    // checkMenu->addAction("查看2");
    // checkMenu->addAction("查看3");
    // checkMenu->addAction("查看4");

    // ElaMenu* checkMenu1 = _homeMenu->addMenu(ElaIconType::Cubes, "查看");
    // checkMenu1->addAction("查看1");
    // checkMenu1->addAction("查看2");
    // checkMenu1->addAction("查看3");
    // checkMenu1->addAction("查看4");

    // ElaMenu* checkMenu2 = checkMenu->addMenu(ElaIconType::Cubes, "查看");
    // checkMenu2->addAction("查看1");
    // checkMenu2->addAction("查看2");
    // checkMenu2->addAction("查看3");
    // checkMenu2->addAction("查看4");

    // // QKeySequence key = QKeySequence(Qt::CTRL | Qt::Key_S);

    // _homeMenu->addSeparator();
    // _homeMenu->addElaIconAction(ElaIconType::BoxCheck, "排序方式", QKeySequence::Save);
    // _homeMenu->addElaIconAction(ElaIconType::ArrowRotateRight, "刷新");
    // QAction* action = _homeMenu->addElaIconAction(ElaIconType::ArrowRotateLeft, "撤销");
    // connect(action, &QAction::triggered, this, [=]() {
    //     ElaNavigationRouter::getInstance()->navigationRouteBack();
    // });

    // _homeMenu->addElaIconAction(ElaIconType::Copy, "复制");
    // _homeMenu->addElaIconAction(ElaIconType::MagnifyingGlassPlus, "显示设置");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("Home");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setSpacing(0);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(backgroundCard);
    centerVLayout->addSpacing(20);
    centerVLayout->addLayout(flowTextLayout);
    centerVLayout->addSpacing(10);
    centerVLayout->addLayout(flowLayout);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget);

    // 初始化提示
    // ElaMessageBar::success(ElaMessageBarType::BottomRight, "Success", "初始化成功!", 2000);
    // qDebug() << "初始化成功";
}

T_Home2::~T_Home2()
{
}

void T_Home2::mouseReleaseEvent(QMouseEvent* event)
{
    switch (event->button())
    {
    case Qt::RightButton:
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        _homeMenu->popup(event->globalPosition().toPoint());
#else
        _homeMenu->popup(event->globalPos());
#endif
        break;
    }
    default:
    {
        break;
    }
    }
    ElaScrollPage::mouseReleaseEvent(event);
}
