#pragma once

#include "type.h"

void InitializeFileSystem();

void PrintfWorkingDirectory();
Boolean ChangeDirectory(String path);
void ListFiles(String option, String path);
Boolean MakeDirectory(String path, String option);
InodeID CreateFile(InodeID id, String name, FileType type);