cmake_minimum_required(VERSION 3.16)
project(FluentCSRM VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
if(WIN32)
    enable_language(RC)  # 启用RC语言
endif()

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Network)
find_package(Qt${QT_VERSION_MAJOR} OPTIONAL_COMPONENTS Widgets)

qt_standard_project_setup()
add_subdirectory(ElaWidgetTools)

# 添加资源文件
set(Resource_resource_files
    "Pic/CS2.ico"
    "Pic/CSRM.png"
    "Pic/favicon.ico"
)

qt_add_resources(FluentCSRM "Resource"
    PREFIX
        "/pic"
    FILES
        ${Resource_resource_files}
)

set(Resource1_resource_files
    "${CMAKE_CURRENT_SOURCE_DIR}/Uac/res.rc"  # 确保路径正确
    "${CMAKE_CURRENT_SOURCE_DIR}/Uac/uac.manifest"
)

qt_add_resources(FluentCSRM "Resource1"
    PREFIX
        "/uac"
    FILES
        ${Resource1_resource_files}
)

set(Resource2_resource_files
    "Tools/7z.dll"
    "Tools/7z.exe"
    "Tools/getPerfectPath.bat"
)

qt_add_resources(FluentCSRM "Resource2"
    PREFIX
        "/Tools"
    FILES
        ${Resource2_resource_files}
)

set(Resource3_resource_files
    "Md/formatHelp.css"
    "Md/formatHelp.md"
)

qt_add_resources(FluentCSRM "Resource3"
    PREFIX
        "/md"
    FILES
        ${Resource3_resource_files}
)

qt_add_executable(FluentCSRM WIN32 MACOSX_BUNDLE
    FluentHiper.cpp FluentHiper.h
    Headers/BaseInclude.h
    Headers/BaseScrollpage.h
    Headers/T_Deploy.h
    Headers/T_FormatHelp.h
    Headers/T_Home.h
    Sources/BaseScrollpage.cpp
    Sources/T_Deploy.cpp
    Sources/T_FormatHelp.cpp
    Sources/T_Home.cpp
    main.cpp
    ElaWidgetTools/src/include/ElaWidgetTools.qrc
)

target_include_directories(FluentCSRM PRIVATE
    ElaWidgetTools/src/include
)

target_link_libraries(FluentCSRM PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Network
)

if((QT_VERSION_MAJOR GREATER 4))
    target_link_libraries(FluentCSRM PRIVATE
        Qt::Widgets
    )
endif()

target_link_libraries(FluentCSRM PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ElaWidgetTools/build/Desktop_Qt_6_7_1_MinGW_64_bit-Release/
    # elawidgettools
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/elawidgettools.dll
)

install(TARGETS FluentCSRM
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET FluentCSRM
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
