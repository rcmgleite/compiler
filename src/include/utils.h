/*
 * utils.h
 *
 *  Created on: Sep 22, 2015
 *      Author: rafael
 */

#ifndef SRC_INCLUDE_UTILS_H_
#define SRC_INCLUDE_UTILS_H_

#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define ERROR -1

#define DEBUG(msg) do { \
  printf("%s %d [%s] %s\n", __FILE__, __LINE__, __func__, #msg); \
} while (0)

#define UNUSED(x) (void)(x)

static inline char* strdup(const char* s)
{
  size_t len = strlen(s) + 1;
  char* p = malloc(len);

  return p ? memcpy(p, s, len) : NULL;
}

#endif /* SRC_INCLUDE_UTILS_H_ */
