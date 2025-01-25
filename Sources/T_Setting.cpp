#include "Headers/T_Setting.h"
#include "AsulCFGManager.h"
#define parentWindow dynamic_cast<Asul *>(this->parent())
T_Setting::T_Setting(QWidget *parent)
    : BaseScrollPage{parent}
{
    initWidget("加入我们","格式要求","在这里获取我们需要的格式");

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
    centerVLayout->addWidget(steamPathArea);
    centerVLayout->addSpacing(5);

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

    centerVLayout->addWidget(perfectPathArea);
    centerVLayout->addSpacing(5);
    ElaScrollPageArea * ToggleArea=new ElaScrollPageArea(parent);
    QHBoxLayout * ToggleHLayout=new QHBoxLayout(ToggleArea);
    ElaText * ToggleText=new ElaText(parent);
    ToggleText->setTextPixelSize(15);
    ToggleText->setText("自动运行(单击切换状态): ");
    ToggleText->setMinimumWidth(200);
    ToggleHLayout->addWidget(ToggleText);
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

    ToggleHLayout->addStretch();

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
    centerVLayout->addWidget(ToggleArea);
    centerVLayout->addSpacing(5);
    centerVLayout->addStretch();
    // centerVLayout->addWidget();
}
void T_Setting::updateText(){
    steamPathChangeButton->setText(steamPath);
    perfectPathChangeButton->setText(perfectPath);
}

T_Setting::~T_Setting(){

}
