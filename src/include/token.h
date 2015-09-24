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
	CLASS_INT, // int number
	CLASS_FLOAT, // float number
	CLASS_RESERVED_WORD, // if, while, int, ...
	CLASS_IDENTIFIER, // variable name
	CLASS_SINGLE_OPERATOR, // '=', '>', '<', '!', '+', '-', '*', '/'
	CLASS_DOUBLE_OPERATOR, // "==", ">=", "<=", "!="
	CLASS_DELIMITER, //  '{', '}', '[', ']', ',', ';', ' ', '\t'
} token_class;

typedef struct token_t {
	token_class class;
	int value;	// will be an index of an specific table or the actual value of a float or int numbers
} token_t;

token_t* new_token(token_class c, int v) {
	token_t* t = (token_t*)malloc(sizeof(token_t));
	t->class = c;
	t->value = v;
	return t;
}

void print_token_class(token_t* t) {
	switch(t->class){
	case CLASS_INT:
		printf("token_class: %s\n", "CLASS_INT");
		break;
	case CLASS_FLOAT:
		printf("token_class: %s\n", "CLASS_FLOAT");
		break;
	case CLASS_RESERVED_WORD:
		printf("token_class: %s\n", "CLASS_RESERVED_WORD");
		break;
	case CLASS_IDENTIFIER:
		printf("token_class: %s\n", "CLASS_IDENTIFIER");
		break;
	case CLASS_SINGLE_OPERATOR:
		printf("token_class: %s\n", "CLASS_SINGLE_OPERATOR");
		break;
	case CLASS_DOUBLE_OPERATOR:
		printf("token_class: %s\n", "CLASS_DOUBLE_OPERATOR");
		break;
	case CLASS_DELIMITER:
		printf("token_class: %s\n", "CLASS_DELIMITER");
		break;
	default:
		printf("token_class: %s\n", "UNDEFINED");
		break;
	}
}

#endif /* SRC_INCLUDE_TOKEN_H_ */
