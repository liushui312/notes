#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/file.h>

static void _logprint(char *logfile, char *s)
{
	int fd;
	fd = open(logfile, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if(fd < 0)
	{
//		fprintf(stderr, "[%s:%d]%s\n", __FILE__, __LINE__,
//			strerror(errno));
	
		return;
	}

	flock(fd, LOCK_EX);

	write(fd, s, strlen(s));

	flock(fd, LOCK_UN);
	close(fd);
}

void logprint(char *logfile, char *fmt, ...)
{
	va_list arg;
	char buff[1024];

	va_start(arg, fmt);
	vsnprintf(buff, sizeof(buff), fmt, arg);
	va_end(arg);

	_logprint(logfile, buff);
}

void logprintn(char *logfile, char *file, int line, char *fmt, ...)
{
	va_list arg;
	char buff[1024];
	int n;

	va_start(arg, fmt);
	n = snprintf(buff, sizeof(buff), "[%s:%d]", file, line);
	vsnprintf(buff+n, sizeof(buff)-n, fmt, arg);
	va_end(arg);

	_logprint(logfile, buff);
}
