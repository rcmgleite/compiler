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
#include "symbol_table.h"

/*
 *  Structure that represents the state of the analysis
 */
typedef struct {
  int current_sub_machine_state;
  sub_machine_t current_sub_machine;
  token_t* curr_token;
  token_t* last_token;
  int get_token_flag;
} analysis_state_t;

/*
 *  State function table
 */
int (* const sub_machines[FSM_SIZE]) (token_t* t) = {
  fsm_program,
  fsm_var_declaration,
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
 *  Global state variable
 */
analysis_state_t state;

/*
 *  Global symbol table
 */
symbol_table_t* symbol_table;

/*
 *  Stack node has the sub-machine and state
 */
typedef struct stack_node {
  sub_machine_t sub_machine;
  int state;
  struct stack_node* next;
} stack_node;

/*
 *   Stack has just a top node and size
 */
typedef struct stack {
  stack_node* top;
  int size;
} stack_t;

/*
 *  push into the stack
 */
void push(stack_t* s, sub_machine_t sub_machine, int ret_state) {
  stack_node* sn = malloc(sizeof(stack_node*));
  sn->next = s->top;
  sn->sub_machine = sub_machine;
  sn->state = ret_state;
  s->top = sn;
  s->size++;

  state.get_token_flag = 0;
}

/*
 *  Pop from stack
 *    This function will update the 'state' global variable and free the memory
 *    allocated for the stack_node
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
    state.get_token_flag = 0;
  } else {
    state.current_sub_machine_state = ERROR;
  }
}

int is_empty(stack_t* s) {
  return s->size == 0;
}

/*
 *  Stack global variable
 */
stack_t stack;

/*
 *  Verify if next token should be issued or not.
 */
int should_get_next_token() {
  if(state.get_token_flag) {
    return TRUE;
  }
  return FALSE;
}

/*
 *  Go to next sub_machine
 */
int call_sm(sub_machine_t sm, int ret_st) {
  // push current sub_machine and return state into the stack
  push(&stack, state.current_sub_machine, ret_st);
  // update current sub_machine
  state.current_sub_machine = sm;
  return 0; // state 0 is the next state for the other fsm
}

/*
 *   Entry point for compilation
 */
int analyze(FILE* fp) {
  state.get_token_flag = 1;
  state.current_sub_machine_state = 0;
  state.current_sub_machine = 0;
  state.curr_token = NULL;
  state.last_token = NULL;

  symbol_table = new_symbol_table_t();

  while(TRUE) {
    if(should_get_next_token()) {
      symbol_table_print(symbol_table);
      state.last_token = state.curr_token;
      state.curr_token = get_token(fp);
      if (state.curr_token == NULL) {
        break;
      }
    }

    state.get_token_flag = 1;

    state.current_sub_machine_state = sub_machines[state.current_sub_machine](state.curr_token);
    if(state.current_sub_machine_state == ERROR) {
      DEBUG("Compilation error!!!!");
      return 1;
    }
  }

  DEBUG("Compilation Successful");
  return 0;
}

/*
 *  Aux functions
 */
int is_type(token_t* t) {
  if(t->class == CLASS_RESERVED_WORD && (strcmp(get_reserved_words()[t->value.i_value], "int") == 0
      || strcmp(get_reserved_words()[t->value.i_value], "float") == 0
      || strcmp(get_reserved_words()[t->value.i_value], "string") == 0 )){
    return TRUE;
  }
  return FALSE;
}

/*
 *  Internal state functions
 */
int fsm_program(token_t* t){
  switch(state.current_sub_machine_state) {
  case 0:
    if(is_type(t)) {
      return 1;
    }
    break;

  case 1:
    if(t->class == CLASS_IDENTIFIER) {
      if(symbol_table_insert(symbol_table, t, state.last_token->value.i_value, 1) == 0) {
        fprintf(stderr, "[ERROR] Function declared twice on the same scope");
        return ERROR;
      }
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
      return call_sm(FSM_VAR_DECLARATION, 6);
    }
    break;

  case 4:
    semantic_tbd();
    return call_sm(FSM_EXPR, 9);

  case 5:
    if(is_type(t)) {
      semantic_tbd();
      return 1;
    }

    //final state
    pop(&stack);
    return state.current_sub_machine_state;

  case 6:
    if(t->class == CLASS_DELIMITER) {
      if(get_delimiters()[t->value.i_value] == ',') {
        semantic_tbd();
        return 11;
      } else if(get_delimiters()[t->value.i_value] == ')') {
        semantic_tbd();
        return 7;
      }
    }
    break;

  case 7:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == '{') {
      symbol_table_new_scope(&symbol_table);
      return 8;
    }
    break;

  case 8:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == '}') {
      symbol_table_close_scope(&symbol_table);
      return 5;
    } else {
      semantic_tbd();
      return call_sm(FSM_INSTRUCTION, 8);
    }
    break;

  case 9:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == ']') {
      semantic_tbd();
      return 10;
    }
    break;

  case 10:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == ';') {
      semantic_tbd();
      return 5;
    }
    break;

  case 11:
    semantic_tbd();
    return call_sm(FSM_VAR_DECLARATION, 6);
  }

  return ERROR;
}

