/*
 * token.h
 *
 *  Created on: Sep 22, 2015
 *      Author: rafael
 */

#ifndef SRC_INCLUDE_TOKEN_H_
#define SRC_INCLUDE_TOKEN_H_

#include "stdio.h"

/*
 *	Obs:
 *		IDENTIFIERS -> variable/function names
 *		SPECIAL_SYMBOLS -> { } [ ] . , < > ; + = / *
 */
typedef enum {
	CLASS_INT,
	CLASS_FLOAT,
	CLASS_RESERVED_WORD,
	CLASS_IDENTIFIER,
	CLASS_SPECIAL_SYMBOL
} token_class;

typedef struct token_t {
	token_class class;
	int value;	// will always be an index of an specific table
	int line;
} token_t;

#endif /* SRC_INCLUDE_TOKEN_H_ */
