/*
 * syntatic.c
 *
 *  Created on: Oct 28, 2015
 *      Author: rafael
 */

#include <stdlib.h>
#include "syntatic.h"
#include "utils.h"
#include "token.h"
#include "lex.h"
#include "semantic.h"
#include "tables.h"
#include "string.h"

/*
 *	Structure that represents the state of the analysis
 */
typedef struct {
	int current_sub_machine_state;
	sub_machine_t current_sub_machine;
	sub_machine_t last_sub_machine;

} analysis_state_t;

/*
 *	State function table
 */
int (* const sub_machines[FSM_SIZE]) (token_t* t) = {
	fsm_program,
	fsm_var_declaration,
	fsm_modifier,
	fsm_instruction,
	fsm_loop,
	fsm_cond,
	fsm_return,
	fsm_expr,
	fsm_term_and,
	fsm_term_equal,
	fsm_term_relacional,
	fsm_term_add,
	fsm_term_mult,
	fsm_term_primary
};

/*
 *	Global state variable
 */
analysis_state_t state;


/*
 *	Stack node has the sub-machine and state
 */
typedef struct stack_node {
	sub_machine_t sub_machine;
	int state;
	struct stack_node* next;
} stack_node;

/*
 * 	Stack has just a top node and size
 */
typedef struct stack {
	stack_node* top;
	int size;
} stack_t;

/*
 *	push into the stack
 */
void push(stack_t* s, sub_machine_t sub_machine, int state) {
	stack_node* sn = malloc(sizeof(stack_node*));
	sn->next = s->top;
	sn->sub_machine = sub_machine;
	sn->state = state;
	s->top = sn;
	s->size++;
}

/*
 *	Pop from stack
 *		This function will update the 'state' global variable and free the memory
 *		allocated for the stack_node
 */
void pop(stack_t* s) {
	if(s->size > 0) {
		//update state variable
		state.current_sub_machine = s->top->sub_machine;
		state.current_sub_machine_state = s->top->state;

		//decrement stack size
		s->size--;

		// free stack top and point it to the next item
		stack_node* aux = s->top;
		s->top = s->top->next;
		free(aux);
	} else {
		fprintf(stderr, "[ERROR] Trying to pop from empty stack");
		exit(1);
	}
}

int is_empty(stack_t* s) {
	return s->size == 0;
}

/*
 *	Stack global variable
 */
stack_t stack;

/*
 *	Verify if next token should be issued or not.
 */
int should_get_next_token() {
	if(state.current_sub_machine == state.last_sub_machine) {
		DEBUG("Will return TRUE");
		return TRUE;
	}
	DEBUG("Will return FALSE");
	return FALSE;
}

/*
 * 	Verify if analysis is finished
 */

int finished() {
	if(state.current_sub_machine == FSM_PROGRAM && state.current_sub_machine_state == 0) {
		DEBUG("Will return TRUE");
		return TRUE;
	}
	DEBUG("Will return FALSE");
	return FALSE;
}

/*
 *	Go to next sub_machine
 */
int call_sm(sub_machine_t sm, int ret_st) {
	// push current sub_machine and return state into the stack
	push(&stack, state.current_sub_machine, ret_st);
	// update current sub_machine
	state.current_sub_machine = sm;
	return 0; // state 0 is the next state for the other fsm
}

/*
 * 	Entry point for compilation
 */
int analyze(FILE* fp) {
	token_t* t;
	while(TRUE) {
		if(should_get_next_token() && !finished()) {
			DEBUG("Got new token");
			t = get_token(fp);
			print_token(t);
		}

		state.current_sub_machine_state = sub_machines[state.current_sub_machine](t);
		if(state.current_sub_machine_state == -1) {
			DEBUG("Compilation error!!!!");
			return 1;
		}
	}

	return 0;
}

/*
 *	Internal state functions
 */
int fsm_program(token_t* t){ // TODO - check PUSH POP
	switch(state.current_sub_machine_state) {
	case 0:
		if(t->class == CLASS_RESERVED_WORD && (strcmp(get_reserved_words()[t->value.i_value], "int") == 0
				|| strcmp(get_reserved_words()[t->value.i_value], "float") == 0 )) {
			semantic_tbd();
			return 1;
		}
		break;

	case 1:
		if(t->class == CLASS_IDENTIFIER) {
			semantic_tbd();
			return 2;
		}
		break;

	case 2:
		if(t->class == CLASS_DELIMITER) {
			if(get_delimiters()[t->value.i_value] == '(') {
				semantic_tbd();
				return 3;
			} else if(get_delimiters()[t->value.i_value] == '[') {
				semantic_tbd();
				return 4;
			} else if(get_delimiters()[t->value.i_value] == ';') {
				semantic_tbd();
				return 5;
			}
		}
		break;

	case 3:
		if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == ')'){
			semantic_tbd();
			return 7;
		} else {
			semantic_tbd();
			call_sm(FSM_VAR_DECLARATION, 6);
		}
		break;

	case 4:
		semantic_tbd();
		call_sm(FSM_EXPR, 8);
		break;

	case 5:
		if(t->class == CLASS_RESERVED_WORD && (strcmp(get_reserved_words()[t->value.i_value], "int") == 0
				|| strcmp(get_reserved_words()[t->value.i_value], "float") == 0 )) {
			semantic_tbd();
			return 1;
		}
		break;

	case 6:
		if(t->class == CLASS_DELIMITER) {
			if(get_delimiters()[t->value.i_value] == ',') {
				semantic_tbd();
				return 10;
			} else if(get_delimiters()[t->value.i_value] == ')') {
				semantic_tbd();
				return 7;
			}
		}
		break;

	case 7:


		break;
	}
	// TODO - verificar o return final
	return 0;
}

int fsm_var_declaration(token_t* t){
	return 0;
}

int fsm_modifier(token_t* t) {
	return 0;
}

int fsm_instruction(token_t* t) {
	return 0;
}

int fsm_loop(token_t* t) {
	return 0;
}

int fsm_cond(token_t* t) {
	return 0;
}

int fsm_return(token_t* t) {
	return 0;
}

int fsm_expr(token_t* t) {
	return 0;
}

int fsm_term_and(token_t* t) {
	return 0;
}

int fsm_term_equal(token_t* t) {
	return 0;
}

int fsm_term_relacional(token_t* t) {
	return 0;
}

int fsm_term_add(token_t* t) {
	return 0;
}

int fsm_term_mult(token_t* t) {
	return 0;
}

int fsm_term_primary(token_t* t) {
	return 0;
}

