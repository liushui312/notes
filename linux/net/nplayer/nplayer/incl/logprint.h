#ifndef _LOG_PRINT_H_
#define _LOG_PRINT_H_

#define lprintn(logfile, ...) \
	logprintn(logfile, __FILE__, __LINE__, __VA_ARGS__)

void logprint(char *logfile, char *fmt, ...);
void logprintn(char *logfile, char *file, int line, char *fmt, ...);

#endif //_LOG_PRINT_H_
