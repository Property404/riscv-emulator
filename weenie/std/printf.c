#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

static bool string_has(const char* string, char character) {
    for (size_t i=0; string[i] != '\0'; i++) {
        if (string[i] == character)
            return true;
    }
    return false;
}

static void puthex(uint32_t val, bool upper, int width, char pad_character) {
    const char* hex_digits = upper?
        "0123456789ABCDEF":"0123456789abcdef";

    bool hit = false;
    for (int i = sizeof(val)*2-1; i >= 0; i--) {
        const uint8_t nybble = (val >> (4*i)) & 0x0F;
        if (!hit && !nybble && i != 0) {
            if (width > i) {
                putchar(pad_character);
            }
            continue;
        }
        putchar(hex_digits[nybble]);
        hit = true;
    }
}

typedef struct {
    const char* flag;
    unsigned width;
    const char* length;
    const char* type;

    const char* end;
} FormatPlaceholder;

/* Check if `string` begins with any of the prefixes contained in `set`
 * If yes, return a pointer to the prefix in `set`
 * If no, return NULL
 *
 * `Set` is a null-delimited list. We do it this stupid way so we can use `printf`
 * very early, before paging has been set up.
 *
 * If we were to pass an array of char*'s, each char* would point to the VIRTUAL
 * address, because those aren't affected by the GOT, even though the pointer
 * TO the array would be
 *
 * Sets end with two nulls
 * */
static const char* get_first_match(
    const char* __restrict__ string,
    const char* __restrict__ set
) {
    while (*set != '\0') {
        const size_t length = strlen(set);
        if (strncmp(set, string, length) == 0) {
            return set;
        }
        set += length + 1;
    }
    return NULL;
}

// See: https://en.wikipedia.org/wiki/Printf_format_string#Format_placeholder_specification
static int parse_placeholder(const char* fmt, FormatPlaceholder* placeholder) {
    assert(fmt != NULL);
    assert(placeholder != NULL);

    // These HAVE to end with an extra '\0'
    const char* FORMAT_FLAGS = "0\0";
    const char* FORMAT_LENGTHS = "ll\0l\0q\0";
    const char* FORMAT_TYPES = "p\0x\0s\0c\0d\0i\0";

    placeholder->flag = NULL;
    placeholder->width = 0;
    placeholder->length = NULL;
    placeholder->type = NULL;

    const char* ptr = fmt;

    // Find flags
    placeholder->flag = get_first_match(ptr, FORMAT_FLAGS);
    if (placeholder->flag)ptr+=strlen(placeholder->flag);

    // Find width
    while (string_has("0123456789", *ptr)) {
        placeholder->width *= 10;
        placeholder->width += *ptr - '0';
        ptr++;
    }

    // Find length
    placeholder->length = get_first_match(ptr, FORMAT_LENGTHS);
    if (placeholder->length)ptr+=strlen(placeholder->length);

    // Find type
    placeholder->type = get_first_match(ptr, FORMAT_TYPES);
    if (placeholder->type)ptr+=strlen(placeholder->type);

    placeholder->end = ptr;

    return 0;
}


int vprintf(const char* fmt, va_list args) {
    while (*fmt != 0) {
        if (*fmt == '%') {
            fmt++;
            FormatPlaceholder placeholder;
            if (parse_placeholder(fmt, &placeholder)) {
                puts("Failed to parse placeholder!\n");
                return -1;
            }
            fmt = placeholder.end;

            const char type = *(placeholder.type);
            switch (type) {
                case 'p':
                    puts("0x");
                    puthex(va_arg(args, uintptr_t), false, sizeof(uintptr_t)*2, '0');
                    break;
                case 'X':
                case 'x':
                {
                    const bool upper = type <= 'a';
                    const unsigned width = placeholder.width;
                    const char pad_character =
                        (placeholder.flag != NULL && string_has(placeholder.flag, '0'))?'0':' ';
                    if (placeholder.length && !strncmp(placeholder.length, "ll", 2)) {
                        puthex(va_arg(args, long long), upper, width, pad_character);
                    } else if (placeholder.length && !strncmp(placeholder.length, "l", 1)) {
                        puthex(va_arg(args, long), upper, width, pad_character);
                    } else if (placeholder.length && !strncmp(placeholder.length, "q", 1)) {
                        puthex(va_arg(args, uint64_t), upper, width, pad_character);
                    } else  {
                        puthex(va_arg(args, unsigned), upper, width, pad_character);
                    }
                    break;
                }
                case 's':
                    puts(va_arg(args, char*));
                    break;
                case 'c':
                    // integer promotion happens here
                    putchar((va_arg(args, int)));
                    break;
                case '%':
                    putchar('%');
                    break;
                default:
                    puts("[INVALID_TYPE:");
                    puts(placeholder.type);
                    puts("]");
            }
            continue;
        } else {
            putchar(*fmt);
        }
        fmt++;
    }

    return 0;
}


int printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    return 0;
}
