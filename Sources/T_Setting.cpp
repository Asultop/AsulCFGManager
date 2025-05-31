#include "Headers/T_Setting.h"
#include "AsulCFGManager.h"
#include <QDesktopServices>
#define parentWindow dynamic_cast<Asul *>(this->parent())
T_Setting::T_Setting(QWidget *parent)
    : BaseScrollPage{parent}
{
    initWidget("设置","设置","调整 ACMG 的配置");

    ElaScrollPageArea *steamPathArea=new ElaScrollPageArea(this);
    QHBoxLayout *steamPathLayout=new QHBoxLayout(steamPathArea);
    steamPathText=new ElaText("CFG路径: ",this);
    steamPathText->setMinimumHeight(MiddleHeight);
    steamPathText->setTextPixelSize(15);
    steamPathChangeButton=new ElaPushButton(steamPath,this);
    steamPathChangeButton->setMinimumHeight(MiddleHeight);
    steamPathChangeButton->setMinimumWidth(300);
    steamPathLayout->addWidget(steamPathArea);
    steamPathLayout->addWidget(steamPathText);
    steamPathLayout->addWidget(steamPathChangeButton);

    connect(steamPathChangeButton,&ElaPushButton::clicked,[=](){
        QString directory = QFileDialog::getExistingDirectory(parent, "选择文件夹", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(directory.isEmpty()) return;
        steamPath=directory;
        if(!steamPath.contains("csgo")||!steamPath.contains("cfg")){
            parentWindow->warnTips("也许","这并非是一个有效的目录...");
        }
        steamPathChangeButton->setText(steamPath);
        emit steamPathHasChanged();
    });


    ElaScrollPageArea *perfectPathArea=new ElaScrollPageArea(this);
    QHBoxLayout *perfectLayout=new QHBoxLayout(perfectPathArea);
    perfectPathText=new ElaText("完美Resource路径:",this);
    perfectPathText->setMinimumHeight(MiddleHeight);
    perfectPathText->setTextPixelSize(15);
    perfectPathChangeButton=new ElaPushButton(perfectPath,this);
    perfectPathChangeButton->setMinimumHeight(MiddleHeight);
    perfectPathChangeButton->setMinimumWidth(300);
    perfectLayout->addWidget(perfectPathText);
    perfectLayout->addWidget(perfectPathArea);
    perfectLayout->addWidget(perfectPathChangeButton);
    connect(perfectPathChangeButton,&ElaPushButton::clicked,[=](){

        QString directory = QFileDialog::getExistingDirectory(parent, "选择文件夹", "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(directory.isEmpty()) return;
        perfectPath=directory;
        if(!perfectPath.contains("perfectworldarena")){
            parentWindow->warnTips("也许","这并非是一个有效的目录...");
        }
        perfectPathChangeButton->setText(perfectPath);
        emit perfectPathHasChanged();
    });


    ElaScrollPageArea * ToggleArea=new ElaScrollPageArea(parent);
    QHBoxLayout * ToggleHLayout=new QHBoxLayout(ToggleArea);
    ElaText * ToggleText=new ElaText(parent);
    ElaText * ToggleTipText=new ElaText(parent);
    QVBoxLayout *downStyle=new QVBoxLayout(parent);
    downStyle->addWidget(new ElaText(parent));
    downStyle->addWidget(ToggleTipText);
    downStyle->addSpacing(10);
    ToggleText->setTextPixelSize(15);
    ToggleTipText->setTextPixelSize(10);
    ToggleText->setText("自动运行: ");
    ToggleTipText->setText("单击切换状态");

    // ToggleText->setMinimumWidth(200);
    ToggleHLayout->addWidget(ToggleText);
    ToggleHLayout->addLayout(downStyle);

    ToggleHLayout->addStretch();

    ElaToggleButton *AutoEXEButton=new ElaToggleButton(parent);
    AutoEXEButton->setMinimumHeight(MiddleHeight);
    AutoEXEButton->setMinimumWidth(200);
    ElaToggleButton *AutoHelpButton=new ElaToggleButton(parent);
    AutoHelpButton->setMinimumHeight(MiddleHeight);
    AutoHelpButton->setMinimumWidth(200);
    ElaToggleButton *AutoDirButton=new ElaToggleButton(parent);
    AutoDirButton->setMinimumHeight(MiddleHeight);
    AutoDirButton->setMinimumWidth(200);
    AutoDirButton->setText("打开CFG目录");

    ToggleHLayout->addWidget(AutoDirButton);

    AutoHelpButton->setText("打开帮助文档");
    ToggleHLayout->addWidget(AutoHelpButton);
    AutoEXEButton->setText("运行自动程序");
    ToggleHLayout->addWidget(AutoEXEButton);



    AutoEXEButton->setIsToggled(true);
    AutoDirButton->setIsToggled(true);
    AutoHelpButton->setIsToggled(true);

    connect(AutoEXEButton,&ElaToggleButton::toggled,[=](bool toggled){
        autoExe=toggled;
        emit toggleAreaHasChanged();
    });
    connect(AutoDirButton,&ElaToggleButton::toggled,[=](bool toggled){
        autoDir=toggled;
        emit toggleAreaHasChanged();
    });
    connect(AutoHelpButton,&ElaToggleButton::toggled,[=](bool toggled){
        autoHelp=toggled;
        emit toggleAreaHasChanged();
    });




    ElaText * openText=new ElaText("打开目录",parent);
    QHBoxLayout *openLayout=new QHBoxLayout(parent);
    ElaPushButton *openCFGButton=new ElaPushButton("CFG 目标地址",parent);
    ElaPushButton *openPerfectButton=new ElaPushButton("完美 Resource 地址",parent);
    connect(openCFGButton,&ElaPushButton::clicked,[=](){
        if(steamPath.isEmpty())
            parentWindow->errTips("地址","未找到 CFG 目录地址请手动输入");
        else QDesktopServices::openUrl(QUrl::fromLocalFile(steamPath));
    });
    connect(openPerfectButton,&ElaPushButton::clicked,[=](){
        if(perfectPath.isEmpty())
            parentWindow->errTips("地址","未找到 完美Resources 目录地址请手动输入");
        else QDesktopServices::openUrl(QUrl::fromLocalFile(perfectPath));
    });

    openLayout->addWidget(openCFGButton);
    openLayout->addWidget(openPerfectButton);

    ElaScrollPageArea *openArea = GenerateArea(parent,openText,openLayout);
    ElaToggleButton *previewEnable=new ElaToggleButton("显示",parent);
    ElaScrollPageArea *previewArea=GenerateArea(parent,new ElaText("配置时实时显示配置文件详细信息",parent),previewEnable);
    connect(previewEnable,&ElaToggleButton::toggled,[=](bool toggle){
        previewShow=toggle;
        emit changeAll();
    });

    centerVLayout->addWidget(steamPathArea);
    centerVLayout->addWidget(perfectPathArea);
    centerVLayout->addWidget(ToggleArea);
    centerVLayout->addWidget(previewArea);
    centerVLayout->addWidget(openArea);
    centerVLayout->addStretch();

    // centerVLayout->addWidget();
}
void T_Setting::updateText(){
    steamPathChangeButton->setText(steamPath);
    perfectPathChangeButton->setText(perfectPath);
    qDebug()<<"steamUpdateTo"<<steamPath;
    qDebug()<<"perfectUpdateTo"<<perfectPath;
}

T_Setting::~T_Setting(){

}
