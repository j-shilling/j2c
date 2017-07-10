/*
 * ErrorHandling.h
 *
 *  Created on: Jul 7, 2017
 *      Author: jake
 */

#ifndef ERRORHANDLING_H_
#define ERRORHANDLING_H_

#include "String.h"
#include <stdbool.h>

void exit_if_not (bool condition, const char *fmt, ...);

#endif /* ERRORHANDLING_H_ */
