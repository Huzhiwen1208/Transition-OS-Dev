#include "mod.h"

static Cursor newCursor(u32 row, u32 col);
/// @brief get screen current address
static PhysicalAddress getScreenAddress();
/// @brief set screen current address
static void setScreenAddress(PhysicalAddress address);
/// @brief get cursor position
static Cursor getCursor();
/// @brief get cursor physical address
static PhysicalAddress getCursorAddress();
/// @brief set cursor position
static void setCursor(Cursor cursor);
/// @brief clear screen
static void clearScreen();
/// @brief 如果当前行被写过，则换行
static void ConsoleAlignLine();

Size ConsoleWriteWithColor(const char* buf, Size len, ConsoleColor color) {
    if (color != WHITE && color != LIGHT_BLUE) {
        ConsoleAlignLine();
    }
    // 1. find cursor position in byte
    u16* currentPosition = (u16*)getCursorAddress();
    u32 row = getCursor().Row;
    u32 col = getCursor().Col;
    while (len--) {
        char ch = *buf++;
        // switch (ch) {
        //     case BS: // \b
        //         if (col > 0) {
        //             col--;
        //         }
        //         break;
        //     case HT: // \t
        //         col += 4;
        //         if (col >= ScreenWidth) {
        //             col = ScreenWidth - 1;
        //         }
        //         break;
        //     case LF: // \n
        //         row++;
        //         col=0;
        //         break;
        //     case CR: // \r
        //         col = 0;
        //         break;
        //     default:
        //         *currentPosition = (u16)(color << 8 | ch);
        //         col++;
        // }
        if (ch == BS) { // \b
            if (col > 0) {
                col--;
            }
        } else if (ch == HT) { // \t
            col += 4;
            if (col >= ScreenWidth) {
                col = ScreenWidth - 1;
            }
        } else if (ch == LF) { // \n
            row++;
            col=0;
        } else if (ch == CR) { // \r
            col = 0;
        } else {
            *currentPosition = (u16)(color << 8 | ch);
            col++;
        }

        if (col >= ScreenWidth) {
            col = 0;
            row++;
        }

        if (row >= ScreenHigh) {
            // move screen up
            PhysicalAddress screenAddress = getScreenAddress();
            if (screenAddress + ScreenSize >= MonitorBaseAddress + MonitorSize) {
                MemoryCopy((void*)MonitorBaseAddress, (void*)screenAddress, ScreenSize);
                MemoryFree((void*)(MonitorBaseAddress + ScreenSize), MonitorSize - ScreenSize);
                screenAddress = MonitorBaseAddress;
                screenAddress += ScreenWidthByte;
            } else {
                screenAddress += ScreenWidthByte;
            }

            setScreenAddress(screenAddress);
            row--;
        }

        currentPosition = (u16*)(getScreenAddress() + row * ScreenWidthByte + col * 2);
    }

    setCursor(newCursor(row, col));
    return len;
}

void InitializeConsole() {
    setScreenAddress(MonitorBaseAddress);
    setCursor(newCursor(0, 0));
    clearScreen();
}

static PhysicalAddress getScreenAddress() {
    WriteByte(MonitorAddressRegister, ScreenAddressHigh);
    u32 screenAddressOffset = ReadByte(MonitorDataRegister) << 8; // get high location
    WriteByte(MonitorAddressRegister, ScreenAddressLow);
    screenAddressOffset |= ReadByte(MonitorDataRegister); // get low location and merge

    screenAddressOffset <<= 1; // 2 bytes per character
    return screenAddressOffset + MonitorBaseAddress;
}

static void setScreenAddress(PhysicalAddress address) {
    PhysicalAddress screenAddressOffset = address - MonitorBaseAddress;
    screenAddressOffset >>= 1; // 2 bytes per character
    WriteByte(MonitorAddressRegister, ScreenAddressHigh);
    WriteByte(MonitorDataRegister, (u8)(screenAddressOffset >> 8)); // set high location
    u32 test = ReadByte(MonitorDataRegister);
    WriteByte(MonitorAddressRegister, ScreenAddressLow);
    WriteByte(MonitorDataRegister, (u8)screenAddressOffset); // set low location
}

static Cursor getCursor() {
    WriteByte(MonitorAddressRegister, CursorAddressHigh);
    u32 cursorOffset = ReadByte(MonitorDataRegister) << 8; // get high location
    WriteByte(MonitorAddressRegister, CursorAddressLow);
    cursorOffset |= ReadByte(MonitorDataRegister); // get low location and merge

    cursorOffset <<= 1; // 2 bytes per character
    PhysicalAddress cursorAddress = cursorOffset + MonitorBaseAddress;
    u32 offset = cursorAddress - getScreenAddress();
    offset >>= 1; // 2 bytes per character

    return newCursor(offset / ScreenWidth, offset % ScreenWidth);
}

static PhysicalAddress getCursorAddress() {
    WriteByte(MonitorAddressRegister, CursorAddressHigh);
    u32 cursorOffset = ReadByte(MonitorDataRegister) << 8; // get high location
    WriteByte(MonitorAddressRegister, CursorAddressLow);
    cursorOffset |= ReadByte(MonitorDataRegister); // get low location and merge

    cursorOffset <<= 1; // 2 bytes per character
    return cursorOffset + MonitorBaseAddress;
}

static void setCursor(Cursor cursor) {
    PhysicalAddress cursorAddress = getScreenAddress() + (cursor.Row * ScreenWidth + cursor.Col) * 2;
    PhysicalAddress cursorOffset = cursorAddress - MonitorBaseAddress;
    cursorOffset >>= 1; // 2 bytes per character
    WriteByte(MonitorAddressRegister, CursorAddressHigh);
    WriteByte(MonitorDataRegister, (u8)(cursorOffset >> 8)); // set high location
    WriteByte(MonitorAddressRegister, CursorAddressLow);
    WriteByte(MonitorDataRegister, (u8)cursorOffset); // set low location
}

static void clearScreen() {
    u16* ptr = (u16*)MonitorBaseAddress;
    while (ptr < (u16*)(MonitorBaseAddress + MonitorSize)) {
        *ptr++ = 0x0720;
    }
}

static Cursor newCursor(u32 row, u32 col) {
    Cursor cursor = {row, col};
    return cursor;
}

static void ConsoleAlignLine() {
    Cursor cursor = getCursor();
    if (cursor.Col != 0) {
        ConsoleWriteWithColor("\n", 1, WHITE);
    }
}