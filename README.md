<div align="center">
**⚡ CodeEditor**
 
**A feature-rich C++ code editor built with Qt6 for Windows**
 
Write, compile, and run C/C++ code — all in one lightweight desktop app.
 
![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=flat-square&logo=cplusplus)
![Qt](https://img.shields.io/badge/Qt-6-41CD52?style=flat-square&logo=qt)
![Platform](https://img.shields.io/badge/platform-Windows-0078D6?style=flat-square&logo=windows)
![License](https://img.shields.io/badge/license-MIT-blue?style=flat-square)
![Status](https://img.shields.io/badge/status-active-brightgreen?style=flat-square)
 
![CodeEditor Screenshot](https://raw.githubusercontent.com/snehal-thombare08/CodeEditor/74b5614bfdaf9b356ab75e1572b162471ba31ced/Screenshot%202026-06-12%20084957.png)
 
</div>
---
 
## 📖 Table of Contents
 
- [Features](#-features)
- [Built With](#️-built-with)
- [Getting Started](#-getting-started)
- [Roadmap](#-roadmap)
- [Author](#-author)
- [License](#-license)
---
 
## ✨ Features
 
| Feature | Description |
|---|---|
| 🎨 Syntax Highlighting | Full C/C++ syntax highlighting for better readability |
| 🔢 Line Numbers | Line numbering for easy navigation and debugging |
| 📑 Multiple Tabs | Work on several files at once in a tabbed interface |
| 🌗 Dark / Light Theme | Switch themes based on your preference |
| 🔍 Find and Replace | Quickly locate and update code |
| ⚙️ Compile and Run | Build and execute C/C++ code directly from the editor |
| 💾 Auto Save | Never lose your work |
| 🔠 Font Size Control | Adjust editor font size on the fly |
 
---
 
## 🛠️ Built With
 
- **C++17**
- **Qt6** (Widgets, GUI)
- **MinGW**
---
 
## 🚀 Getting Started
 
### Prerequisites
- [Qt6](https://www.qt.io/download) installed (this project uses `6.11.1`)
- MinGW compiler toolchain
- CMake
### Build Instructions
 
```bash
cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH=C:/Qt/6.11.1/mingw_64
mingw32-make -j4
```
 
Then run the generated executable from the build directory.
 
---
 
## 📌 Roadmap
- [ ] Add support for more languages
- [ ] Integrated debugger
- [ ] Custom keybindings
- [ ] Plugin support
*(Feel free to edit this section based on your actual plans!)*
 
---
 
## 👩‍💻 Author
 
**Snehal Thombare**
[GitHub](https://github.com/snehal-thombare08)
 
---
 
## 📄 License
 
Add a license (e.g., MIT) if you'd like others to freely use/contribute to this project.
