/*
 * semantic.c
 *
 *  Created on: Oct 28, 2015
 *      Author: rafael
 */
#include "semantic.h"
#include "utils.h"

void semantic_tbd() {
//  DEBUG("TODO");
}

int check_type(int type1, int type2) {
  return type1 == type2;
}

int check_scope(symbol_table_t* curr_scope, char* name) {
  return symbol_table_lookup(curr_scope, name);
}



