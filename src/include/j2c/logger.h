#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <glib.h>
#include <gio/gio.h>

#define j2c_logger_finest(__fmt, ...)	j2c_logger_log (J2C_LOGGER_LEVEL_FINEST, __FILE__, __LINE__, __func__, __fmt, ##__VA_ARGS__)
#define j2c_logger_finer(__fmt, ...)	j2c_logger_log (J2C_LOGGER_LEVEL_FINER, __FILE__, __LINE__, __func__, __fmt, ##__VA_ARGS__)
#define j2c_logger_fine(__fmt, ...)	j2c_logger_log (J2C_LOGGER_LEVEL_FINE, __FILE__, __LINE__, __func__, __fmt, ##__VA_ARGS__)
#define j2c_logger_info(__fmt, ...)	j2c_logger_log (J2C_LOGGER_LEVEL_INFO, __FILE__, __LINE__, __func__, __fmt, ##__VA_ARGS__)
#define j2c_logger_warning(__fmt, ...)	j2c_logger_log (J2C_LOGGER_LEVEL_WARNING, __FILE__, __LINE__, __func__, __fmt, ##__VA_ARGS__)
#define j2c_logger_severe(__fmt, ...)	j2c_logger_log (J2C_LOGGER_LEVEL_SEVERE, __FILE__, __LINE__, __func__, __fmt, ##__VA_ARGS__)
#define j2c_logger_fatal(__fmt, ...)	j2c_logger_log (J2C_LOGGER_LEVEL_FATAL, __FILE__, __LINE__, __func__, __fmt, ##__VA_ARGS__)

typedef enum
{
  J2C_LOGGER_LEVEL_FINEST,
  J2C_LOGGER_LEVEL_FINER,
  J2C_LOGGER_LEVEL_FINE,
  J2C_LOGGER_LEVEL_INFO,
  J2C_LOGGER_LEVEL_WARNING,
  J2C_LOGGER_LEVEL_SEVERE,
  J2C_LOGGER_LEVEL_FATAL,

  J2C_LOGGER_LEVEL_N_LEVELS
} J2cLoggerLevel;

void
j2c_logger_log (J2cLoggerLevel const level,
		gchar const *const file,
		gint const line,
		gchar const *const func,
		gchar const *const __fmt,
		...);
void j2c_logger_heading (gchar const *const heading);

#endif /* __LOGGER_H__ */
