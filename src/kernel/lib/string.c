#include "mod.h"

Size StringLength(const char* str) {
    Size len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

Boolean StringEqual(const char* str1, const char* str2) {
    Size len1 = StringLength(str1);
    Size len2 = StringLength(str2);
    if (len1 != len2) {
        return FALSE;
    }
    
    for (Size i = 0; i < len1; i++) {
        if (str1[i] != str2[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

Boolean StringStartWith(const String str1, const String start) {
    Size len1 = StringLength(str1);
    Size len2 = StringLength(start);
    if (len1 < len2) {
        return FALSE;
    }
    
    for (Size i = 0; i < len2; i++) {
        if (str1[i] != start[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

u32 StringToInt32(const char* str) {
    u32 result = 0;
    Size len = StringLength(str);
    for (Size i = 0; i < len; i++) {
        if (!IsDigit(str[i])) {
            Panic("StringToInt32: string {%s} is not a number.", str);
        }
        result = result * 10 + str[i] - '0';
    }
    return result;
}

u32 StringASCIITotal(const char* str) {
    u32 result = 0;
    Size len = StringLength(str);
    for (Size i = 0; i < len; i++) {
        result += str[i];
    }
    return result;
}

u32 CharCount(const String str, char ch) {
    u32 result = 0;
    Size len = StringLength(str);
    for (Size i = 0; i < len; i++) {
        if (str[i] == ch) {
            result++;
        }
    }
    return result;
}

Boolean IsDigit(char c) {
    return c >= '0' && c <= '9';
}

Boolean IsAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

Boolean IsUpper(char c) {
    return c >= 'A' && c <= 'Z';
}

Boolean IsLower(char c) {
    return c >= 'a' && c <= 'z';
}