int fsm_var_declaration(token_t* t){
  switch(state.current_sub_machine_state) {
  case 0:
    if(is_type(t)) {
      semantic_tbd();
      return 1;
    }
    break;

  case 1:
    if(t->class == CLASS_IDENTIFIER) {
      semantic_tbd();
      if(symbol_table_insert(symbol_table, t, state.last_token->value.i_value, 0) == 0) {
        fprintf(stderr, "[ERROR] Variable declared twice on the same scope");
        return ERROR;
      }
      return 2;
    }
    break;

  case 2:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == '[') {
      semantic_tbd();
      return 3;
    }
    //final state
    pop(&stack);
    return state.current_sub_machine_state;

  case 3:
    semantic_tbd();
    return call_sm(FSM_EXPR, 4);

  case 4:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == '[') {
      semantic_tbd();
      return 5;
    }
    break;

  case 5:
    //final state
    pop(&stack);
    return state.current_sub_machine_state;
  }
  return ERROR;
}

int fsm_instruction(token_t* t) {
  switch(state.current_sub_machine_state) {
  case 0:
    if(t->class == CLASS_RESERVED_WORD) {
      if(strcmp(get_reserved_words()[t->value.i_value], "return") == 0) {
        semantic_tbd();
        return 2;
      } else if(strcmp(get_reserved_words()[t->value.i_value], "break") == 0) {
        semantic_tbd();
        return 1;
      } else if(strcmp(get_reserved_words()[t->value.i_value], "continue") == 0) {
        semantic_tbd();
        return 1;
      } else if(strcmp(get_reserved_words()[t->value.i_value], "while") == 0) {
        semantic_tbd();
        return 3;
      } else if(strcmp(get_reserved_words()[t->value.i_value], "if") == 0) {
        semantic_tbd();
        return 4;
      } else if(strcmp(get_reserved_words()[t->value.i_value], "read") == 0) {
        semantic_tbd();
        return 6;
      } else if(strcmp(get_reserved_words()[t->value.i_value], "write") == 0) {
        semantic_tbd();
        return 7;
      } else {
        semantic_tbd();
        return call_sm(FSM_VAR_DECLARATION, 1);
      }
    } else if(t->class == CLASS_IDENTIFIER) {
      semantic_tbd();
      return 5;
    }
    break;

  case 1:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == ';') {
      semantic_tbd();
      return 8;
    }
    break;

  case 2:
    semantic_tbd();
    return call_sm(FSM_RETURN, 8);

  case 3:
    semantic_tbd();
    return call_sm(FSM_LOOP, 8);

  case 4:
    semantic_tbd();
    return call_sm(FSM_COND, 8);

  case 5: // assing
    if(t->class == CLASS_SINGLE_OPERATOR && get_single_operators()[t->value.i_value] == '=') {
      if (check_scope(symbol_table, state.last_token->value.s_value) == 0) {
        fprintf(stderr, "[ERROR] Variable should be declared before used");
        return ERROR;
      }
      return 14;
    } else if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == '(') {
      if (check_scope(symbol_table, state.last_token->value.s_value) == 0) {
        fprintf(stderr, "[ERROR] Function should be declared before used");
        return ERROR;
      }
      return 9;
    }
    break;

  case 6: // chamada de subrotina READ
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == '(') {
      return 12;
    }
    break;

  case 7: // chamada de subrotina WRITE
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == '(') {
      semantic_tbd();
      return 9;
    }
    break;


  case 8:
    //final state
    pop(&stack);
    return state.current_sub_machine_state;

  case 9:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == ')') {
      semantic_tbd();
      return 1;
    }
    semantic_tbd();
    return call_sm(FSM_EXPR, 10);

  case 10:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == ',') {
      semantic_tbd();
      return 11;
    } else if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == ')') {
      semantic_tbd();
      return 1;
    }
    break;

  case 11:
    semantic_tbd();
    return call_sm(FSM_EXPR, 10);

  case 12:
    if(t->class == CLASS_IDENTIFIER) {
      semantic_tbd();
      return 13;
    }
    break;

  case 13:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == ')') {
      semantic_tbd();
      return 1;
    }
    break;

  case 14:
    semantic_tbd();
    return call_sm(FSM_EXPR, 1);

  }

  return ERROR;
}

