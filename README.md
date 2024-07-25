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

Got some competitive programming problems, but you can't host them online (or you wanna do them offline)? Judgel got you covered!

Judgel is a simple and easy to use C++ judge that can be run locally and offline. It has all the verdicts and functions a typical online judge would have.

You only need 3 simple step to start using Judgel! 

🌟 If you find this helpful, consider [staring this repo](https://docs.github.com/en/get-started/exploring-projects-on-github/saving-repositories-with-stars)! It supports me as a student!

## Prerequisites
1. **Windows 11** (Windows 10 is not recommended)
2. **GCC C++ compiler** (g++, MinGW-w64)
    - Check it out: [No yapping installation guide](https://hadrianlau.com/blog/no-yap-mingw-w64-guide)
## Installation 
1. **[Click here](https://raw.githubusercontent.com/udontur/judgel/main/app/judgel.msi)** to download
2. **Open the installer** and press ```Run anyway```
    - Judgel is not a virus (I know this is what a virus would say)
    - You can check the [sourc code here](https://github.com/udontur/judgel/src)
3. **Click ```Next``` or ```Install```** until you see the ```Finish``` button
    - Extra: You can check the installation by typing ```judgel``` in a new terminal
## Usage
```judgel``` works in **a terminal** (CMD or PowerShell) 
Type ```judgel``` for about and help page. 
### Current directory mode (recommended)
This method requries significantly less typing
#### Prerequisites 
1. Your testcase folder **must** be named ```testcase```
2. Your code file **must** be named ```main.cpp```
3. You **must** be on the **folder** that contain the above items
    -  Command: ```cd <YOUR_FOLDER_PATH>```

This is a must for the current direectory mode to work properly
#### Syntax
```judgel <TIME_LIMIT(s, int)>```

Example (Time limit must be an **integer** in **seconds**)
-  Time limit: 2 seconds
- Testcase path: ```C:\Users\udontur\code\WT00A\testcase```
- Code path: ```C:\Users\udontur\code\WT00A\main.cpp```
- The current directory is ```C:\Users\udontur\code\WT00A```

Command: ```judgel 3```. 
It would look like this: ```C:\Users\udontur\code\WT00A> judgel 3```
### Path mode (not recommended)
This method requires more typing, but there are **no prerequisties**
#### Syntax
```judgel <TIME_LIMIT(s, int)> <TESTCASE/PATH> <CODE/PATH>```

Example (Time limit must be an **integer** in **seconds**)
- Time limit: 2 seconds
- Testcase path: ```C:\Users\udontur\Documents\WT00A\testcase```
- Code path: ```C:\Users\udontur\code\i luv cs\a.cpp```

Command: ```judgel 2 "C:\Users\udontur\Documents\WT00A\testcase" "C:\Users\udontur\code\i luv cs\a.cpp"``` 

Note 1: Relative paths is also accepted
Note 2: Paths that contain spaces must be wrapped with ```"``` 

### Report a bug
Please [report the bug](https://github.com/udontur/judgel/issues/new) if you found a bug in the program.

Please list:
1. How to produce the bug
2. A screenshot of the bug

Reporting the bug improves this program. [udontur](https://github.com/udontur) more than happy to discover an unknown bug through you help! 

## License

Distributed under the [MIT License](https://github.com/udontur/ossd-mark-calculator/blob/main/LICENSE). See [LICENSE](https://github.com/udontur/ossd-mark-calculator/blob/main/LICENSE) for more information.
