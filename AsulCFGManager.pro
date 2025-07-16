QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
LIBS += -lpsapi
RC_FILE += Uac/res.rc

# DEFINES += QT_NO_DEBUG_OUTPUT
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += QS_LOG_LINE_NUMBERS # Enable Debug with src line

SOURCES += \
    Sources/AsulWindow.cpp \
    Sources/BanElaComboBox.cpp \
    Sources/BaseScrollpage.cpp \
    Sources/F_Collector.cpp \
    Sources/F_SteamAvatarFetcher.cpp \
    Sources/F_SteamUserQuery.cpp \
    Sources/F_Translator.cpp \
    Sources/GlobalSettings.cpp \
    Sources/T_About.cpp \
    Sources/T_BasePage.cpp \
    Sources/T_CFGSettings.cpp \
    Sources/T_Deploy.cpp \
    Sources/T_FormatHelp.cpp \
    Sources/T_Home.cpp \
    Sources/T_Home2.cpp \
    Sources/T_Manage.cpp \
    Sources/T_Setting.cpp \
    Sources/T_SplashScreen.cpp \
    Sources/T_UserSetting.cpp \
    Sources/W_DragFrame.cpp \
    Sources/W_SCFGTransformer.cpp \
    main.cpp \
    AsulCFGManager.cpp

HEADERS += \
    Headers/AsulWindow.h \
    Headers/BanElaComboBox.h \
    Headers/BaseInclude.h \
    Headers/BaseScrollpage.h \
    AsulCFGManager.h \
    Headers/F_Collector.h \
    Headers/F_SteamAvatarFetcher.h \
    Headers/F_SteamUserQuery.h \
    Headers/F_SystemInfo.h \
    Headers/F_Translator.h \
    Headers/GlobalSettings.h \
    Headers/T_About.h \
    Headers/T_BasePage.h \
    Headers/T_CFGSettings.h \
    Headers/T_Deploy.h \
    Headers/T_FormatHelp.h \
    Headers/T_Home.h \
    Headers/T_Home2.h \
    Headers/T_Manage.h \
    Headers/T_Setting.h \
    Headers/T_SplashScreen.h \
    Headers/T_UserSetting.h \
    Headers/W_DragFrame.h \
    Headers/W_SCFGTransformer.h \
    Headers/singleton.h \
    Headers/stdafx.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    Resource.qrc

win32 {
    # 源DLL路径（根据实际情况调整）
    DLL_SOURCE = $${PWD}/install/bin/ElaWidgetTools.dll
    DLL_SOURCE = $$replace(DLL_SOURCE, /, \\)
    # 目标目录（与exe同级）
    DLL_TARGET = $${OUT_PWD}/release/
    DLL_TARGET = $$replace(DLL_TARGET, /, \\)

    # 构建后执行复制命令
    system(copy /Y $$DLL_SOURCE $$DLL_TARGET)

    message("DLL_SOURCE: " $$DLL_SOURCE)
    message("DLL_TARGET: " $$DLL_TARGET)
}

win32: LIBS += -L$$PWD/install/lib/ -lElaWidgetTools

INCLUDEPATH += $$PWD/install/include
DEPENDPATH += $$PWD/install/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/install/lib/ElaWidgetTools.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/install/lib/libElaWidgetTools.a
# 复制ElaWidgetTools.dll到输出目录


TRANSLATIONS += \
    translations/AM_zh_CN.ts \
    translations/AM_en_US.ts

include (3rd/qmarkdowntextedit/qmarkdowntextedit.pri)
include (3rd/QsLog/QsLog.pri)
include (3rd/ValveFileVDF-1.1.1/ValveFileVDF.pri)



