#ifndef LOGGER_H
#define LOGGER_H

/*! \file */


enum{
    LOG_INFO = 0,
    LOG_WARN,
    LOG_ERR
};

enum{
    LOG_CONSOLE = 0x1,
    LOG_FILE = 0x2,
};

void log_init(const char* path);
void log_close();


void pr_log_level(int log_level, int dest, const char* fmt, ...);

#define pr_err(dest, fmt, ...)    pr_log_level(LOG_ERR,   dest,  "%s:%d " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define pr_info(dest, fmt, ...)   pr_log_level(LOG_INFO,  dest,  "%s:%d " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

#endif /* LOGGER_H */