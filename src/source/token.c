/*
 * token.c
 *
 *  Created on: Sep 24, 2015
 *      Author: rafael
 */
#include <stdlib.h>
#include <stdio.h>

#include "token.h"

token_t* new_token(token_class c, int v) {
	token_t* t = (token_t*)malloc(sizeof(token_t));
	t->class = c;
	t->value = v;
	return t;
}

void print_token(token_t* t) {
	switch(t->class){
	case CLASS_INT:
		printf("token_class: %s\n", "CLASS_INT");
		break;
	case CLASS_FLOAT:
		printf("token_class: %s\n", "CLASS_FLOAT");
		break;
	case CLASS_RESERVED_WORD:
		printf("token_class: %s\n", "CLASS_RESERVED_WORD");
		break;
	case CLASS_IDENTIFIER:
		printf("token_class: %s\n", "CLASS_IDENTIFIER");
		break;
	case CLASS_SINGLE_OPERATOR:
		printf("token_class: %s\n", "CLASS_SINGLE_OPERATOR");
		break;
	case CLASS_DOUBLE_OPERATOR:
		printf("token_class: %s\n", "CLASS_DOUBLE_OPERATOR");
		break;
	case CLASS_DELIMITER:
		printf("token_class: %s\n", "CLASS_DELIMITER");
		break;
	default:
		printf("token_class: %s\n", "UNDEFINED");
		break;
	}

	printf("token_value: %d\n\n", t->value);
}



