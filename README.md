<div align="center">

# ⚡ CodeEditor

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
- [Usage](#-usage)
- [Roadmap](#-roadmap)
- [Contributing](#-contributing)
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

## 🖱️ Usage

1. Launch **CodeEditor.exe**
2. Open or create a `.c` / `.cpp` file
3. Write your code — syntax highlighting and line numbers work automatically
4. Press **Compile & Run** to build and execute your program
5. Use **Find & Replace** (`Ctrl+F`) to quickly edit code
6. Switch between **Dark/Light** theme from the settings menu

---

## 📌 Roadmap
- [ ] Add support for more languages
- [ ] Integrated debugger
- [ ] Custom keybindings
- [ ] Plugin support

*(Feel free to edit this section based on your actual plans!)*

---

## 🤝 Contributing

Contributions are welcome!

1. Fork the project
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## 👩‍💻 Author

**Snehal Thombare**
[![GitHub](https://img.shields.io/badge/GitHub-snehal--thombare08-181717?style=flat-square&logo=github)](https://github.com/binarymind-dev)

---

## 📄 License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

<div align="center">

⭐ If you find this project useful, consider giving it a star!

</div>
