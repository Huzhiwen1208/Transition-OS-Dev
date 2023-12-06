#include "mod.h"

#define NULL_CHAR 0 // 不可见字符

#define CODE_PRINT_SCREEN_DOWN 0xB7

typedef enum {
    KEY_NONE,
    KEY_ESC,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_0,
    KEY_MINUS,
    KEY_EQUAL,
    KEY_BACKSPACE,
    KEY_TAB,
    KEY_Q,
    KEY_W,
    KEY_E,
    KEY_R,
    KEY_T,
    KEY_Y,
    KEY_U,
    KEY_I,
    KEY_O,
    KEY_P,
    KEY_BRACKET_L,
    KEY_BRACKET_R,
    KEY_ENTER,
    KEY_CTRL_L,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_SEMICOLON,
    KEY_QUOTE,
    KEY_BACKQUOTE,
    KEY_SHIFT_L,
    KEY_BACKSLASH,
    KEY_Z,
    KEY_X,
    KEY_C,
    KEY_V,
    KEY_B,
    KEY_N,
    KEY_M,
    KEY_COMMA,
    KEY_POINT,
    KEY_SLASH,
    KEY_SHIFT_R,
    KEY_STAR,
    KEY_ALT_L,
    KEY_SPACE,
    KEY_CAPSLOCK,
    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_NUMLOCK,
    KEY_SCRLOCK,
    KEY_PAD_7,
    KEY_PAD_8,
    KEY_PAD_9,
    KEY_PAD_MINUS,
    KEY_PAD_4,
    KEY_PAD_5,
    KEY_PAD_6,
    KEY_PAD_PLUS,
    KEY_PAD_1,
    KEY_PAD_2,
    KEY_PAD_3,
    KEY_PAD_0,
    KEY_PAD_POINT,
    KEY_54,
    KEY_55,
    KEY_56,
    KEY_F11,
    KEY_F12,
    KEY_59,
    KEY_WIN_L,
    KEY_WIN_R,
    KEY_CLIPBOARD,
    KEY_5D,
    KEY_5E,
} KEY;

