/*
 * main.c
 *
 *  Created on: Sep 22, 2015
 *      Author: rafael
 */
#include <stdio.h>
#include "lex.h"

int main() {
	FILE* fp = fopen("test.cmm", "r");
	if(fp == NULL) {
		fprintf(stderr, "Unable to open file test.c");
		exit(1);
	}
	token_t* t;

	while(TRUE) {
		t = get_token(fp);
		if(t == NULL) {
			break;
		}

		print_token(t);
	}

	printf("\n\n>> FINISHED!\n\n");
	return 0;
}

