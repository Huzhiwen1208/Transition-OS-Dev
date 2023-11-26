#include "utils.h"

Size StringLength(const char* str) {
    Size len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}