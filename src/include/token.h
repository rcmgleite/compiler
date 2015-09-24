/*
 * token.h
 *
 *  Created on: Sep 22, 2015
 *      Author: rafael
 */

#ifndef SRC_INCLUDE_TOKEN_H_
#define SRC_INCLUDE_TOKEN_H_

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
	union value {
		int i_value;
		float f_value;
	} value;
} token_t;

/*
 *	Functions
 */
token_t* new_token(token_class c, void* val);
void print_token(token_t* t);

#endif /* SRC_INCLUDE_TOKEN_H_ */
