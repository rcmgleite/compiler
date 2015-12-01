/*
 * lex.c
 *
 *  Created on: Sep 24, 2015
 *      Author: rafael
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tables.h"
#include "lex.h"

/*
 *	state struct lifecycle functions
 */
void create_state_struct(state_struct_t* ss, FILE* fp) {
	ss->buffer = malloc(BUFFER_SIZE * sizeof(char));
	ss->last_state = malloc(sizeof(state_t));
	ss->curr_state = malloc(sizeof(state_t));
	ss->buffer_ptr = malloc(sizeof(unsigned));
	*ss->last_state = ST_INIT;
	*ss->curr_state = ST_INIT;
	*ss->buffer_ptr = 0;
	ss->fp = fp;
	ss->token = NULL;
}

void destroy_state_struct(state_struct_t* ss) {
	free(ss->buffer);
	free(ss->buffer_ptr);
	free(ss->curr_state);
	free(ss->last_state);
}

/*
 *	Table that represents the Transductor states
 *
 *
 *			digit	alpha	operator	DELIMITER	COMM_INIT	DOT   STRING_QUOTE
 *	INIT
 *	COMMENT
 *	N_INT
 *	FLOAT
 *	STRING_LIT
 *	ALPHAN
 *	OP
 *	DELIM
 *	LEX_ERROR
 */
const state_t next_state[STATES_SIZE][IN_CLASS_SIZE] = {
		{ ST_NUM_INT,	ST_APLHANUM, 	ST_OPERATOR,	ST_DELIMITER,	ST_COMMENT,	ST_LEX_ERROR, ST_STR_LIT},
		{ ST_COMMENT,	ST_COMMENT,	ST_COMMENT,	ST_COMMENT,	ST_COMMENT,	ST_COMMENT, ST_COMMENT },
		{ ST_NUM_INT,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_NUM_FLOAT, ST_TOKEN_END },
		{ ST_NUM_FLOAT,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_LEX_ERROR, ST_LEX_ERROR },
		{ ST_STR_LIT,	ST_STR_LIT,	ST_STR_LIT,	ST_STR_LIT,	ST_STR_LIT,	ST_STR_LIT, ST_TOKEN_END},
		{ ST_APLHANUM,	ST_APLHANUM,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_LEX_ERROR, ST_LEX_ERROR },
		{ ST_TOKEN_END,	ST_TOKEN_END,	ST_OPERATOR,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END, ST_TOKEN_END },
		{ ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END,	ST_TOKEN_END, ST_TOKEN_END },
		{ ST_LEX_ERROR,	ST_LEX_ERROR,	ST_LEX_ERROR,	ST_LEX_ERROR,	ST_LEX_ERROR,	ST_LEX_ERROR, ST_LEX_ERROR }
};

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
	*param->last_state = *param->curr_state;
	*param->curr_state = next_state[*param->curr_state][param->input_class];
}

void handle_delimiter_init(state_struct_t* param) {
	if((param->curr_input == ' ' || param->curr_input == '\t' || param->curr_input == '\n')) {
		*param->curr_state = ST_INIT;
	} else{
		append_input(param);
	}

}

void ignore_input(state_struct_t* param) {
	// just ignore the input
}

void handle_comment_init(state_struct_t* param) {
	*param->curr_state = ST_COMMENT;
}

void handle_comment_delimiter(state_struct_t* param) {
	if(param->curr_input == '\n')
		*param->curr_state = ST_INIT;
}

void handle_token_end(state_struct_t* param) {
	if((param->curr_input == ' ' || param->curr_input == '\t' || param->curr_input == '\n')){
	} else {
		ungetc(param->curr_input, param->fp);
	}
	*param->last_state = *param->curr_state;
	*param->curr_state = ST_TOKEN_END;
}

void handle_str_lit(state_struct_t* param) {
	if(param->curr_input == '"' && *param->curr_state == ST_INIT) {
		*param->last_state = *param->curr_state;
		*param->curr_state = ST_STR_LIT;
	} else if(param->curr_input != '"') {
		param->buffer[*(param->buffer_ptr)] = param->curr_input;
		*(param->buffer_ptr)+=1;
		param->buffer[*(param->buffer_ptr)] = 0;
	} else if(param->curr_input == '"' && *param->curr_state == ST_STR_LIT){
		*param->last_state = *param->curr_state;
		*param->curr_state = ST_TOKEN_END;
	}
}

void handle_error(state_struct_t* param) {
	printf(">>ERROR: BUILD FAILED!\n");
	exit(1);
}

/*
 *			digit	alpha	operator	DELIMITER	COMM_INIT	DOT	STRING_QUOTE
 *	INIT
 *	COMMENT
 *	N_INT
 *	FLOAT
 *	STR_LIT
 *	ALPHAN
 *	OP
 *	DELIM
 *	LEX_ERROR
 */
