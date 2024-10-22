QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

RC_FILE += res.rc
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BaseScrollpage.cpp \
    T_Deploy.cpp \
    T_FormatHelp.cpp \
    T_Home.cpp \
    main.cpp \
    FluentHiper.cpp

HEADERS += \
    BaseInclude.h \
    BaseScrollpage.h \
    FluentHiper.h \
    T_Deploy.h \
    T_FormatHelp.h \
    T_Home.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32: LIBS += -L$$PWD/../ElaWidgetTools-main/build/Desktop_Qt_6_7_1_MinGW_64_bit-Release/ -lelawidgettools

INCLUDEPATH += $$PWD/../ElaWidgetTools-main/src/include
DEPENDPATH += $$PWD/../ElaWidgetTools-main/src/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../ElaWidgetTools-main/build/Desktop_Qt_6_7_1_MinGW_64_bit-Release/elawidgettools.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../ElaWidgetTools-main/build/Desktop_Qt_6_7_1_MinGW_64_bit-Release/libelawidgettools.a

DISTFILES +=

RESOURCES += \
    Resource.qrc
