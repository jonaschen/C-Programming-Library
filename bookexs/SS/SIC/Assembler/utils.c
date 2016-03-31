#include "utils.h"

uint32_t utils_atoh(const char *str)
{
	uint32_t value = 0U;
	const char *ptr = str;

	while (isxdigit(*ptr)) {
		value *= 16;
		if (isdigit(*ptr)) {
			value += (*ptr - '0');
		} else {
			value += (toupper(*ptr) - 'A' + 10);
		}

		ptr++;
	}

	return value;
}
