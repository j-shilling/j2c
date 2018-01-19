#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include <glib.h>
#include <gio/gio.h>

#include <j2c/logger.h>

J2cLoggerLevel j2c_options_logger_level     (void);
gint           j2c_options_max_threads      (void);
GFile *        j2c_options_log_file         (void);
GFile *        j2c_options_output_file      (void);
GFile **       j2c_options_class_path_files (void);
GFile **       j2c_options_target_files     (void);

void j2c_parse_options (gint argc, gchar *argv[]);

#endif /* __OPTIONS_H__ */
