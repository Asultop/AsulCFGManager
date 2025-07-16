# Join Us

### 🎉 Welcome to our platform! Please carefully read the following guidelines to ensure your file compression package and format content meet our requirements.

### File Directory Requirements

📂 Please include a folder in your compressed package, and the name of this folder should match your project name.

### Required Files

1. `config.Asuls` - This file should be located inside the project name folder and contain basic information about the project.
2. `***/***.Asul` - This file directory is included in Config.Asuls, with specific requirements as follows:
Below is a sample content of the `config.Asuls` file:

```Asuls
[INI] MyCFG 1.0.0 AsulTop MyCFGProject
[DIR] MyCFG
[PNG] {Your_Folder}/favicon.png
[HLP] {Your_Folder}/readme.txt
[EXE] {Your_Folder}/install.exe
[UPD] layout1
{Your_Folder}/keyPreference
{Your_Folder}/optPreference
```

🔍 **Here is a detailed explanation of each field in `config.Asuls`:**

- `[INI]`**(Required)**: Indicates the basic information of the CFG. There are 4 parameters in total.
    - `MyCFG`: This will be the displayed name of the CFG.
    - `1.0.0`: This will be the displayed version of the CFG.
    - `AsulTop`: This will be the displayed author name of the CFG.
    - `MyCFGProject`: This will be displayed in detail in the opened CFG configuration. Note that spaces should not be included here.
- `[DIR]`**(Required)**: Specifies the file name style in the CFG directory.
- `[PNG]`**(Optional)**: If you need to display a custom image during deployment, you can use this tag.
- `[HLP]`**(Optional)**: If you need to run a help file after deployment, you can use this tag.
- `[EXE]`**(Optional)**: If you need to run an executable file after deployment, you can use this tag.
- `[UPD]`**(Optional)**: Specifies the CFG parsing file version. If the versions are the same, a secure replacement can be configured.

- `{Your_Folder}/...`: These are all the files that need to be configured in your project.
    - **Tips:** {Your_Folder}/ is a relative path. You can configure it arbitrarily, but make sure the corresponding directory contains this file.
    - `{Your_Folder}/keyPreference`: The directory where keyPreference.asul is located, and after configuration, keyPreference.cfg will be saved here.
    - You can add feature values infinitely by line breaks.
Please ensure your `config.Asuls` file contains all necessary information and the paths are correct. This way, we can ensure smooth integration and testing of your project. 👍

🔍 **Here is the usage of special words in `***.Asul`:**
- `func`: Provides selection options.
    - func "Title" "Details" "Default Configuration (the key needs to be provided again below)" Number
    - "func_1" "Function 1"
    - "func_2" "Function 2"
- `key`: Provides key binding operations.
    - key "Default Key" "Key to Bind" "Title" "Details"
- `text`: Provides text pasting functionality.
    - text "Information"
- `line`: "Provides number input, replacement value, etc."
    - line "Title" "Details" "Original Text (parts containing %1 will be replaced)" "Default Value"
If you have any questions or need further assistance, feel free to contact us at any time! 🌟

**Markdown file generation assistance provided by KimiAI[@Moonshot](https://kimi.ai/)**

**Markdown file generation assistance provided by ChatGPT-4o[@OpenAi](https://Openai.com/)**

**Markdown file generation assistance provided by Claude3.5-Sonnet[@Claude](https://www.anthropic.com/claude/sonnet)**

**Markdown file generation assistance provided by GitHub Copilot[@Github](https://github.com/)**