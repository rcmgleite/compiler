/*
 * symbol_table.h
 *
 *  Created on: Nov 27, 2015
 *      Author: rafael
 */

#ifndef SRC_INCLUDE_SYMBOL_TABLE_H_
#define SRC_INCLUDE_SYMBOL_TABLE_H_
#include "token.h"

/*
 *	Symbol table entry struct
 */
typedef struct symbol_table_entry_t {
	char* name;
	int type;
	int is_function;
	int addr;
	struct symbol_table_entry* next;
} symbol_table_entry_t;

/*
 * Symbol table structure
 */
typedef struct symbol_table_t {
	symbol_table_entry_t* first_row;
	struct symbol_table_t* prev;
	unsigned size;
} symbol_table_t;

/*
 *	Return 1 if symbol is preset on symbol table and 0 otherwise
 */
int symbol_table_lookup(symbol_table_t* t, char* name);

/*
 *	Insert a new symbol at the beginning of the symbol table.
 *		Will return 0 if symbol was already inserted and 1 otherwise
 */
int symbol_table_insert(symbol_table_t* t, token_t *token, int type, int is_function);

/*
 *	Print symbol table from current scope to the first
 */
void symbol_table_print(symbol_table_t* t);

#endif /* SRC_INCLUDE_SYMBOL_TABLE_H_ */
