#include <stdio.h>
#include <ctype.h>

const char Lower[] = "lower";
const char Upper[] = "upper";

int main(int argc, char *argv[])
{
	char c;
	char action = 'l';

	if (!strncmp(&argv[0][2], Upper, 5))
		action = 'U';

	while (read(0, &c, 1) == 1) {
		if (action == 'U')
			c = toupper(c);
		else
			c = tolower(c);

		write(1, &c, 1);
	}

	return 0;
}

