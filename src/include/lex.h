/*
 * lex.h
 *
 *  Created on: Sep 22, 2015
 *      Author: rafael
 */

#ifndef SRC_INCLUDE_LEX_H_
#define SRC_INCLUDE_LEX_H_

#include <stdio.h>
#include <stdlib.h>

#include "token.h"
#include "utils.h"

#define BUFFER_SIZE 255

/*
 *	Reserved words
 */
const char* const reserved_words[] = { "if", "else", "while", "int", "float",
		"return", "const" };

/*
 * 	unary and binary operators
 */
#define OPERATORS_SIZE 8
const char operators[OPERATORS_SIZE] = { '=', '>', '<', '!', '+', '-', '*', '/' };

/*
 *	delimiters
 */
#define DELIMITERS_SIZE 11
const char delimiters[DELIMITERS_SIZE] = { '{', '}', '[', ']', '(', ')', ',', ';', ' ', '\n', '\t' };

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

void print_state(state_t s) {
	switch(s){
	case ST_INIT:
		printf("state: ST_INIT");
		break;
	case ST_COMMENT:
		printf("state: ST_COMMENT");
		break;
	case ST_NUM_INT:
		printf("state: ST_NUM_INT");
		break;
	case ST_NUM_FLOAT:
		printf("state: ST_NUM_FLOAT");
		break;
	case ST_APLHANUM:
		printf("state: ST_ALPHANUM");
		break;
	case ST_OPERATOR:
		printf("state: ST_OPERATOR");
		break;
	case ST_DELIMITER:
		printf("state: ST_DELIMITER");
		break;
	case ST_TOKEN_END:
		printf("state: ST_TOKEN_END");
		break;
	case ST_LEX_ERROR:
		printf("state: ST_LEX_ERROR");
		break;
	default:
		printf("state: UNKNOWN");
		break;
	}

	printf("\n");
}

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
	IN_CLASS_SIZE
} input_class;

/*
 *	Table that represents the Transductor states
 *
 *
 *			digit	alpha	operator	DELIMITER	COMM_INIT	DOT
 *	INIT
 *	COMMENT
 *	N_INT
 *	FLOAT
 *	ALPHAN
 *	OP
 *	DELIM
 */
const state_t next_state[STATES_SIZE][IN_CLASS_SIZE] = {
		{ ST_NUM_INT,	ST_APLHANUM, 	ST_OPERATOR,	ST_DELIMITER,	ST_COMMENT,	ST_LEX_ERROR },
		{ ST_COMMENT,	ST_COMMENT,	ST_COMMENT,	ST_COMMENT,	ST_COMMENT,	ST_COMMENT },
		{ ST_NUM_INT,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_NUM_FLOAT },
		{ ST_NUM_FLOAT,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_LEX_ERROR },
		{ ST_APLHANUM,	ST_APLHANUM,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_LEX_ERROR },
		{ ST_TOKEN_END,	ST_TOKEN_END,	ST_OPERATOR,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END },
		{ ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END }
};


/*
 *	Struct that represents the state function parameters
 */
typedef struct state_struct_t {
	char curr_input;
	char* buffer;
	unsigned *buffer_ptr;
	FILE* fp;
	state_t* curr_state;
	input_class input_class;
	token_t* token;
} state_struct_t;

/*
 *	Handlers
 */
void append_input(state_struct_t* param) {
	param->buffer[*(param->buffer_ptr)] = param->curr_input;
	*(param->buffer_ptr)+=1;
	param->buffer[*(param->buffer_ptr)] = 0;

	/*
	 *	Calculate next state;
	 */
	*param->curr_state = next_state[*param->curr_state][param->input_class];
}

void handle_delimiter_init(state_struct_t* param) {
	if((param->curr_input == ' ' || param->curr_input == '\t' || param->curr_input == '\n')) {
		*param->curr_state = ST_INIT;
	} else {
		append_input(param);
	}

}

void ignore_input(state_struct_t* param) {
	// just ignore the input
}

void handle_token_end(state_struct_t* param) {
	if((param->curr_input == ' ' || param->curr_input == '\t' || param->curr_input == '\n')){
	} else {
		ungetc(param->curr_input, param->fp);
	}
	*param->curr_state = ST_TOKEN_END;
}

void handle_error(state_struct_t* param) {
	printf("Compilation error");
	exit(1);
}

/*
 *			digit	alpha	operator	DELIMITER	COMM_INIT	DOT
 *	INIT
 *	COMMENT
 *	N_INT
 *	FLOAT
 *	ALPHAN
 *	OP
 *	DELIM
 */
void (* state_function[STATES_SIZE][IN_CLASS_SIZE]) (state_struct_t* param) = {
		{ append_input, append_input, append_input, handle_delimiter_init, append_input, append_input},
		{ ignore_input, ignore_input, ignore_input, ignore_input, ignore_input, ignore_input},
		{ append_input, handle_token_end, handle_token_end, handle_token_end, handle_token_end, append_input},
		{ append_input, handle_token_end, handle_token_end, handle_token_end, handle_token_end, handle_error},
		{ append_input, append_input, handle_token_end, handle_token_end, handle_token_end, handle_error},
		{ handle_token_end, handle_token_end, append_input, handle_token_end, handle_token_end, handle_token_end},
		{ handle_token_end, handle_token_end, handle_token_end, handle_token_end, handle_token_end, handle_token_end}
};

int is_digit(char c) {
	return c >= '0' && c <= '9';
}

int is_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_operator(char c) {
	unsigned i;
	for(i = 0; i < OPERATORS_SIZE; i++) {
		if (c == operators[i])
			return TRUE;
	}
	return FALSE;
}

int is_delimiter(char c) {
	unsigned i;
	for(i = 0; i < DELIMITERS_SIZE; i++){
		if (c == delimiters[i]) {
			return TRUE;
		}
	}
	return FALSE;
}

int is_comment_begin(char c) {
	return c == commentary;
}

int is_dot(char c) {
	return c == '.';
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

	state_struct_t state_struct;
	state_struct.buffer = malloc(BUFFER_SIZE * sizeof(char));
	state_struct.curr_state = malloc(sizeof(state_t));
	state_struct.buffer_ptr = malloc(sizeof(unsigned));
	*state_struct.curr_state = ST_INIT;
	*state_struct.buffer_ptr = 0;
	state_struct.fp = fp;

	/*
	 *	Main loop
	 */
	do {
		/*
		 *	Get next input
		 */
		state_struct.curr_input = getc(fp);

		/*
		 *	update state_struct
		 */
		state_struct.input_class = classify_input_class(state_struct.curr_input);

		state_function[*state_struct.curr_state][state_struct.input_class](&state_struct);
	} while(*state_struct.curr_state != ST_TOKEN_END);

	/*
	 *	Create token to return based on the buffer and states used
	 *	TODO
	 */
	printf("buffer: %s\n", state_struct.buffer);
	/*
	 *	Free dynamic allocated memory and return
	 */
	free(state_struct.buffer);
	free(state_struct.buffer_ptr);
	free(state_struct.curr_state);
	return state_struct.token;
}


#endif /* SRC_INCLUDE_LEX_H_ */
