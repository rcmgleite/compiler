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

#define BUFFER_SIZE 255
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
	IN_CLASS_SIZE
} input_class;

/*
 *	Struct that represents the state function parameters
 */
typedef struct state_struct_t {
	char curr_input;
	char* buffer;
	unsigned *buffer_ptr;
	FILE* fp;
	state_t* last_state;
	state_t* curr_state;
	input_class input_class;
	token_t* token;
} state_struct_t;

/*
 *	state struct lifecycle functions
 */
void create_state_struct(state_struct_t* ss, FILE* fp);
void destroy_state_struct(state_struct_t* ss);

/*
 *	main function
 */
token_t* get_token(FILE *fp);

#endif /* SRC_INCLUDE_LEX_H_ */
