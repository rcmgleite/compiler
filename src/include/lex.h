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

/*
 *	Reserved words
 */
const char* const reserved_words[] = { "if", "else", "while", "int", "float",
		"return", "const" };

/*
 * 	unary and binary operators
 */
const char operators[] = { '=', '>', '<', '!', '+', '-', '*', '/' };

/*
 *	delimiters
 */
const char delimiters[] = { '{', '}', '[', ']', ',', ';' };

/*
 *	comment begin
 */
const char commentary = '#';

/*
 *	Transductor states enum
 */
typedef enum {
	ST_INIT,
	ST_COMMENT,
	ST_NUM_INT,
	ST_NUM_FLOAT,
	ST_APLHANUM,
	ST_OPERATOR,
	ST_DELIMITER,
	ST_TOKEN_END,
	ST_LEX_ERROR,
	STATES_SIZE
} state_t;

/*
 *	possible input characters classes
 */
typedef enum {
	IN_DIGIT, // 0..9
	IN_APLHA, //A-Za-z
	IN_OPERATOR, // '=', '>', '<', '!', '+', '-', '*', '/'
	IN_DELIMITER, //  '{', '}', '[', ']', ',', ';', ' ', '\t'
	COMMENT_BEGIN, // #
	DOT, // . -> only used for float numbers
	NEW_LINE, //'\n'
	INPUT_CLASS_SIZE
} input_class;

/*
 *	Table that represents the Transductor states
 */
const state_t next_state[STATES_SIZE][INPUT_CLASS_SIZE] = {
//char read	digit		apha		operator		DELIMITER		COMM_INIT		DOT		NEW_LINE
/*INIT*/	{ ST_NUM_INT,	ST_APLHANUM, 	ST_OPERATOR,	ST_DELIMITER,	ST_COMMENT,	ST_LEX_ERROR,	ST_TOKEN_END },
/*COMMENT*/	{ ST_COMMENT,	ST_COMMENT,	ST_COMMENT,	ST_COMMENT,	ST_COMMENT,	ST_COMMENT,	ST_TOKEN_END },
/*N_INT*/	{ ST_NUM_INT,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_NUM_FLOAT,	ST_TOKEN_END },
/*FLOAT*/	{ ST_NUM_FLOAT,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_LEX_ERROR,	ST_TOKEN_END },
/*ALPHAN*/	{ ST_APLHANUM,	ST_APLHANUM,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_LEX_ERROR,	ST_TOKEN_END },
/*OPER*/	{ ST_TOKEN_END,	ST_TOKEN_END,	ST_OPERATOR,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END },
/*DELIM*/	{ ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END }
};

/*
 *	Variables
 */
char buffer[255];
state_t current_state;

/*
 *	Handlers
 */
void handle_digit(char received_char) {
	//TODO
}

void handle_alpha(char received_char){
	//TODO
}

void handle_operator(char received_char){
	//TODO
}

void handle_delimiter(char received_char){
	//TODO
}

void handle_comment_begin(char received_char){
	//TODO
}

void handle_dot(char received_char){
	//TODO
}

void handle_new_line(char received_char){
	//TODO
}

void (* state_function[STATES_SIZE]) (char received_char) = {
		handle_digit,
		handle_alpha,
		handle_operator,
		handle_delimiter,
		handle_comment_begin,
		handle_dot,
		handle_new_line
};

/*
 *	main function - get_token(FILE* fp);
 */
token_t get_token(FILE *fp) {
	current_state = ST_INIT;
	do {
		char curr_token = getc(fp);

	} while(current_state != ST_TOKEN_END);
}


#endif /* SRC_INCLUDE_LEX_H_ */
