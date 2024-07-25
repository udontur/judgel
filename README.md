<br />
<div align="center">
  <h1 align="center">Judgel</h3>
  <p align="center">
    🧑‍⚖️ Simple local C++ judge
    <br />
    <br />
    <a href="https://github.com/udontur/judgel/issues/new">Report Bug</a>
    ·
    <a href="https://github.com/udontur">Made with passion by Hadrian</a>
  </p>
</div>

## About

Do you have competitive programming problems but can't host them online or want to work on them offline? Judgel has you covered!

🧑‍⚖️ Judgel is a simple and easy-to-use C++ judge that can be run locally and offline. 

⚙️ It has a variety of verdicts, an accurate judging process, and a simple UI!

🪛 You only need 3 simple step to get started!

🌟 If you find this helpful, please consider [starring this repository](https://docs.github.com/en/get-started/exploring-projects-on-github/saving-repositories-with-stars)! It will support me as a student.

## Prerequisites
1. Windows 11
2. GCC C++ compiler ([No yapping installation guide](https://hadrianlau.com/blog/no-yap-mingw-w64-guide))
## Installation 
1. [Click to download](https://raw.githubusercontent.com/udontur/judgel/main/app/installer/judgel.msi)
2. Open the installer, click ```More info``` and ```Run anyway```
3. Click ```Yes``` or ```Next``` or ```Install``` until you see the ```Finish``` button

## Usage (method 1)
File and folder names with a space may break Judgel
### Prerequisites 
1. Testcase folder name: ```testcase```
2. Code file name: ```main.cpp```
3. Current directory contains the above item
    -  Command: ```cd <YOUR_FOLDER_PATH>```

### Syntax
```judgel <TIME_LIMIT(s, int)>```

### Example
Settings
- Time limit: 2 seconds (must be an **integer** in **seconds**)
- Testcase path: ```C:\Users\udontur\code\WT00A\testcase```
- Code file path: ```C:\Users\udontur\code\WT00A\main.cpp```

Commands
1. Change directory: ```cd C:\Users\udontur\code\WT00A```
    - You don't need to do this everytime
2. Judge program:  ```judgel 2```
    - Repeat this command for future use

[Usage method 2](https://github.com/udontur/judgel/blob/main/asset/usage-method-2.md) (not recommended)

## License

Distributed under the [MIT License](https://github.com/udontur/judgel/blob/main/LICENSE). 
