#include "AsulCFGManager.h"

#include <QApplication>
#include <ElaApplication.h>
#include <QSplashScreen>
#include <QSettings>
#include <ElaWindow.h>
#include <Headers/T_SplashScreen.h>
#include <QTranslator>
#include <QPalette>
#include <QColor>
// 获取较暗的颜色版本（降低亮度）
QColor getDarkerColor(const QColor& color, int factor = 90) {
    // 创建颜色副本并调整亮度
    QColor result = color;
    result.setHslF(
        result.hslHueF(),
        result.hslSaturationF(),
        qMax(0.0, result.lightnessF() * (factor / 100.0)),
        result.alphaF()
        );
    return result;
}

// 获取较亮的颜色版本（提高亮度）
QColor getLighterColor(const QColor& color, int factor = 110) {
    // 创建颜色副本并调整亮度
    QColor result = color;
    result.setHslF(
        result.hslHueF(),
        result.hslSaturationF(),
        qMin(1.0, result.lightnessF() * (factor / 100.0)),
        result.alphaF()
        );
    return result;
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    eApp->init();
    qApp->setAutoSipEnabled(true);


    QColor bgColor = qApp->palette().color(QPalette::Highlight);

    qDebug()<<"bgColor=" <<bgColor;

    eTheme->setThemeColor(ElaThemeType::Dark, ElaThemeType::PrimaryNormal, bgColor);
    eTheme->setThemeColor(ElaThemeType::Dark, ElaThemeType::PrimaryHover, getLighterColor(bgColor));
    eTheme->setThemeColor(ElaThemeType::Dark, ElaThemeType::PrimaryPress, getLighterColor(bgColor,105));

    eTheme->setThemeColor(ElaThemeType::Light, ElaThemeType::PrimaryNormal, bgColor);
    eTheme->setThemeColor(ElaThemeType::Light, ElaThemeType::PrimaryHover, getDarkerColor(bgColor));
    eTheme->setThemeColor(ElaThemeType::Light, ElaThemeType::PrimaryPress, getDarkerColor(bgColor,95));

    QSettings setting("HKEY_CURRENT_USER\\Software\\Asul\\AM",QSettings::NativeFormat);
    bool dark=setting.value("DarkTheme").toBool();
    QString lang=setting.value("Language").toString();
    QStringList SupportLang={"zh_CN","en_US"};
    QTranslator translator;
    if(lang.isEmpty()&&!SupportLang.contains(lang)){

        QLocale::Language lab = QLocale::system().language();
        if(QLocale::Chinese == lab)
        {
            auto REF=translator.load(":/lang/translations/AM_zh_CN.qm");
            Q_UNUSED(REF);
            lang="zh_CN";

        }else if(QLocale::English == lab){
            auto REF=translator.load(":/lang/translations/AM_en_US.qm");
            Q_UNUSED(REF);
            lang="en_US";
        }

        setting.setValue("Language",lang);
    }else{
        if(lang=="zh_CN"){
            auto REF=translator.load(":/lang/translations/AM_zh_CN.qm");
            Q_UNUSED(REF);
        }else if(lang=="en_US"){
            auto REF=translator.load(":/lang/translations/AM_en_US.qm");
            Q_UNUSED(REF);
        }
    }
    qApp->installTranslator(&translator);
    QPixmap pix;
    eTheme->setThemeMode(dark?ElaThemeType::Light:ElaThemeType::Dark);
    eTheme->setThemeMode(dark?ElaThemeType::Dark:ElaThemeType::Light);
    QTimer::singleShot(500,[=](){
        eTheme->setThemeMode(dark?ElaThemeType::Light:ElaThemeType::Dark);
        eTheme->setThemeMode(dark?ElaThemeType::Dark:ElaThemeType::Light);
    });


    if(eTheme->getThemeMode()==ElaThemeType::Light){
        pix.load(":/pic/Pic/splash_dark_hRes.png");
    }else{
        pix.load(":/pic/Pic/splash_light_hRes.png");
    }
    pix=pix.scaled(QSize(400,244),Qt::KeepAspectRatio,Qt::SmoothTransformation);


    T_SplashScreen *splashWindow=new T_SplashScreen();



    // splashWindow->setWindowFlag(Qt::WindowStaysOnTopHint);
    // QApplication::processEvents();
    // QTimer::singleShot(500,[=](){splashWindow->show();});
    splashWindow->show();

    a.processEvents();




    Asul w;
    // w.showMinimized();
    // w.setHidden(true);
    w.setVisible(false);
    Asul::connect(eTheme,&ElaTheme::themeModeChanged,[=](ElaThemeType::ThemeMode themeMode){
        QSettings darkSet("HKEY_CURRENT_USER\\Software\\Asul\\AM",QSettings::NativeFormat);
        darkSet.setValue("DarkTheme",themeMode==ElaThemeType::Light?false:true);
    });
    QTimer::singleShot(200,[&](){
        w.show();
    });



    // eTheme->setThemeColor();

    // Asul::connect(&w,&Asul::initFinished,[=](){
    //     splashWindow->close();
    //     splashWindow->deleteLater();
    // });
    // splash.finish(&w);
    QTimer::singleShot(200,[=](){
        splashWindow->close();
        splashWindow->deleteLater();
        // aboutpage->deleteLater();
    });
    return a.exec();
}
