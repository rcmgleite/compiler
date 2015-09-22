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
	RESERVED_WORD, IDENTIFIER, SPECIAL_SYMBOL
} token_class;

typedef struct token_t {
	token_class class;
	int value;
	int line;
} token_t;

void print_token(const token_t* const t) {
	switch (t->class) {
	case RESERVED_WORD:
		printf("RESERVED_WORD\n");
		break;
	case IDENTIFIER:
		printf("IDENTIFIER\n");
		break;

	case SPECIAL_SYMBOL:
		printf("SPECIAL_SYMBOL\n");
		break;
	}

	// Print value from token
	printf("'%c' at line: %d", t->value, t->line);
}

#endif /* SRC_INCLUDE_TOKEN_H_ */
