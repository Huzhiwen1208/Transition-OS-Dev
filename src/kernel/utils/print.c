#include "utils.h"
#include "../common/common.h"
#include "../console/console.h"

// varivable arguments support
typedef char *va_list;

#define stack_size(t) (sizeof(t) <= sizeof(char *) ? sizeof(char *) : sizeof(t))
#define va_start(ap, v) (ap = (va_list)&v + sizeof(char *))
#define va_arg(ap, t) (*(t *)((ap += stack_size(t)) - stack_size(t)))
#define va_end(ap) (ap = (va_list)0)

#define ZEROPAD 0x01
#define SIGN 0x02
#define PLUS 0x04
#define SPACE 0x08
#define LEFT 0x10
#define SPECIAL 0x20
#define SMALL 0x40
#define DOUBLE 0x80

static Size SkipAtoi(const char **s)
{
    Size i = 0;
    while (IsDigit(**s))
        i = i * 10 + *((*s)++) - '0';
    return i;
}

static char *Number(char *str, u32 *num, i32 base, i32 size, i32 precision, i32 flags)
{
    char pad, sign, tmp[36];
    const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    Size i;
    Size index;
    char *ptr = str;

    if (flags & SMALL)
        digits = "0123456789abcdefghijklmnopqrstuvwxyz";

    if (flags & LEFT)
        flags &= ~ZEROPAD;

    if (base < 2 || base > 36)
        return 0;

    pad = (flags & ZEROPAD) ? '0' : ' ';

    if (flags & DOUBLE && (*(double *)(num)) < 0)
    {
        sign = '-';
        *(double *)(num) = -(*(double *)(num));
    }
    else if (flags & SIGN && !(flags & DOUBLE) && ((i32)(*num)) < 0)
    {
        sign = '-';
        (*num) = -(i32)(*num);
    }
    else
        sign = (flags & PLUS) ? '+' : ((flags & SPACE) ? ' ' : 0);

    if (sign)
        size--;

    if (flags & SPECIAL)
    {
        if (base == 16)
            size -= 2;
        else if (base == 8)
            size--;
    }

    i = 0;

    if (flags & DOUBLE)
    {
        u32 ival = (u32)(*(double *)num);
        u32 fval = (u32)(((*(double *)num) - ival) * 1000000);
        do
        {
            index = (fval) % base;
            (fval) /= base;
            tmp[i++] = digits[index];
        } while (fval);
        tmp[i++] = '.';

        do
        {
            index = (ival) % base;
            (ival) /= base;
            tmp[i++] = digits[index];
        } while (ival);
    }
    else if ((*num) == 0)
    {
        tmp[i++] = '0';
    }
    else
    {
        while ((*num) != 0)
        {
            index = (*num) % base;
            (*num) /= base;
            tmp[i++] = digits[index];
        }
    }

    if (i > precision)
        precision = i;

    size -= precision;

    if (!(flags & (ZEROPAD + LEFT)))
        while (size-- > 0)
            *str++ = ' ';

    if (sign)
        *str++ = sign;

    if (flags & SPECIAL)
    {
        if (base == 8)
            *str++ = '0';
        else if (base == 16)
        {
            *str++ = '0';
            *str++ = digits[33];
        }
    }
    if (!(flags & LEFT))
        while (size-- > 0)
            *str++ = pad;

    while (i < precision--)
        *str++ = '0';

    while (i-- > 0)
        *str++ = tmp[i];

    while (size-- > 0)
        *str++ = ' ';
    return str;
}

