#ifndef STEG_LSB_COMMON_H
#define STEG_LSB_COMMON_H

// cli options
typedef struct{
    char *input;
    char *output;
    char *msg_str;
    char *msg_file;
    char *mode;
    char *alg;
} Options;

#define LOG_ERROR(msg) \
    fprintf(stderr, "[%s:%d] ERROR: %s\n", __FILE__, __LINE__, msg)

#define LOG_INFO(msg) \
    fprintf(stderr, "[%s:%d] INFO: %s\n", __FILE__, __LINE__, msg)

typedef enum{
    OK_STATUS = 0,
    ERR_FILE_NOT_FOUND,
    ERR_INVALID_FORMAT,
    MEMORY_ERR,
    IO_ERROR,
    ERR_MISSING_MODE_OR_ALG
} ERRStatus;


#endif //STEG_LSB_COMMON_H
