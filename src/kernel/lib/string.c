#include "lib.h"

Size StringLength(const char* str) {
    Size len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
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