static char keymap[][4] = {
    /* scancode | without shift | with shift | l,r */
    /* ---------------------------------- */
    /* 0x00 */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // NULL
    /* 0x01 */ {0x1b, 0x1b, FALSE, FALSE},           // ESC
    /* 0x02 */ {'1', '!', FALSE, FALSE},
    /* 0x03 */ {'2', '@', FALSE, FALSE},
    /* 0x04 */ {'3', '#', FALSE, FALSE},
    /* 0x05 */ {'4', '$', FALSE, FALSE},
    /* 0x06 */ {'5', '%', FALSE, FALSE},
    /* 0x07 */ {'6', '^', FALSE, FALSE},
    /* 0x08 */ {'7', '&', FALSE, FALSE},
    /* 0x09 */ {'8', '*', FALSE, FALSE},
    /* 0x0A */ {'9', '(', FALSE, FALSE},
    /* 0x0B */ {'0', ')', FALSE, FALSE},
    /* 0x0C */ {'-', '_', FALSE, FALSE},
    /* 0x0D */ {'=', '+', FALSE, FALSE},
    /* 0x0E */ {'\b', '\b', FALSE, FALSE}, // BACKSPACE
    /* 0x0F */ {'\t', '\t', FALSE, FALSE}, // TAB
    /* 0x10 */ {'q', 'Q', FALSE, FALSE},
    /* 0x11 */ {'w', 'W', FALSE, FALSE},
    /* 0x12 */ {'e', 'E', FALSE, FALSE},
    /* 0x13 */ {'r', 'R', FALSE, FALSE},
    /* 0x14 */ {'t', 'T', FALSE, FALSE},
    /* 0x15 */ {'y', 'Y', FALSE, FALSE},
    /* 0x16 */ {'u', 'U', FALSE, FALSE},
    /* 0x17 */ {'i', 'I', FALSE, FALSE},
    /* 0x18 */ {'o', 'O', FALSE, FALSE},
    /* 0x19 */ {'p', 'P', FALSE, FALSE},
    /* 0x1A */ {'[', '{', FALSE, FALSE},
    /* 0x1B */ {']', '}', FALSE, FALSE},
    /* 0x1C */ {'\n', '\n', FALSE, FALSE},           // ENTER
    /* 0x1D */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // CTRL_L
    /* 0x1E */ {'a', 'A', FALSE, FALSE},
    /* 0x1F */ {'s', 'S', FALSE, FALSE},
    /* 0x20 */ {'d', 'D', FALSE, FALSE},
    /* 0x21 */ {'f', 'F', FALSE, FALSE},
    /* 0x22 */ {'g', 'G', FALSE, FALSE},
    /* 0x23 */ {'h', 'H', FALSE, FALSE},
    /* 0x24 */ {'j', 'J', FALSE, FALSE},
    /* 0x25 */ {'k', 'K', FALSE, FALSE},
    /* 0x26 */ {'l', 'L', FALSE, FALSE},
    /* 0x27 */ {';', ':', FALSE, FALSE},
    /* 0x28 */ {'\'', '"', FALSE, FALSE},
    /* 0x29 */ {'`', '~', FALSE, FALSE},
    /* 0x2A */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // SHIFT_L
    /* 0x2B */ {'\\', '|', FALSE, FALSE},
    /* 0x2C */ {'z', 'Z', FALSE, FALSE},
    /* 0x2D */ {'x', 'X', FALSE, FALSE},
    /* 0x2E */ {'c', 'C', FALSE, FALSE},
    /* 0x2F */ {'v', 'V', FALSE, FALSE},
    /* 0x30 */ {'b', 'B', FALSE, FALSE},
    /* 0x31 */ {'n', 'N', FALSE, FALSE},
    /* 0x32 */ {'m', 'M', FALSE, FALSE},
    /* 0x33 */ {',', '<', FALSE, FALSE},
    /* 0x34 */ {'.', '>', FALSE, FALSE},
    /* 0x35 */ {'/', '?', FALSE, FALSE},
    /* 0x36 */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // SHIFT_R
    /* 0x37 */ {'*', '*', FALSE, FALSE},             // PAD *
    /* 0x38 */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // ALT_L
    /* 0x39 */ {' ', ' ', FALSE, FALSE},             // SPACE
    /* 0x3A */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // CAPSLOCK
    /* 0x3B */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // F1
    /* 0x3C */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // F2
    /* 0x3D */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // F3
    /* 0x3E */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // F4
    /* 0x3F */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // F5
    /* 0x40 */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // F6
    /* 0x41 */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // F7
    /* 0x42 */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // F8
    /* 0x43 */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // F9
    /* 0x44 */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // F10
    /* 0x45 */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // NUMLOCK
    /* 0x46 */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // SCRLOCK
    /* 0x47 */ {'7', NULL_CHAR, FALSE, FALSE},       // pad 7 - Home
    /* 0x48 */ {'8', NULL_CHAR, FALSE, FALSE},       // pad 8 - Up
    /* 0x49 */ {'9', NULL_CHAR, FALSE, FALSE},       // pad 9 - PageUp
    /* 0x4A */ {'-', '-', FALSE, FALSE},             // pad -
    /* 0x4B */ {'4', NULL_CHAR, FALSE, FALSE},       // pad 4 - Left
    /* 0x4C */ {'5', NULL_CHAR, FALSE, FALSE},       // pad 5
    /* 0x4D */ {'6', NULL_CHAR, FALSE, FALSE},       // pad 6 - Right
    /* 0x4E */ {'+', '+', FALSE, FALSE},             // pad +
    /* 0x4F */ {'1', NULL_CHAR, FALSE, FALSE},       // pad 1 - End
    /* 0x50 */ {'2', NULL_CHAR, FALSE, FALSE},       // pad 2 - Down
    /* 0x51 */ {'3', NULL_CHAR, FALSE, FALSE},       // pad 3 - PageDown
    /* 0x52 */ {'0', NULL_CHAR, FALSE, FALSE},       // pad 0 - Insert
    /* 0x53 */ {'.', 0x7F, FALSE, FALSE},            // pad . - Delete
    /* 0x54 */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, //
    /* 0x55 */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, //
    /* 0x56 */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, //
    /* 0x57 */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // F11
    /* 0x58 */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // F12
    /* 0x59 */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, //
    /* 0x5A */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, //
    /* 0x5B */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // Left Windows
    /* 0x5C */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // Right Windows
    /* 0x5D */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, // Clipboard
    /* 0x5E */ {NULL_CHAR, NULL_CHAR, FALSE, FALSE}, //
};