int fsm_loop(token_t* t) {
  switch(state.current_sub_machine_state){
  case 0:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == '(') {
      semantic_tbd();
      return 1;
    }
    break;

  case 1:
    semantic_tbd();
    return call_sm(FSM_EXPR, 2);

  case 2:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == ')') {
      semantic_tbd();
      return 3;
    }
    break;

  case 3:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == '{') {
      symbol_table_new_scope(&symbol_table);
      return 4;
    }
    break;

  case 4:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == '}') {
      symbol_table_close_scope(&symbol_table);
      return 5;
    } else {
      semantic_tbd();
      return call_sm(FSM_INSTRUCTION, 4);
    }
    break;

  case 5:
    //final state
    pop(&stack);
    return state.current_sub_machine_state;
  }
  return ERROR;
}

int fsm_cond(token_t* t) {
  switch(state.current_sub_machine_state) {
  case 0:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == '(') {
      semantic_tbd();
      return 1;
    }
    break;

  case 1:
    semantic_tbd();
    return call_sm(FSM_EXPR, 2);

  case 2:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == ')') {
      semantic_tbd();
      return 3;
    }
    break;

  case 3:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == '{') {
      symbol_table_new_scope(&symbol_table);
      return 4;
    }
    break;

  case 4:
    if(t->class == CLASS_RESERVED_WORD && strcmp(get_reserved_words()[t->value.i_value], "else") == 0) {
      semantic_tbd();
      return 5;
    } else if (t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == '}') {
      symbol_table_close_scope(&symbol_table);
      return 6;
    } else {
      semantic_tbd();
      return call_sm(FSM_INSTRUCTION, 4);
    }
    break;

  case 5:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == '}') {
      symbol_table_close_scope(&symbol_table);
      return 6;
    } else {
      semantic_tbd();
      return call_sm(FSM_INSTRUCTION, 5);
    }
    break;

  case 6:
    //final state
    pop(&stack);
    return state.current_sub_machine_state;
  }
  return ERROR;
}

int fsm_return(token_t* t) {
  switch(state.current_sub_machine_state) {
  case 0:
    semantic_tbd();
    int ret = call_sm(FSM_EXPR, 1);
    if (check_scope(symbol_table, state.curr_token->value.s_value) == 0) {
      fprintf(stderr, "[ERROR] Variable should be declared before used");
      return ERROR;
    }
    return ret;

  case 1:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == ';') {
      semantic_tbd();
      return 2;
    }
    break;

  case 2:
    //final state
    pop(&stack);
    return state.current_sub_machine_state;
  }
  return ERROR;
}

int fsm_expr(token_t* t) {
  switch(state.current_sub_machine_state) {
  case 0:
    semantic_tbd();
    return call_sm(FSM_TERM_AND, 1);

  case 1:
    if((t->class == CLASS_SINGLE_OPERATOR && get_single_operators()[t->value.i_value] == '|') ||
        (t->class == CLASS_DOUBLE_OPERATOR && strcmp(get_double_operators()[t->value.i_value], "||") == 0)) {
      semantic_tbd();
      return 2;
    }
    //final state
    pop(&stack);
    return state.current_sub_machine_state;

  case 2:
    semantic_tbd();
    return call_sm(FSM_EXPR, 3);

  case 3:
    //final state
    pop(&stack);
    return state.current_sub_machine_state;
  }
  return ERROR;
}

int fsm_term_and(token_t* t) {
  switch(state.current_sub_machine_state) {
  case 0:
    semantic_tbd();
    return call_sm(FSM_TERM_EQUAL, 1);

  case 1:
    if((t->class == CLASS_SINGLE_OPERATOR && get_single_operators()[t->value.i_value] == '&') ||
        (t->class == CLASS_DOUBLE_OPERATOR && strcmp(get_double_operators()[t->value.i_value], "&&") == 0)) {
      semantic_tbd();
      return 2;
    }
    //final state
    pop(&stack);
    return state.current_sub_machine_state;

  case 2:
    semantic_tbd();
    return call_sm(FSM_TERM_ADD, 3);

  case 3:
    //final state
    pop(&stack);
    return state.current_sub_machine_state;
  }
  return ERROR;
}

