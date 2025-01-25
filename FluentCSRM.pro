QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

RC_FILE += Uac/res.rc

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Sources/BaseScrollpage.cpp \
    Sources/F_Collector.cpp \
    Sources/T_About.cpp \
    Sources/T_Deploy.cpp \
    Sources/T_FormatHelp.cpp \
    Sources/T_Home.cpp \
    Sources/T_Setting.cpp \
    Sources/W_DragFrame.cpp \
    main.cpp \
    AsulCFGManager.cpp

HEADERS += \
    Headers/BaseInclude.h \
    Headers/BaseScrollpage.h \
    AsulCFGManager.h \
    Headers/F_Collector.h \
    Headers/T_About.h \
    Headers/T_Deploy.h \
    Headers/T_FormatHelp.h \
    Headers/T_Home.h \
    Headers/T_Setting.h \
    Headers/W_DragFrame.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# win32: LIBS += -L$$PWD/ElaWidgetTools/build/Desktop_Qt_6_7_1_MinGW_64_bit-Release/ -lelawidgettools

# INCLUDEPATH += $$PWD/ElaWidgetTools/src/include
# DEPENDPATH += $$PWD/ElaWidgetTools/src/include

# win32:!win32-g++: PRE_TARGETDEPS += $$PWD/ElaWidgetTools/build/Desktop_Qt_6_7_1_MinGW_64_bit-Release/elawidgettools.lib
# else:win32-g++: PRE_TARGETDEPS += $$PWD/ElaWidgetTools/build/Desktop_Qt_6_7_1_MinGW_64_bit-Release/libelawidgettools.a

DISTFILES +=

RESOURCES += \
    Resource.qrc

win32: LIBS += -L$$PWD/install/lib/ -lElaWidgetTools

INCLUDEPATH += $$PWD/install/include
DEPENDPATH += $$PWD/install/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/install/lib/ElaWidgetTools.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/install/lib/libElaWidgetTools.a
