/*
 * lex.h
 *
 *  Created on: Sep 22, 2015
 *      Author: rafael
 */

#ifndef SRC_INCLUDE_LEX_H_
#define SRC_INCLUDE_LEX_H_

#include <stdio.h>

#include "token.h"
#include "utils.h"

typedef enum {
	ST_INITIAL,
	ST_INT,
	ST_FLOAT,
	ST_COMMENT,
	ST_APLHANUM,
	ST_SPECIAL_CHAR,
	STATE_T_SIZE
} state_t;

typedef enum {
	I_DIGIT, I_CHAR, I_SPECIAL_C, INPUT_CLASS_SIZE
} input_class;

/*
 *	Palavras reservadas
 */
const char* const keywords[] = { "if", "else", "while", "int", "float",
		"return", "const" };

state_t curr_state;

token_t get_token(FILE *fp) {
	curr_state = ST_INITIAL;
	char curr_char;		// current character that will be read
	char buffer[255];	// buffer for the already read chars

	while (TRUE) {
		curr_char = getc(fp);
	}
	return 0;
}

#endif /* SRC_INCLUDE_LEX_H_ */