static Boolean capslockState;
static Boolean extcodeState; // 扩展码状态

#define CtrlState (keymap[KEY_CTRL_L][2] || keymap[KEY_CTRL_L][3])
#define shiftState (keymap[KEY_SHIFT_L][2] || keymap[KEY_SHIFT_R][2])

static char *globalBuffer;
static Size globalBufferLength;

Size ReadLine(char* buf, Size len) {
    if (len > 1024) {
        return 0;
    }

    u8 status = GetInterruptStatus();
    asm volatile("sti");

    globalBuffer = buf;
    // 输入回车结束读取
    while (globalBufferLength == 0 || globalBuffer[globalBufferLength - 1] != '\n');

    if (globalBufferLength > len) {
        globalBufferLength = len;
    }

    // 去掉回车，换成字符串结束符
    globalBuffer[globalBufferLength - 1] = '\0';
    len = globalBufferLength - 1;

    globalBuffer = NULL;
    globalBufferLength = 0;
    RestoreInterruptStatus(status);
    return len;
}

void KeyboardHandler(int vector)
{
    Assert(vector == 0x21);
    OuteralInterruptCompleted(vector);

    u16 scancode = ReadByte(KeyboardDataPort);
    u8 ext = 2;

    if (scancode == 0xe0) {
        extcodeState = TRUE;
        return;
    }

    // 是扩展码
    if (extcodeState) {
        // 改状态索引
        ext = 3;

        // 修改扫描码，添加 0xe0 前缀
        scancode |= 0xe000;

        // 扩展状态无效
        extcodeState = FALSE;
    }

    // 获得通码
    u16 makecode = (scancode & 0x7f);

    // 是否是断码(按键抬起)
    Boolean breakcode = ((scancode & 0x0080) != 0);
    if (breakcode) {
        // 如果是则设置状态
        keymap[makecode][ext] = FALSE;
        return;
    }

    // 下面是通码，按键按下
    keymap[makecode][ext] = TRUE;

    // 计算 shift 状态
    Boolean shift = FALSE;
    if (capslockState && ('a' <= keymap[makecode][0] && keymap[makecode][0] <= 'z'))
        shift = !shift;

    if (shiftState)
        shift = !shift;

    // 获得按键 ASCII 码
    char ch = NULL_CHAR;

    if (ext == 3 && (makecode != KEY_SLASH))
        ch = keymap[makecode][1];
    else
        ch = keymap[makecode][shift];

    if (ch == NULL_CHAR)
        return;

    if (CtrlState && IsLower(ch)) {
        Printf("^%c", ch);
        return;
    }

    Printf("%c", ch);
    // 写入缓冲区
    if (globalBuffer) {
        globalBuffer[globalBufferLength++] = ch;
    }
}

void InitializeKeyboard() {
    capslockState = FALSE;
    extcodeState = FALSE;
    globalBuffer = NULL;
    globalBufferLength = 0;

    SetInterruptHandler(0x21, KeyboardHandler);
    SetInterrupt(0x21);
}