int fsm_term_equal(token_t* t) {
  switch(state.current_sub_machine_state) {
  case 0:
    semantic_tbd();
    return call_sm(FSM_TERM_RELACIONAL, 1);

  case 1:
    if(t->class == CLASS_DOUBLE_OPERATOR && (strcmp(get_double_operators()[t->value.i_value], "==") == 0 ||
        strcmp(get_double_operators()[t->value.i_value], "!=") == 0)) {
      semantic_tbd();
      return 2;
    }
    //final state
    pop(&stack);
    return state.current_sub_machine_state;

  case 2:
    semantic_tbd();
    return call_sm(FSM_TERM_EQUAL, 3);

  case 3:
    //final state
    pop(&stack);
    return state.current_sub_machine_state;

  }
  return ERROR;
}

int fsm_term_relacional(token_t* t) {
  switch(state.current_sub_machine_state) {
  case 0:
    semantic_tbd();
    return call_sm(FSM_TERM_ADD, 1);

  case 1:
    if(t->class == CLASS_DOUBLE_OPERATOR && (strcmp(get_double_operators()[t->value.i_value], ">=") == 0 ||
        strcmp(get_double_operators()[t->value.i_value], "<=") == 0)) {
      semantic_tbd();
      return 2;
    } else if(t->class == CLASS_SINGLE_OPERATOR && (get_single_operators()[t->value.i_value] == '>' ||
        get_single_operators()[t->value.i_value] == '<')) {
      return 2;
    }
    //final state
    pop(&stack);
    return state.current_sub_machine_state;

  case 2:
    semantic_tbd();
    return call_sm(FSM_TERM_RELACIONAL, 3);

  case 3:
    //final state
    pop(&stack);
    return state.current_sub_machine_state;

  }
  return ERROR;
}

int fsm_term_add(token_t* t) {
  switch(state.current_sub_machine_state) {
  case 0:
    semantic_tbd();
    return call_sm(FSM_TERM_MULT, 1);

  case 1:
    if(t->class == CLASS_SINGLE_OPERATOR && (get_single_operators()[t->value.i_value] == '+' ||
      get_single_operators()[t->value.i_value] == '-')) {
    return 2;
    }
    //final state
    pop(&stack);
    return state.current_sub_machine_state;

  case 2:
    semantic_tbd();
    return call_sm(FSM_TERM_AND, 3);

  case 3:
    //final state
    pop(&stack);
    return state.current_sub_machine_state;
  }
  return ERROR;
}

int fsm_term_mult(token_t* t) {
  switch(state.current_sub_machine_state) {
    case 0:
      semantic_tbd();
      return call_sm(FSM_TERM_PRIMARY, 1);

    case 1:
      if(t->class == CLASS_SINGLE_OPERATOR && (get_single_operators()[t->value.i_value] == '*' ||
        get_single_operators()[t->value.i_value] == '/' || get_single_operators()[t->value.i_value] == '%')) {
        return 2;
      }
      //final state
      pop(&stack);
      return state.current_sub_machine_state;

    case 2:
      semantic_tbd();
      return call_sm(FSM_TERM_MULT, 3);

    case 3:
      //final state
      pop(&stack);
      return state.current_sub_machine_state;
    }
    return ERROR;
}

int fsm_term_primary(token_t* t) {
  switch(state.current_sub_machine_state) {
  case 0:
    if(t->class == CLASS_IDENTIFIER){
      semantic_tbd();
      return 1;
    } else if(t->class == CLASS_INT || t->class == CLASS_FLOAT || t->class == CLASS_STRING_LIT) {
      semantic_tbd();
      return 2;
    } else if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == '(') {
      semantic_tbd();
      return 3;
    }
    break;

  case 1:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == '(') {
      semantic_tbd();
      return 4;
    }

    //final state
    pop(&stack);
    return state.current_sub_machine_state;

  case 2:
    //final state
    pop(&stack);
    return state.current_sub_machine_state;

  case 3:
    semantic_tbd();
    return call_sm(FSM_EXPR, 5);

  case 4:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == ')') {
      semantic_tbd();
      return 2;
    } else {
      semantic_tbd();
      return call_sm(FSM_EXPR, 6);
    }
    break;

  case 5:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == ')') {
      semantic_tbd();
      return 2;
    }
    break;

  case 6:
    if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == ',') {
      semantic_tbd();
      return 7;
    } else if(t->class == CLASS_DELIMITER && get_delimiters()[t->value.i_value] == ')') {
      semantic_tbd();
      return 2;
    }
    break;

  case 7:
    semantic_tbd();
    return call_sm(FSM_EXPR, 6);
  }
  return ERROR;
}

