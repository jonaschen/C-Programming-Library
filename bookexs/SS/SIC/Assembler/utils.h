#ifndef __UTILS_H
#define __UTILS_H

#include <ctype.h>
#include <stdint.h>

#define IS_SEMICOLON(c)		((c) == ';')
#define IS_COMMA(c)		((c) == ',')
#define IS_COLON(c)		((c) == ':')
#define IS_SPECIAL_SYMBOL(c)	(IS_SEMICOLON(c) || IS_COMMA(c))

#define IS_LINE_END(c)		((c) == '\0' || (c) == '\n')

uint32_t utils_atoh(const char *str);

#endif
