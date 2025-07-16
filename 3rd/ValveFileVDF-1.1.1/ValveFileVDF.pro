# This pro file will build ValveFileVDF as a header-only library
TEMPLATE = aux
TARGET = ValveFileVDF
VERSION = 1.0.0

# Project metadata
DEFINES += DESCRIPTION=\\\"parses valve\'s vdf file format\\\"
DEFINES += HOMEPAGE_URL=\\\"https://github.com/TinyTinni/ValveFileVDF\\\"

# Options (mapped to qmake features)
CONFIG += VALVEFILEVDF_ENABLE_TESTING=off
CONFIG += VALVEFILEVDF_ENABLE_FUZZING=off
CONFIG += VALVEFILEVDF_ENABLE_PYTHON=off

# Header-only library configuration
INCLUDEPATH += $$PWD/include
HEADERS += $$PWD/include/vdf_parser.hpp

# Install settings
includedir = /usr/include
target.path = /usr/lib
share.path = /usr/local/share/$$TARGET

headers.files = $$HEADERS
headers.path = $$includedir
share.files = *.txt

INSTALLS += headers
INSTALLS += target
INSTALLS += share

# Conditional build settings
win32 {
    # Windows-specific configurations
}

unix:!macx {
    contains(QT_ARCH, x86_64) {
        target.path = /usr/lib64
    } else {
        target.path = /usr/lib
    }
}

# Testing configuration (simplified)
VALVEFILEVDF_ENABLE_TESTING {
    SUBDIRS += ./tests
}

VALVEFILEVDF_ENABLE_FUZZING {
    # Fuzzing requires special compiler checks
    (CLANG|MSVC):!mac {
        SUBDIRS += fuzzing
    } else {
        message("Fuzzing not supported - requires Clang or MSVC")
        CONFIG -= VALVEFILEVDF_ENABLE_FUZZING
    }
}

VALVEFILEVDF_ENABLE_PYTHON {
    SUBDIRS += python
}