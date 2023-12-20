#pragma once

#include "type.h"

void InitializeFileSystem();

// pwd命令
void PrintfWorkingDirectory();
// cd命令，切换当前目录到path下，如果path不存在或者不是目录，则不切换并抛出错误信息返回FALSE
Boolean ChangeDirectory(String path);
// linux中的ls初步实现，现在只支持-a，例如：ls -a /home/hzw 或者 ls -a ./abc/abdcd，暂不支持../abc/abdcd
void ListFiles(String option, String path);
// mkdir [-p] path，path可以为相对路径，也可以为绝对路径，但是我们暂时不支持..模式
Boolean MakeDirectory(String path, String option);
/// @brief 在inodeID指定的目录下创建文件，文件名为name，文件类型为type，如果id==-1, 那么就在当前目录下创建文件
/// @param id 目录的InodeID
/// @param name 文件名
/// @param type 文件类型
/// @return 创建成功返回创建文件的InodeID，否则返回-1
InodeID CreateFile(InodeID id, String name, FileType type);

// 清空文件内容
Boolean ClearFileContent(String path);
// 读取指定文件第n行的内容到buf中去，n从1开始，如果n超过文件行数，返回FALSE
Boolean ReadFileLine(String path, u32 n, String buf);
// write命令，将内容写入文件，要求文件必须存在
Boolean WriteFileContent(String path, String content, Boolean trunc);
// cat命令，打印文件内容 cat -n 5 path指打印文件前n行
Boolean PrintFileContent(String path, u32 n);