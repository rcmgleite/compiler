/*
 * symbol_table.h
 *
 *  Created on: Nov 27, 2015
 *      Author: rafael
 */

#ifndef SRC_INCLUDE_SYMBOL_TABLE_H_
#define SRC_INCLUDE_SYMBOL_TABLE_H_

/*
 *	symbol table node
 */
typedef struct symbol_table_node {
	char* name;
	//thik about type....
	int addr;
	struct symbol_table_node* next;

} symbol_table_node;

/*
 * Symbol table structure
 */
typedef struct symbol_table {
	symbol_table_node* first;
	struct symbol_table* prev;
} symbol_table;



#endif /* SRC_INCLUDE_SYMBOL_TABLE_H_ */
