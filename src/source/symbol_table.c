/*
 * symbol_table_t.cpp
 *
 *  Created on: Dec 1, 2015
 *      Author: rafael
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#include "symbol_table.h"

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
////////////////////// SYMBOL TABLE ENTRY FUNCTIONS ///////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

/*
 *	Return a newly allocated entry with next pointing to NULL
 */
static symbol_table_entry_t* new_entry_from_token(token_t* token, int type, int is_function) {
	symbol_table_entry_t* new_entry = malloc(sizeof(symbol_table_entry_t));
	new_entry->name = strdup(token->value.s_value);
	new_entry->type = type;
	new_entry->is_function = is_function;
	new_entry->addr = -1; //TODO
	new_entry->next = NULL;

	return new_entry;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////// SYMBOL TABLE FUNCTIONS //////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

void symbol_table_new_scope(symbol_table_t** t) {
	DEBUG("Opening a new Scope");
	symbol_table_t* new_scope = malloc(sizeof(symbol_table_t));
	new_scope->first_row = NULL;
	new_scope->size = 0;
	new_scope->prev = *t;
	*t = new_scope;
}

void symbol_table_close_scope(symbol_table_t** t) {
	DEBUG("Closing Scope");
	symbol_table_t* aux = (*t)->prev;
	symbol_table_entry_t* curr_row = (*t)->first_row;

	// Clean rows memory
	while(curr_row != NULL) {
		symbol_table_entry_t* next_row = curr_row->next;
		free(curr_row->name);
		free(curr_row);
		curr_row = next_row;
	}

	//Clean symbol_table memory
	free(*t);
	*t = aux;
}

static void do_insert_symbol(symbol_table_t* t, token_t *token, int type, int is_function) {
	symbol_table_entry_t* old_first_row = t->first_row;
	t->first_row = new_entry_from_token(token, type, is_function);
	t->first_row->next = old_first_row;
	t->size++;
}

symbol_table_t* new_symbol_table_t() {
	symbol_table_t* st = malloc(sizeof(symbol_table_t));
	st->size = 0;
	st->prev = NULL;
	st->first_row = NULL;

	return st;
}

int symbol_table_insert(symbol_table_t* t, token_t *token, int type, int is_function) {
	DEBUG("Inserting item on symbol table");
	if (t != NULL) { // if table != null
		if(symbol_table_lookup(t, token->value.s_value)) { // if symbol already present, return 0
			DEBUG("Symbol already present... ERROR");
			return 0;
		}
		DEBUG("Will execute do_insert");
		do_insert_symbol(t, token, type, is_function);
		return 1;
	}
	return 0;
}

int symbol_table_lookup(symbol_table_t* t, char* name) {
	symbol_table_t* aux_table = t;
	while(aux_table != NULL) { // will search in the current table(scope) and all the parents
		symbol_table_entry_t* aux_entry = aux_table->first_row;
		while(aux_entry != NULL) { // will iterate over every row on the table
			if(strcmp(aux_entry->name, name) == 0)
				return 1;
			aux_entry = aux_entry->next;
		}

		aux_table = aux_table->prev;
	}

	return 0;
}

void symbol_table_print(symbol_table_t* t) {
	printf("\n[INFO] ----- Print symbol table -----\n");
	symbol_table_t* aux_table = t;
	while(aux_table != NULL) { // will search in the current table(scope) and all the parents
		symbol_table_entry_t* aux_entry = aux_table->first_row;
		while(aux_entry != NULL) { // will iterate over every row on the table
			printf("[INFO] -- name: %s | type: %d | is_function: %d\n", aux_entry->name, aux_entry->type, aux_entry->is_function);
			aux_entry = aux_entry->next;
		}

		DEBUG("[INFO] Changing scope");
		aux_table = aux_table->prev;
	}
	printf("\n\n");
}


