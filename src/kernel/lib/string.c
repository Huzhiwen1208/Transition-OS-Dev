#include "lib.h"

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