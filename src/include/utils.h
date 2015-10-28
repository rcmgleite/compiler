/*
 * utils.h
 *
 *  Created on: Sep 22, 2015
 *      Author: rafael
 */

#ifndef SRC_INCLUDE_UTILS_H_
#define SRC_INCLUDE_UTILS_H_

#include <stdio.h>

#define TRUE 1
#define FALSE 0

#define DEBUG(msg) \
	do { \
		printf("%s %d [%s] %s\n", __FILE__, __LINE__, __func__, #msg); \
	} while (0)

#endif /* SRC_INCLUDE_UTILS_H_ */