void (* state_function[STATES_SIZE][IN_CLASS_SIZE]) (state_struct_t* param) = {
		{ append_input,	append_input,	append_input,	handle_delimiter_init,	handle_comment_init,	handle_error, handle_str_lit },
		{ ignore_input,	ignore_input,	ignore_input,	handle_comment_delimiter,	ignore_input,	ignore_input, ignore_input },
		{ append_input,	handle_token_end,	handle_token_end,	handle_token_end,	handle_token_end,	append_input, handle_error },
		{ append_input,	handle_token_end,	handle_token_end,	handle_token_end,	handle_token_end,	handle_error, handle_error },
		{ handle_str_lit, handle_str_lit, handle_str_lit, handle_str_lit, handle_str_lit, handle_str_lit, handle_str_lit},
		{ append_input,	append_input,	handle_token_end,	handle_token_end,	handle_token_end,	handle_error, handle_error },
		{ handle_token_end,	handle_token_end,	append_input,	handle_token_end,	handle_token_end,	handle_token_end, handle_token_end },
		{ handle_token_end,	handle_token_end,	handle_token_end,	handle_token_end,	handle_token_end,	handle_token_end, handle_token_end },
		{ handle_error,	handle_error,	handle_error,	handle_error,	handle_error,	handle_error, handle_error }
};

/*
 *	Helpers
 */
int is_digit(char c) {
	return c >= '0' && c <= '9';
}

int is_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_operator(char c) {
	unsigned i;
	for(i = 0; i < SINGLE_OPERATORS_SIZE; i++) {
		if (c == get_single_operators()[i])
			return TRUE;
	}
	return FALSE;
}

int is_delimiter(char c) {
	unsigned i;
	for(i = 0; i < DELIMITERS_SIZE; i++){
		if (c == get_delimiters()[i]) {
			return TRUE;
		}
	}
	return FALSE;
}

int is_comment_begin(char c) {
	return c == get_commentary();
}

int is_dot(char c) {
	return c == get_dot();
}

int is_string_quote(char c) {
	return c == get_string_quote();
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
	else if(is_string_quote(c))
		return IN_STRING_QUOTE;
	else
		return -1;
}

void build_token(state_struct_t* param) {
	unsigned i;
	float f_val;
	int i_val;
	switch(*param->last_state) {
	case ST_APLHANUM:
		for(i = 0; i < RESERVED_WORDS_SIZE; i++) {
			if(strcmp(param->buffer, get_reserved_words()[i]) == 0)
				break;
		}

		if(i == RESERVED_WORDS_SIZE) {
			param->token = new_token(CLASS_IDENTIFIER, param->buffer);
		} else {
			param->token = new_token(CLASS_RESERVED_WORD, &i);
		}
		break;
	case ST_DELIMITER:
		for(i = 0; i < DELIMITERS_SIZE; i++){
			if(param->buffer[0] == get_delimiters()[i])
				break;
		}
		param->token = new_token(CLASS_DELIMITER, &i);
		break;
	case ST_NUM_FLOAT:
		f_val = strtof(param->buffer, NULL);
		param->token = new_token(CLASS_FLOAT, &f_val);
		break;
	case ST_NUM_INT:
		i_val = strtol(param->buffer, NULL, 10);
		param->token = new_token(CLASS_INT, &i_val);
		break;
	case ST_STR_LIT:
		param->token = new_token(CLASS_STRING_LIT, param->buffer);
		break;
	case ST_OPERATOR:
		if(*param->buffer_ptr == 1){
			for(i = 0; i < SINGLE_OPERATORS_SIZE; i++) {
				if(param->buffer[0] == get_single_operators()[i])
					break;
			}
			param->token = new_token(CLASS_SINGLE_OPERATOR, &i);
		} else {
			for(i = 0; i < DOUBLE_OPERATORS_SIZE; i++) {
				if(param->buffer[0] == get_double_operators()[i][0])
					break;
			}
			param->token = new_token(CLASS_DOUBLE_OPERATOR, &i);
		}
		break;
	default:
		fprintf(stderr, "[ERROR] Shouldn't get at 'default' option of build_token\nUnknown error\n");
	}
}

/*
 *	main function
 */
token_t* get_token(FILE *fp) {
	state_struct_t state_struct;
	create_state_struct(&state_struct, fp);

	/*
	 *	Main loop
	 */
	do {
		/*
		 *	Get next input
		 */
		state_struct.curr_input = getc(fp);
		if(state_struct.curr_input == EOF) {
			break;
		}

		fprintf(stdout, "[DEBUG] curr_char = %c\n", state_struct.curr_input);

		/*
		 *	update state_struct
		 */
		state_struct.input_class = classify_input_class(state_struct.curr_input);

//		fprintf(stdout, "[DEBUG] curr_state = %d\n", *state_struct.curr_state);
		state_function[*state_struct.curr_state][state_struct.input_class](&state_struct);
	} while(*state_struct.curr_state != ST_TOKEN_END);

	/*
	 * 	Build token to be returned
	 */
	if(*state_struct.buffer_ptr != 0){
		printf("[INFO] buffer: %s\n", state_struct.buffer);
		build_token(&state_struct);
//		print_token(state_struct.token);
	}

	/*
	 *	Free dynamic allocated memory and return
	 */
	destroy_state_struct(&state_struct);
//	print_symbol_table();

	return state_struct.token;
}
