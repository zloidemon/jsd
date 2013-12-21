/**
 * @file logger.c
 *
 * @brief Simple logger
 *
 */

#include <jsd.h>

daemond d; /**< External damond struct */

void
jsd_logger(jsd_log_t level, const char *fmt, ...)
{ 
  /**
   * Fix me, buffer overload
   */
  char msg[1024];

  va_list ap;
  va_start(ap, fmt);
  vsprintf(msg, fmt, ap);
  va_end(ap);

  switch (level) {
  case JSD_ERR: {
    daemond_say(&d, "<r>ERROR</>: %s", msg);
    break;
  }
  case JSD_INFO: {
    daemond_say(&d, "<g>INFO</>: %s", msg);
    break;
  }
  case JSD_DEBUG:{
    daemond_say(&d, "<n>DEBUG</>: %s", msg);
    break;
  }
  default: break;
  }
}
