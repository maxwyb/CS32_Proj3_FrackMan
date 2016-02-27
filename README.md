FrackMan: UCLA CS 32 Project 3
=====================

An OpenGL-based video game implemented on C++, similar to Dig Dug.


### Notes

- The OpenGL framework is run on Mac OS X Xcode environment in this version of the game.
- Specification PDF for the game is attached in the repository.
- Course website: http://web.cs.ucla.edu/classes/winter16/cs32/

### How to Run the Game

- Make sure you have XQuartz and freeGLUT installed on your Mac OS X. Note that XQuartz is not shipped with OS X 10.8 or above and has to be manually installed.
- Installation instructions: http://web.cs.ucla.edu/classes/winter16/cs32/Projects/3/spec.html
- When running the program in Xcode you may get a message relating to Assets directory. Select File / Project Settings... and for Derived Data Location, select Project-relative Location.
- Press Command + R to run the program again.

### A Simplified Chinese installation guide to my beloved parents

- 访问 http://www.xquartz.org, 下载 XQuartz-2.7.8.dmg 并双击安装。安装后点击屏幕左上方 Apple 图标注销后重新登录。
- 下载 https://distfiles.macports.org/MacPorts/MacPorts-2.3.4-10.10-Yosemite.pkg 并双击安装，此版本针对 OS X Yosemite.
- 打开"终端"，输入命令<pre><code>sudo port install freeglut</code></pre>，之后按照提示输入管理员账户密码，回车键确认。
- 点击本网页右上角的 "Download ZIP" 下载本程序，解压缩后在 Xcode 中打开项目文件。选择“文件 - 项目设置", 在弹出窗口选择 "Derived Data 位置" 为 “项目相对位置"，确定。
- 在 Xcode 主界面按下 Command + R, 应弹出 XQuartz 窗口并运行游戏。

---
<p>Yingbo (Max) Wang</p>
<p>University of California, Los Angeles</p>