Size Vsprintf(char *buf, const char *fmt, va_list args)
{
    i32 len;
    i32 i;

    char *str;
    char *s;
    i32 *ip;

    i32 flags;

    i32 field_width;
    i32 precision;
    i32 qualifier;
    u32 num;
    u8 *ptr;

    for (str = buf; *fmt; ++fmt)
    {
        if (*fmt != '%')
        {
            *str++ = *fmt;
            continue;
        }

        flags = 0;
    repeat:
        ++fmt;
        switch (*fmt)
        {
        case '-':
            flags |= LEFT;
            goto repeat;
        case '+':
            flags |= PLUS;
            goto repeat;
        case ' ':
            flags |= SPACE;
            goto repeat;
        case '#':
            flags |= SPECIAL;
            goto repeat;
        case '0':
            flags |= ZEROPAD;
            goto repeat;
        }

        field_width = -1;

        if (IsDigit(*fmt))
            field_width = SkipAtoi(&fmt);

        else if (*fmt == '*')
        {
            ++fmt;
            field_width = va_arg(args, i32);

            if (field_width < 0)
            {
                field_width = -field_width;
                flags |= LEFT;
            }
        }

        precision = -1;

        if (*fmt == '.')
        {
            ++fmt;
            if (IsDigit(*fmt))
                precision = SkipAtoi(&fmt);

            else if (*fmt == '*')
            {
                precision = va_arg(args, i32);
            }
            if (precision < 0)
                precision = 0;
        }

        qualifier = -1;
        if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L')
        {
            qualifier = *fmt;
            ++fmt;
        }

        switch (*fmt)
        {

        case 'c':
            if (!(flags & LEFT))
                while (--field_width > 0)
                    *str++ = ' ';
            *str++ = (unsigned char)va_arg(args, i32);
            while (--field_width > 0)
                *str++ = ' ';
            break;

        case 's':
            s = va_arg(args, char *);
            len = StringLength(s);
            if (precision < 0)
                precision = len;
            else if (len > precision)
                len = precision;

            if (!(flags & LEFT))
                while (len < field_width--)
                    *str++ = ' ';
            for (i = 0; i < len; ++i)
                *str++ = *s++;
            while (len < field_width--)
                *str++ = ' ';
            break;

        case 'o':
            num = va_arg(args, unsigned long);
            str = Number(str, &num, 8, field_width, precision, flags);
            break;

        case 'p':
            if (field_width == -1)
            {
                field_width = 8;
                flags |= ZEROPAD;
            }
            num = va_arg(args, unsigned long);
            str = Number(str, &num, 16, field_width, precision, flags);
            break;

        case 'x':
            flags |= SMALL;
        case 'X':
            num = va_arg(args, unsigned long);
            str = Number(str, &num, 16, field_width, precision, flags);
            break;

        case 'd':
        case 'i':
            flags |= SIGN;
        case 'u':
            num = va_arg(args, unsigned long);
            str = Number(str, &num, 10, field_width, precision, flags);
            break;

        case 'n':
            ip = va_arg(args, i32 *);
            *ip = (str - buf);
            break;
        case 'f':
            flags |= SIGN;
            flags |= DOUBLE;
            double dnum = va_arg(args, double);
            str = Number(str, (u32 *)&dnum, 10, field_width, precision, flags);
            break;
        case 'b': // binary
            num = va_arg(args, unsigned long);
            str = Number(str, &num, 2, field_width, precision, flags);
            break;
        case 'm': // mac address
            flags |= SMALL | ZEROPAD;
            ptr = va_arg(args, char *);
            for (Size t = 0; t < 6; t++, ptr++)
            {
                i32 num = *ptr;
                str = Number(str, &num, 16, 2, precision, flags);
                *str = ':';
                str++;
            }
            str--;
            break;
        case 'r': // ip address
            flags |= SMALL;
            ptr = va_arg(args, u8 *);
            for (Size t = 0; t < 4; t++, ptr++)
            {
                i32 num = *ptr;
                str = Number(str, &num, 10, field_width, precision, flags);
                *str = '.';
                str++;
            }
            str--;
            break;
        default:
            if (*fmt != '%')
                *str++ = '%';
            if (*fmt)
                *str++ = *fmt;
            else
                --fmt;
            break;
        }
    }
    *str = '\0';

    i = str - buf;
    Assert(i < 1024);
    return i;
}

Size Sprintf(char *buf, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    Size i = Vsprintf(buf, fmt, args);
    va_end(args);
    return i;
}
    
Size Printf(const char *fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    Size i = Vsprintf(buf, fmt, args);
    va_end(args);
    return ConsoleWrite(buf, i);
}

Size Println(const char *fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    Size i = Vsprintf(buf, fmt, args);
    va_end(args);
    ConsoleWrite(buf, i);
    ConsoleWrite("\n", 1);
    return i;
}

Size PrintWithColor(ConsoleColor color, const char *fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    Size i = Vsprintf(buf, fmt, args);
    va_end(args);
    return ConsoleWriteWithColor(buf, i, color);
}

void Panic(const char *fmt, ...)
{
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    i32 i = Vsprintf(buf, fmt, args);
    va_end(args);
    PrintWithColor(LIGHT_RED, "[PANIC] %s\n", buf);
    Suspend();
}