


<div style="position: relative; padding: 40% 45%;">
<iframe style="position: absolute; width: 100%; height: 100%; left: 0; top: 0;" src="https://docs.python.org/zh-cn/3/tutorial/index.html" frameborder="1" scrolling="yes" width="320" height="240"</iframe>
</div>


# 接入我们

### 🎉 欢迎加入我们的平台！请仔细阅读以下指南，确保您的文件压缩包及格式内容符合我们的要求。

### 文件目录要求

📂 请在您的压缩包中包含一个文件夹，该文件夹的名称应与您的项目名称相匹配。

### 必须包含的文件

1. `config.Asuls` - 此文件应位于项目名称文件夹内，包含项目的基本信息。
2. `***/***.Asul` - 此文件目录包含在Config.Asuls内，具体要求如下
以下是 `config.Asuls` 文件的样例内容：

```Asuls
[INI] MyCFG 1.0.0 AsulTop 我的CFG项目
[EXE] install.exe
[DIR] MyCFG
Asul/keyPreference
Asul/optPreference
```

🔍 **以下是对 `config.Asuls` 中各字段的详细解释：**

- `[INI]`**(必须)**: 表明这是CFG的基本信息 一共有4个参数
    - `MyCFG`: 这将会是展示CFG的名字
    - `1.0.0`: 这将会是展示CFG的版本
    - `AsulTop`: 这将会是展示CFG的作者名
    - `我的CFG项目`: 这将会作为详细展示在打开的CFG配置中，注意不要包含空格，此处不应该有空格
- `[EXE]`**(可选)**:如果需要在部署完毕后运行一个文件，那么可以使用这个标签
- `[DIR]`**(必须)**:指定在CFG目录下的文件名样式
- `Asul/...`: 这是您项目中所有需要配置的文件
    - **Tips:** Asul/是相对路径你可以任意配置不过需保证相应目录下存在这个文件
    - `Asul/keyPreference`:keyPreference.asul所在目录，且配置后keyPreference.cfg将保存到此处 
请确保您的 `config.Asuls` 文件包含了所有必要的信息，并且路径正确无误。这样我们才能确保顺利地接入和测试您的项目。👍

🔍 **以下是对 `***.Asul` 中的特殊字的使用：**
- `func` : 提供选择选项
    - func "标题" "详情" "默认配置(下方需再次提供该键)" 个数
    - "func_1" "功能1"
    - "func_2" "功能2"
- `key` : 提供按键绑定操作
    - key "默认键" "所需绑定的按键" "标题" "详情"
- `text` : 提供粘贴文本的功能
    - text "信息"
- `line` : "提供数字输入，替换值等功能" 
    - line "标题" "详细" "原文本(包含%1的部分将被替换)" "默认值"
如果有任何疑问或需要进一步的帮助，欢迎随时联系我们！🌟


**Markdown 文件由 KimiAI[@Moonshot](https://kimi.ai/) 提供生成帮助**

**Markdown 文件由 ChatGPT-4o[@OpenAi](https://Openai.com/) 提供生成帮助**

**Markdown 文件由 Claude3.5-Sonnet[@Claude](https://www.anthropic.com/claude/sonnet) 提供生成帮助**

**Markdown 文件由 GitHub Copilot[@Github](https://github.com/) 提供生成帮助**
