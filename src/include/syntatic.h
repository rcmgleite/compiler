/*
 * syntatic.h
 *
 *  Created on: Oct 28, 2015
 *      Author: rafael
 */

#ifndef SRC_INCLUDE_SYNTATIC_H_
#define SRC_INCLUDE_SYNTATIC_H_

#include <stdio.h>
#include "token.h"

typedef enum {
	FSM_PROGRAM,
	FSM_VAR_DECLARATION,
	FSM_MODIFIER,
	FSM_INSTRUCTION,
	FSM_LOOP,
	FSM_COND,
	FSM_RETURN,
	FSM_EXPR,
	FSM_TERM_AND,
	FSM_TERM_EQUAL,
	FSM_TERM_RELACIONAL,
	FSM_TERM_ADD,
	FSM_TERM_MULT,
	FSM_TERM_PRIMARY,
	FSM_SIZE
} sub_machine_t;

/*
 * 	Entry point for compilation
 */
int analyze(FILE *fp);

/*
 *	Internal state functions
 */
int fsm_program(token_t* t);
int fsm_var_declaration(token_t* t);
int fsm_modifier(token_t* t);
int fsm_instruction(token_t* t);
int fsm_loop(token_t* t);
int fsm_cond(token_t* t);
int fsm_return(token_t* t);
int fsm_expr(token_t* t);
int fsm_term_and(token_t* t);
int fsm_term_equal(token_t* t);
int fsm_term_relacional(token_t* t);
int fsm_term_add(token_t* t);
int fsm_term_mult(token_t* t);
int fsm_term_primary(token_t* t);



#endif /* SRC_INCLUDE_SYNTATIC_H_ */
