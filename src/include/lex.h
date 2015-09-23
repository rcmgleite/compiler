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
const unsigned operators_size = 8;
const char operators[] = { '=', '>', '<', '!', '+', '-', '*', '/' };

/*
 *	delimiters
 */
const unsigned delimiters_size = 7;
const char delimiters[] = { '{', '}', '[', ']', ',', ';', ' ' };

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
	IN_COMMENT_BEGIN, // #
	IN_DOT, // . -> only used for float numbers
	IN_NEW_LINE, //'\n'
	IN_CLASS_SIZE
} input_class;

/*
 *	Table that represents the Transductor states
 */
const state_t next_state[STATES_SIZE][IN_CLASS_SIZE] = {
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
 *	Handlers
 */
void handle_digit(token_t** curr_token, char received_char, char buffer[]) {
	//TODO
}

void handle_alpha(token_t** curr_token, char received_char, char buffer[]){
	//TODO
}

void handle_operator(token_t** curr_token, char received_char, char buffer[]){
	//TODO
}

void handle_delimiter(token_t** curr_token, char received_char, char buffer[]){
	//TODO
}

void handle_comment_begin(token_t** curr_token, char received_char, char buffer[]){
	//TODO
}

void handle_dot(token_t** curr_token, char received_char, char buffer[]){
	//TODO
}

void handle_new_line(token_t** curr_token, char received_char, char buffer[]){
	//TODO
}

void (* state_function[STATES_SIZE]) (token_t** curr_token, char received_char, char buffer[]) = {
		handle_digit,
		handle_alpha,
		handle_operator,
		handle_delimiter,
		handle_comment_begin,
		handle_dot,
		handle_new_line
};

int is_digit(char c) {
	return c >= '0' && c <= '9';
}

int is_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_operator(char c) {
	unsigned i;
	for(i = 0; i < operators_size; i++) {
		if (c == operators[i])
			return TRUE;
	}
	return FALSE;
}

int is_delimiter(char c) {
	unsigned i;
	for(i = 0; i < delimiters_size; i++){
		if (c == delimiters[i])
			return TRUE;
	}
	return FALSE;
}

int is_comment_begin(char c) {
	return c == commentary;
}

int is_dot(char c) {
	return c == '.';
}

int is_new_line(char c) {
	return c == '\n';
}

/*
 *	classify input read from file by its type
 *		-> it returns -1 if the type does not match any of the specified by the language
 */
input_class classify_input_class(char c) {
	if(is_digit(c))
		return IN_DIGIT;
	else if(is_alpha(c))
		return IN_APLHA;
	else if(is_operator(c))
		return IN_OPERATOR;
	else if(is_delimiter(c))
		return IN_DELIMITER;
	else if(is_comment_begin(c))
		return IN_COMMENT_BEGIN;
	else if(is_dot(c))
		return IN_DOT;
	else if(is_new_line(c))
		return IN_NEW_LINE;
	else
		return -1;
}

/*
 *	main function - get_token(FILE* fp);
 */
token_t* get_token(FILE *fp) {

	/*
	 *	Variables
	 */
	char buffer[255];
	state_t current_state = ST_INIT;
	token_t* curr_token = NULL;

	/*
	 *	Function main loop
	 */
	do {
		/*
		 *	Get next input
		 */
		char curr_input = getc(fp);

		/*
		 *	Classify the new input by its type
		 */
		input_class i_class = classify_input_class(curr_input);

		/*
		 *	Find the next state based on the current state and the input class
		 */
		current_state = next_state[current_state][i_class];
		state_function[current_state](&curr_token, curr_input, buffer);

	} while(current_state != ST_TOKEN_END);

	// create token to return based on the buffer and states used

	return curr_token;
}


#endif /* SRC_INCLUDE_LEX_H_ */
