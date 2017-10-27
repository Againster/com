/**
 * @file lz_log_adapter.h
 * @date 2016/04/07 20:00:49
 * @author lissa,lissaagainster@yahoo.com
 * @copyright lissa
 * @brief This file defined output log interface
 */
#ifndef _LZ_LOG_ADAPTER_
#define _LZ_LOG_ADAPTER_

#define LZ_LOG_STDERR 0 
#define LZ_LOG_EMERG  1
#define LZ_LOG_ALERT  2
#define LZ_LOG_CRIT   3
#define LZ_LOG_ERR    4
#define LZ_LOG_WARN   5
#define LZ_LOG_NOTICE 6
#define LZ_LOG_INFO   7
#define LZ_LOG_DEBUG  8

typedef struct lz_log_handler_s lz_log_handler_t;

typedef struct lz_log_s {
  int                             fd;
  lz_log_handler_t               *h;
} lz_log_t;

typedef void (*lz_log_handler_pt)(int log_level,
                                  lz_log_t* log_ctx,
                                  int error_no,
                                  const char* error_format_str,
                                  ...);
struct lz_log_handler_s {
  lz_log_handler_pt               log_handler;
};


#endif /** _LZ_LOG_ADAPTER_ */
