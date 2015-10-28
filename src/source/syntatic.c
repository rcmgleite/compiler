/*
 * syntatic.c
 *
 *  Created on: Oct 28, 2015
 *      Author: rafael
 */

#include "syntatic.h"
#include "utils.h"
#include "token.h"
#include "lex.h"

int analyze(FILE* fp) {
	token_t* t;

	while(TRUE) {
		t = get_token(fp);
		if(t == NULL) {
			break;
		}

		print_token(t);

		// TODO - init syntatic
	}

	return 0;
}
