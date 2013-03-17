/**
 * @file logger.c
 *
 * @brief Simple logger
 *
 */

#include <jsd.h>

void
jsd_logger(jsd_log_t level, const char *fmt, ...)
{ 

  char msg[1024];

  va_list ap;
  va_start(ap, fmt);

  vsprintf(msg, fmt, ap);
  sprintf(msg, "%s\n", msg);

  va_end(ap);

  switch (level) {
  case JSD_ERR: {
    fprintf(stderr, "ERROR: %s", msg);
    break;
  }
  case JSD_INFO: {
    fprintf(stdout, "INFO: %s", msg);
    break;
  }
  case JSD_DEBUG:{
    fprintf(stdout, "DEBUG: %s",msg);
     break;
  }
  default: break;
  }
}
