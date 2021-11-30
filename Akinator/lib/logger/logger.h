#ifndef LOGGER_H
#define LOGGER_H



enum{
    LOG = -1,
    LOG_INFO = 0,
    LOG_WARN,
    LOG_ERR
};

enum{
    LOG_CONSOLE = 0b1,
    LOG_CONSOLE_STDERR = 0b11,
    LOG_FILE = 0b10000,
};

void log_init(const char* path);
void log_close() __attribute__ ((destructor));
void log_set_path(const char* path);


void pr_log_level(int log_level, int dest, const char* fmt, ...);

#define pr_err(dest, fmt, ...)    pr_log_level(LOG_ERR,   dest,  "%s:%d " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define pr_info(dest, fmt, ...)   pr_log_level(LOG_INFO,  dest,  "%s:%d " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define pr_warn(dest, fmt, ...)   pr_log_level(LOG_WARN,  dest,  "%s:%d " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define pr_log(dest, fmt, ...)    pr_log_level(LOG,       dest,  fmt, ##__VA_ARGS__)

#endif /* LOGGER_H */
