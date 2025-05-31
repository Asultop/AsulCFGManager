


<h1 align="center"><img src="./Pic/favicon.ico" width="64" align="center"> AsulCFGManager (AM)</h1>

<p align="center">
    <a href="https://asul.top/CSRM">
        <img alt="Static Badge" src="https://img.shields.io/badge/website-Alivn-blue?style=for-the-badge">
    </a>
    <a href="https://github.com/AsulTop/AsulCFGManager/pulse">
        <img alt="GitHub commit activity" src="https://img.shields.io/github/commit-activity/m/AsulTop/AsulCFGManager?style=for-the-badge">
    </a>
</p>
<p align="center">
    <a href="https://github.com/AsulTop/AsulCFGManager">
        <img alt="Static Badge" src="https://img.shields.io/badge/AsulCFGManager-Master?style=for-the-badge&logo=github&labelColor=green&color=green">
    </a>
    <!-- <a href="https://github.com/eLecCap1taL/AutoSettings">
        <img alt="Static Badge" src="https://img.shields.io/badge/Manager-Manager?style=for-the-badge&logo=github&labelColor=yellow&color=yellow">
    </a> -->
    <a href="https://github.com/AsulTop/Asultop.github.io">
        <img alt="Static Badge" src="https://img.shields.io/badge/Page-Page?style=for-the-badge&logo=github&labelColor=purple&color=purple">
    </a>
</p>

# AsulCFGManager - CS2 配置管理工具
--- 
## 项目简介
**AsulCFGManager** 是一个高效、用户友好的配置管理工具，专为《反恐精英 2》（CS2）设计。该工具支持导入、解析和管理游戏配置文件（CFG），旨在提升玩家的游戏体验和设置管理效率。

## 主要功能
- **安装包导入**：轻松导入预先准备的 CS2 配置安装包。
- **自动解析**：智能解析安装包内容，自动将配置文件复制到指定位置。
- **简便管理**：通过直观界面，快速切换和更新配置文件，适应不同的游戏需求。

## 环境要求
- **操作系统**：Windows, macOS, 或 Linux
- **Qt 6.3.x 或以上**：用于项目构建与界面展示。
- **C++ 编译器**：推荐使用 GCC 或 MSVC，确保支持 C++11 或以上标准。

## 安装与构建指南

### 克隆项目
```bash
git clone https://github.com/Asultop/AsulCFGManager.git
cd AsulCFGManager
cd ElaWidgetTool
```

### 构建项目
1. 编译 ElaWidgetTool
2. 使用 Qt Creator 打开 `CMakeList.txt`
3. 或通过 CMake 构建
4. 将编译获得的 `elawidgettools.dll` 放入父目录 `../dll`
6. 返回到父目录：
   ```bash
   cd ..
   ```
7. 使用 Qt Creator 打开 `FluentCSRM.pro` 文件，配置编译选项。
8. 或者通过命令行构建：
   ```bash
   qmake FluentCSRM.pro
   make
   ```

### 运行程序
在构建完成后，执行生成的可执行文件以启动程序。

## 使用说明
1. 启动 AsulCFGManager。
2. 点击“导入配置”，选择准备好的 CS2 安装包。
3. 工具将自动解析并将配置文件复制至游戏配置目录。
4. 利用界面功能快速切换和应用所需配置。

## 项目结构
- **Sources/**：包含所有源代码文件。
- **Headers/**：存放项目头文件。
- **Tools/**：附加工具和实用程序。
- **lib/**：依赖库和外部资源。
- **Resources/**：包含图标和其他静态资源。
- **FluentCSRM.pro**：项目的 Qt 工程文件，定义构建设置。
- ~~**CMakeLists.txt**：项目的 CMake工程文件，但不推荐使用，很多参数配置（比如.rc文件链接）缺失 绝对不是因为我一开是没用CMake~~

## 贡献指南
欢迎社区参与！如果您希望贡献代码或提出改进建议，请：
1. Fork 本项目。
2. 创建新分支进行更改。
3. 提交 Pull Request，描述您的更改和改进。

## 联系信息
如有问题或建议，请通过 [Issues](https://github.com/Asultop/AsulCFGManager/issues) 联系我们。

**本文档使用ChatGPT4o辅助编写**
