# ValveFileVDF.pri

# 包含目录
INCLUDEPATH += $$PWD/include

# 源文件
SOURCES += \
    $$PWD/include/vdf_parser.hpp

# 头文件
HEADERS += \
    $$PWD/include/vdf_parser.hpp

# 安装规则
unix {
    target.path = /usr/local/lib
    headers.path = /usr/local/include
    INSTALLS += target headers
}

# 测试和模糊测试
contains(DEFINES, VALVEFILEVDF_ENABLE_TESTING) {
    SUBDIRS += tests
}

contains(DEFINES, VALVEFILEVDF_ENABLE_FUZZING) {
    SUBDIRS += fuzzing
}

# Python 绑定
contains(DEFINES, VALVEFILEVDF_ENABLE_PYTHON) {
    SUBDIRS += python
}
