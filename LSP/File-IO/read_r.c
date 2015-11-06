#include <sys/types.h>
#include <errno.h>

ssize_t read_r(int fd, char *buf, size_t count)
{
	ssize_t ret, bytes = 0;
	size_t len = count;
	char *ptr = buf;

	while (len > 0 && (ret = read(fd, ptr, len)) != 0) {
		if (ret == -1) {
			if (errno == EINTR)
				continue;
			perror("read_r");
			break;
		}

		len -= ret;
		ptr += ret;
		bytes += ret;
	}

	return bytes;
}
