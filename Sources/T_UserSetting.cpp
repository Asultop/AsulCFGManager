#include "Headers/T_UserSetting.h"
#include "Headers/GlobalSettings.h"
#include <vdf_parser.hpp>

#define DBG(x) QLOG_DEBUG()<<"[Key] "<<#x<<" : "<<x;
#define _DBG(label,x) QLOG_DEBUG()<<"["<<label<<"] "<<#x<<" : "<<x;


T_UserSetting::T_UserSetting(QWidget *parent)
    : ElaWindow{parent}
{

    this->setUserInfoCardVisible(false);
    this->setNavigationBarDisplayMode(ElaNavigationType::Minimal);
    this->setIsNavigationBarEnable(false);
    this->setWindowButtonFlags(ElaAppBarType::CloseButtonHint | ElaAppBarType::MaximizeButtonHint | ElaAppBarType::MinimizeButtonHint);
    this->setWindowTitle("");
    this->setProperty("ElaBaseClassName","FakeElaWindow");



    // initWindow();
}

T_UserSetting::~T_UserSetting()
{

}



void T_UserSetting::initWindow(const SteamUserInfo *usr)
{
    if(!m_inited){
        BaseScrollPage *centerPage=new BaseScrollPage(this);
        centerPage->initWidget(" ",tr("管理 ")+usr->personaName+"-[Steam64ID]:"+usr->userId+tr("的 配置"),tr("解析完成"));
        QString FileLocation=gSettings->getSteamUserPath()+usr->userShortId+"/730/local/cfg/cs2_user_convars_0_slot0.vcfg";
        // QString FileLocation="C:/Program Files (x86)/Steam/userdata/468857839/730/local/cfg/cs2_user_convars_0_slot0.vcfg";
        DBG(gSettings->getSteamUserPath()+usr->userShortId+"/730/local/cfg/cs2_user_convars_0_slot0.vcfg");

        QFile file(FileLocation);

        QString content="";
        if(file.exists()){
            if(file.open(QIODevice::ReadWrite)){
                content=file.readAll();
                file.close();
            }else{ QLOG_WARN()<<tr("无法打开文件: ")<<FileLocation;}
        }else{ QLOG_WARN()<<tr("找不到文件: ")<<FileLocation;}


        QString sensitivity="";
        QString m_yaw="";
        QString m_pitch="";

        if(!content.isEmpty()){


            QByteArray contentByte=content.toUtf8();
            auto root = tyti::vdf::read((contentByte.toStdString()).cbegin(),(content.toStdString()).cend());

            for(const auto& child: root.childs){
                if(child.first == "convars"){
                    DBG("GetInConvars"+usr->personaName);
                    sensitivity=QString::fromStdString(child.second->attribs["sensitivity"]);
                    m_yaw=QString::fromStdString(child.second->attribs["m_yaw"]);
                    m_pitch=QString::fromStdString(child.second->attribs["m_pitch"]);
                }
            }
        }


        ElaLineEdit *sensitivityLine=new ElaLineEdit(this);
        sensitivityLine->setText(sensitivity);
        ElaLineEdit *m_yawLine=new ElaLineEdit(this);
        m_yawLine->setText(m_yaw);
        ElaLineEdit *m_pitchLine=new ElaLineEdit(this);
        m_pitchLine->setText(m_pitch);


        ElaPlainTextEdit *previewShow=new ElaPlainTextEdit(this);
        previewShow->setVisible(false);
        connect(gSettings,&GlobalSettings::pPreviewShowChanged,[=](){
            previewShow->setVisible(gSettings->getPreviewShow());
        });
        previewShow->setReadOnly(true);
        previewShow->setPlainText(content);
        centerPage->centerVLayout->addWidget(centerPage->GenerateArea(this,new ElaText("sensitivity",this),sensitivityLine));
        centerPage->centerVLayout->addWidget(centerPage->GenerateArea(this,new ElaText(tr("m_yaw"),this),m_yawLine));
        centerPage->centerVLayout->addWidget(centerPage->GenerateArea(this,new ElaText(tr("m_pitch"),this),m_pitchLine));
        centerPage->centerVLayout->addWidget(previewShow);
        centerPage->centerVLayout->addStretch();

        this->setCentralWidget(centerPage);
        m_inited=true;
    }
}
