#pragma once

#include "type.h"

u8 ReadByte(u16 port);
void WriteByte(u16 port, u8 data);
u16 ReadWord(u16 port);
void WriteWord(u16 port, u16 data);