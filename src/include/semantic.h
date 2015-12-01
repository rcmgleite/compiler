/*
 * semantic.h
 *
 *  Created on: Oct 28, 2015
 *      Author: rafael
 */

#ifndef SRC_INCLUDE_SEMANTIC_H_
#define SRC_INCLUDE_SEMANTIC_H_
#include "symbol_table.h"

void semantic_tbd();
int check_type(int type1, int type2);
int check_scope(symbol_table_t* curr_scope, char* name);


#endif /* SRC_INCLUDE_SEMANTIC_H_ */
