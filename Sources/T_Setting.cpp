#include "Headers/T_Setting.h"
#include "ElaToggleSwitch.h"
#include "Headers/F_SystemInfo.h"
#include "Headers/BanElaComboBox.h"
#include "AsulCFGManager.h"
#include <QDesktopServices>
#include "ElaMessageBar.h"
#include "qtranslator.h"
#define parentWindow dynamic_cast<Asul *>(this->parent())
T_Setting::T_Setting(QWidget *parent,bool ReadHWInfo)
    : BaseScrollPage{parent}
{
    ElaLog::getInstance()->logMessage("==========");
    initWidget(tr("设置"),tr("设置"),tr("调整 ACM 的配置"));
    static QSettings setting("HKEY_CURRENT_USER\\Software\\Asul\\AM",QSettings::NativeFormat);
    ElaScrollPageArea *steamPathArea=new ElaScrollPageArea(this);
    QHBoxLayout *steamPathLayout=new QHBoxLayout(steamPathArea);
    steamPathText=new ElaText(tr("CFG路径: "),this);
    steamPathText->setMinimumHeight(MiddleHeight);
    steamPathText->setTextPixelSize(15);
    steamPathChangeButton=new ElaPushButton(steamPath,this);
    steamPathChangeButton->setMinimumHeight(MiddleHeight);
    steamPathChangeButton->setMinimumWidth(300);
    steamPathLayout->addWidget(steamPathArea);
    steamPathLayout->addWidget(steamPathText);
    steamPathLayout->addWidget(steamPathChangeButton);

    connect(steamPathChangeButton,&ElaPushButton::clicked,[=](){

        QString directory = QFileDialog::getExistingDirectory(parent, tr("选择文件夹"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(directory.isEmpty()) return;
        steamPath=directory;
        if(!steamPath.contains("csgo")||!steamPath.contains("cfg")){
            parentWindow->warnTips(tr("也许"),tr("这并非是一个有效的目录..."));
        }
        steamPathChangeButton->setText(steamPath);
        emit steamPathHasChanged();
    });


    ElaScrollPageArea *perfectPathArea=new ElaScrollPageArea(this);
    QHBoxLayout *perfectLayout=new QHBoxLayout(perfectPathArea);
    perfectPathText=new ElaText(tr("完美Resource路径:"),this);
    perfectPathText->setMinimumHeight(MiddleHeight);
    perfectPathText->setTextPixelSize(15);
    perfectPathChangeButton=new ElaPushButton(perfectPath,this);
    perfectPathChangeButton->setMinimumHeight(MiddleHeight);
    perfectPathChangeButton->setMinimumWidth(300);
    perfectLayout->addWidget(perfectPathText);
    perfectLayout->addWidget(perfectPathArea);
    perfectLayout->addWidget(perfectPathChangeButton);
    connect(perfectPathChangeButton,&ElaPushButton::clicked,[=](){

        QString directory = QFileDialog::getExistingDirectory(parent, tr("选择文件夹"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
        if(directory.isEmpty()) return;
        perfectPath=directory;
        if(!perfectPath.contains("perfectworldarena")){
            parentWindow->warnTips(tr("也许"),tr("这并非是一个有效的目录..."));
        }
        perfectPathChangeButton->setText(perfectPath);
        emit perfectPathHasChanged();
    });


    ElaScrollPageArea * ToggleArea=new ElaScrollPageArea(parent);
    QHBoxLayout * ToggleHLayout=new QHBoxLayout(ToggleArea);
    ElaText * ToggleText=new ElaText(parent);
    ElaText * ToggleTipText=new ElaText(parent);
    QVBoxLayout *downStyle=new QVBoxLayout();
    downStyle->addWidget(new ElaText(parent));
    // downStyle->addWidget(ToggleTipText);
    downStyle->addSpacing(10);
    ToggleText->setTextPixelSize(15);
    ToggleTipText->setTextPixelSize(10);
    ToggleText->setText(tr("自动运行: (单击切换)"));
    ToggleTipText->setText(tr("单击切换状态"));

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
    AutoDirButton->setText(tr("打开CFG目录"));

    ToggleHLayout->addWidget(AutoDirButton);

    AutoHelpButton->setText(tr("打开帮助文档"));
    ToggleHLayout->addWidget(AutoHelpButton);
    AutoEXEButton->setText(tr("运行自动程序"));
    ToggleHLayout->addWidget(AutoEXEButton);



    AutoEXEButton->setIsToggled(true);
    AutoDirButton->setIsToggled(true);
    AutoHelpButton->setIsToggled(true);

    gSettings->setAutoDir(true);
    gSettings->setAutoExe(true);
    gSettings->setAutoHelp(true);

    connect(AutoEXEButton,&ElaToggleButton::toggled,[=](bool toggled){
        autoExe=toggled;
        gSettings->setAutoExe(toggled);
        emit toggleAreaHasChanged();
    });
    connect(AutoDirButton,&ElaToggleButton::toggled,[=](bool toggled){
        autoDir=toggled;
        gSettings->setAutoDir(toggled);
        emit toggleAreaHasChanged();
    });
    connect(AutoHelpButton,&ElaToggleButton::toggled,[=](bool toggled){
        autoHelp=toggled;
        gSettings->setAutoHelp(toggled);
        emit toggleAreaHasChanged();
    });




    ElaText * openText=new ElaText(tr("打开目录"),parent);
    QHBoxLayout *openLayout=new QHBoxLayout();
    ElaPushButton *openCFGButton=new ElaPushButton(tr("CFG 目标地址"),parent);
    ElaPushButton *openPerfectButton=new ElaPushButton(tr("完美 Resource 地址"),parent);
    connect(openCFGButton,&ElaPushButton::clicked,[=](){
        if(steamPath.isEmpty())
            ElaMessageBar::error(ElaMessageBarType::Top,tr("设置"),tr("打开 CFG 文件夹失败"),getReadTime(tr("打开 CFG 文件夹失败")),parent);
        else QDesktopServices::openUrl(QUrl::fromLocalFile(steamPath));
    });
    connect(openPerfectButton,&ElaPushButton::clicked,[=](){
        if(perfectPath.isEmpty())
            ElaMessageBar::error(ElaMessageBarType::Top,tr("设置"),tr("打开 完美Resources 文件夹失败"),getReadTime(tr("打开 完美Resources 文件夹失败")),parent);

        else QDesktopServices::openUrl(QUrl::fromLocalFile(perfectPath));
    });

    openLayout->addWidget(openCFGButton);
    openLayout->addWidget(openPerfectButton);

    ElaScrollPageArea *openArea = GenerateArea(parent,openText,openLayout);
    ElaToggleButton *previewEnable=new ElaToggleButton(tr("显示"),parent);
    ElaScrollPageArea *previewArea=GenerateArea(parent,new ElaText(tr("配置时实时显示配置文件详细信息"),parent),previewEnable);
    connect(previewEnable,&ElaToggleButton::toggled,[=](bool toggle){
        previewShow=toggle;
        gSettings->setPreviewShow(toggle);
        emit changeAll();
    });

    sysInfo=new ElaText(
        QString("CPU: %1 %2\nGPU: %3 %4")
            .arg(CPUVendor)
            .arg(CPUName)
            .arg(GPUVendor)
            .arg(GPUName)
        ,this);
    sysInfo->setTextPixelSize(15);
    ElaPushButton *updateSysinfo=new ElaPushButton(tr("更新"),this);
    QHBoxLayout *sysInfoLayout=new QHBoxLayout();
    QWidget *sysInfoWidget=new QWidget(this);
    sysInfoLayout->addWidget(sysInfo);
    sysInfoLayout->addWidget(updateSysinfo);
    sysInfoWidget->setLayout(sysInfoLayout);
    updateSysinfo->setMaximumWidth(300);
    updateSysinfo->setMinimumWidth(300);
    this->addTopWidget(sysInfoWidget);

    connect(updateSysinfo,&ElaPushButton::clicked,[=](){
        if(!ReadHWInfo){
            ElaMessageBar::warning(ElaMessageBarType::Top,tr("配置"),tr("请重启以同意我们获取硬件信息"),getReadTime(tr("请重启以同意我们获取硬件信息")),parent);
            return;
        }
        updateSysinfo->setDisabled(true);
        if(updateSysinfo->text()!=tr("更新")){
            ElaMessageBar::warning(ElaMessageBarType::Top,tr("配置"),tr("正在获取中..."),getReadTime(tr("正在获取中...")),parent);
            return;
        }
        ElaMessageBar::information(ElaMessageBarType::Top,tr("配置"),tr("正在获取中..."),getReadTime(tr("正在获取中...")),parent);
        updateSysinfo->setText(tr("更新中.."));

        CPUVendor=SystemInfo::getCpuVendor();
        setting.setValue("CPUVendor",CPUVendor);
        CPUName=SystemInfo::getCpuName();
        setting.setValue("CPUName",CPUName);
        GPUVendor=SystemInfo::getGpuVendor();
        setting.setValue("GPUVendor",GPUVendor);
        GPUName=SystemInfo::getGpuName();
        setting.setValue("GPUName",GPUName);
        this->updateText();
        updateSysinfo->setText("更新");
        ElaMessageBar::success(ElaMessageBarType::Top,tr("配置"),tr("更新成功!"),getReadTime(tr("更新成功!")),parent);
        updateSysinfo->setDisabled(false);
    });

    BanElaComboBox *comboLang=new BanElaComboBox(this);
    comboLang->addItems(QStringList{"简体中文","English"});
    QString currentLang=setting.value("Language").toString();
    QString langIndexTag;
    if(currentLang=="zh_CN") langIndexTag="简体中文";
    else if(currentLang=="en_US") langIndexTag="English";
    comboLang->setCurrentText(langIndexTag);
    connect(comboLang,&ElaComboBox::currentTextChanged,[=](QString index){
        QString targetLang,loadFile;
        if(index == "简体中文") targetLang="zh_CN",loadFile=":/lang/translations/AM_zh_CN.qm";
        else if(index == "English") targetLang="en_US",loadFile=":/lang/translations/AM_en_US.qm";

        setting.setValue("Language",targetLang);
        // comboLang->setDisabled()
        // ElaMessageBarType::Information()
        QTranslator translator;
        translator.load(loadFile);
        qApp->installTranslator(&translator);
        // ElaMessageBar::information(ElaMessageBarType::Top,tr("语言"),tr("重启以更换语言"),0x3f3f3f);
        bool closeToRestart=askDialog(tr("语言"),tr("重启以更换语言"));
        if(closeToRestart){
            qApp->quit();
            QProcess::startDetached(qApp->applicationFilePath(), QStringList());
        }else{
            ElaMessageBar::warning(ElaMessageBarType::Top,tr("语言"),tr("重启以更换语言"),0x3f3f3f,parent);
        }

    });

    ElaScrollPageArea *languageArea=GenerateArea(this,new ElaText(tr("切换语言"),this),comboLang);

    ElaToggleSwitch *logMode=new ElaToggleSwitch(this);
    logMode->setIsToggled(setting.value("logMode").toBool());
    connect(logMode,&ElaToggleSwitch::toggled,[=](bool toggled){
        setting.setValue("logMode",toggled);
        if(!toggled) return;
        bool tog=askDialog(tr("日志"),tr("重启生效，开启日志可能会导致 **性能问题** "));
        if(!tog){
            logMode->setIsToggled(false);
            setting.setValue("logMode",false);
        }
    });
    ElaScrollPageArea *logArea=GenerateArea(this,new ElaText(tr("开启日志 (可能会有性能问题)"),this),logMode,false);
    centerVLayout->addWidget(steamPathArea);
    centerVLayout->addWidget(perfectPathArea);
    centerVLayout->addWidget(ToggleArea);
    centerVLayout->addWidget(previewArea);
    centerVLayout->addWidget(openArea);
    centerVLayout->addWidget(languageArea);
    centerVLayout->addWidget(logArea);
    centerVLayout->addStretch();

    // centerVLayout->addWidget();
}
void T_Setting::updateText(){
    steamPathChangeButton->setText(steamPath.isEmpty()?tr("选择"):steamPath);
    perfectPathChangeButton->setText(perfectPath.isEmpty()?tr("选择"):perfectPath);
    sysInfo->setText(QString("CPU: %1 %2\nGPU: %3 %4")
                         .arg(CPUVendor.isEmpty()?tr("未知")+" "+tr("厂商"):CPUVendor)
                         .arg(CPUName.isEmpty()?tr("未知")+" "+tr("型号"):CPUName)
                         .arg(GPUVendor.isEmpty()?tr("未知")+" "+tr("厂商"):GPUVendor)
                         .arg(GPUName.isEmpty()?tr("未知")+" "+tr("型号"):GPUName)
                     );
    // qDebug()<<"steamUpdateTo"<<steamPath;
    // qDebug()<<"perfectUpdateTo"<<perfectPath;
}

T_Setting::~T_Setting(){

}
