#include "Headers/T_CFGSettings.h"
#include "ElaImageCard.h"
#include "Headers/GlobalSettings.h"
#include "Headers/F_SteamAvatarFetcher.h"
#include "Headers/T_UserSetting.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QVBoxLayout>
#include <ElaWindow.h>
#include <QDir>
#include <QStringList>

#define DBG(x) QLOG_DEBUG()<<"[Key] "<<#x<<" : "<<x;
#define _DBG(label,x) QLOG_DEBUG()<<"["<<label<<"] "<<#x<<" : "<<x;
// #include "ElaImageCard.h"
// #include "ElaText.h"


T_CFGSettings::T_CFGSettings(QWidget* parent)
    : BaseScrollPage(parent)
{
    this->initWidget(tr("用户"),tr("Steam 用户配置"),tr("在这里可以快速 查看&调整 登陆过本机的 Steam 用户的 CS2 按键配置"));

    DBG(gSettings->getSteamConfPath());
    QString userConf=gSettings->getSteamConfPath()+"/loginusers.vdf";
    QList<SteamUserInfo> allUsers = F_SteamUserQuery::parseUsersFile(userConf);

    ElaToggleButton *showUserInfo=new ElaToggleButton(tr("显示详细信息"),this);
    showUserInfo->setIsToggled(false);
    showUserInfo->setMinimumWidth(250);

    this->addTopWidget(showUserInfo);
    QString apiKey = gSettings->getSettings()->value("steamApiKey").toString();
    if(apiKey.isEmpty()) this->showErr(tr("错误"),tr("steamApi 为空"));
    foreach(SteamUserInfo usr, allUsers){
        ElaScrollPageArea * UsrArea=new ElaScrollPageArea(this);
        QHBoxLayout * HLayout=new QHBoxLayout(UsrArea);
        ElaText *imageCard = new ElaText(this);
        imageCard->setAutoFillBackground(true);

        imageCard->setFixedSize(50,50);

        QPixmap img(":/pic/Pic/steamDefault.jpg");
        QPixmap simg = img.scaled(imageCard->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        imageCard->setPixmap(simg);


        HLayout->addWidget(imageCard);

        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, [=](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray data = reply->readAll();
                QPixmap img=QPixmap::fromImage(QImage::fromData(data));
                QPixmap simg = img.scaled(imageCard->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
                imageCard->setPixmap(QPixmap(simg));
                reply->deleteLater();
                manager->deleteLater();
            }
        });

        if(!apiKey.isEmpty()){
            quint64 steamId = usr.userId.toLongLong();
            SteamAvatarFetcher *fetcher = new SteamAvatarFetcher(this);
            connect(fetcher,&SteamAvatarFetcher::avatarUrlsReady,[=](const QStringList &urls){
                connect(manager, &QNetworkAccessManager::finished, [=](QNetworkReply *reply) {
                    if (reply->error() != QNetworkReply::NoError) {
                        manager->get(QNetworkRequest(QUrl(urls[2])));
                    }
                });
                manager->get(QNetworkRequest(QUrl(urls[2])));
            });

            connect(fetcher,&SteamAvatarFetcher::fetchError,[=](const QString &error){
                QLOG_WARN() <<"SteamAvatar Fetcher: "<< error;
            });


            fetcher->setRetryInterval(2000);
            fetcher->setMaxRetries(5);
            fetcher->fetchAvatarUrls(apiKey, steamId);
        }
        // 用户姓名文本
        ElaText *nameLabel = new ElaText(this);
        QString nameLabelText=usr.personaName;
        nameLabelText+=usr.mostRecent?tr(" (最近登陆)"):"";
        nameLabel->setText(nameLabelText);
        DBG(usr.personaName);
        DBG(usr.mostRecent);
        // nameLabel->setElideMode(Qt::ElideRight);  // 文本过长时显示省略号
        HLayout->addWidget(nameLabel);
        // 管理按钮

        ElaText *userShortIdLabel = new ElaText(this);
        userShortIdLabel->setText("*********");
        connect(showUserInfo,&ElaToggleButton::toggled,[=](bool toggled){
            if(toggled) userShortIdLabel->setText(usr.userShortId);
            else userShortIdLabel->setText("*********");
        });

        QFont idFont = userShortIdLabel->font();
        idFont.setPointSize(12);  // 设置字号
        idFont.setItalic(true);   // 可选：添加斜体样式
        userShortIdLabel->setFont(idFont);
        HLayout->addWidget(userShortIdLabel);


        ElaPushButton *manageBtn = new ElaPushButton(this);


        T_UserSetting *usrPage=new T_UserSetting(this);
        manageBtn->hide();

        connect(manageBtn,&ElaPushButton::clicked,[=](){
            usrPage->initWindow(&usr);
            usrPage->show();
        });


        manageBtn->setText(tr("管理"));
        HLayout->addWidget(manageBtn);

        ElaPushButton *openFile=new ElaPushButton(this);
        openFile->setText("UserData");
        connect(openFile,&ElaPushButton::clicked,[=](){
            QString userDataDir=gSettings->getSteamUserPath()+usr.userShortId+"/730";
            QDesktopServices::openUrl(QUrl::fromLocalFile(userDataDir));
        });
        openFile->setMinimumWidth(80);
        HLayout->addWidget(openFile);

        HLayout->setStretch(0, 0);
        HLayout->setStretch(1, 1);
        HLayout->setStretch(2, 0);
        HLayout->setStretch(3, 0);
        HLayout->setStretch(4, 0);

        HLayout->setAlignment(imageCard, Qt::AlignLeft);
        HLayout->setAlignment(manageBtn, Qt::AlignRight);

        centerVLayout->addWidget(UsrArea);
    }


    centerVLayout->addStretch();

    emit initFinished();

}

T_CFGSettings::~T_CFGSettings()
{
}




