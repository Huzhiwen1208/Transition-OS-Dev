# Transition-OS-Dev
## 环境搭建
> 计算机类型: Deepin or Ubuntu20

```shell
sudo apt-get update
sudo apt-get install git nasm bximage gcc qemu-system-i386 qemu-utils

sudo apt-get install gdb # debug using
```
```shell
git clone git@github.com:Huzhiwen1208/Transition-OS-Dev.git
cd Transition-OS-Dev

make build # build project
make run # execute the project
make debug # should use vscode launch.json with gdb
```

## 开发说明
1. 我决定采用驼峰式的代码风格，在C语言中用驼峰式或许非常别扭，但是我还是决定用它。这有点像Java。
2. 在代码中尽量使用`const u32 MAX = 1024;`来代替宏定义`#define MAX 1024`，这样做是有缺点的，它会带来更大的运行时开销（多头文件使用）。但是可以规避`#define MAX 256 * 4`这种容易出错的定义。而且我更想做的和C语言不一样，这也是个大胆的尝试。并且把所有的常亮，枚举都抽取到`common`模块中。
3. 不使用`int, long long, unsigned int...`，而是使用`i32, i64, u32...`代替。这有点像Rust。
4. 在项目文件布局上，我采用了新的模式，每个模块`modelName`都有三个头文件`modelName.h, type.h, method.h`。其中我们只使用`modelName.h`作为导出（这有点像Rust），其内容框架是
    ```cpp
    // modelName.h
    #pragma once
    #include "type.h"
    #include "method.h"

    // type.h
    #pragma once
    #include "../common/common.h"
    /// @brief 使用到的struct类型，以及其他类型定义

    // method.h
    #pragma once
    #inlude "type.h"
    /// @brief 对外暴露的方法，功能
    ```
5. 整体的开发过程，会尽量解耦，并且采用需要什么添加什么的逻辑，所以前期可扩展性要好（虽然不是商业化工程开发）
6. 每个内容块都会单独开个分支，每个分支对应了一个README.md中的二级标题。其中所有的资源采用垂直记录。分支只能采用PR形式提交到master，并做好合并说明。

## 进入内核
- 对应分支：master
- 内容概述：实现了从裸机（CPU，内存，硬盘）进入内核。
在你的虚拟机（或者物理机，总之是你的运行代码的物理机不是远程开发机）上执行`make run`，你会看到类似下图的界面，那么恭喜你你的初始环境搭建好了。当然，如果出现难以解决的问题，请联系我的邮箱`jackhu.521.rose@gmail.com`。我会把它贴在FAQ中。如果我遇到什么问题，也会分享到FAQ中。

<div align=center><img src="images/initializing.png" width="50%" height="50%"></div>


## FAQ
1. 在操作系统中，数组下标越界是没有Warning和Error的，所以在开发中如果出现了下标越界，可能访问到其他我们并不想读/写的数据，所以需要严谨对待数组下标越界。
