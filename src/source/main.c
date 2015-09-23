/*
 * main.c
 *
 *  Created on: Sep 22, 2015
 *      Author: rafael
 */
#include <stdio.h>
#include "lex.h"

int main() {
	FILE* fp = fopen("test.c", "r");
	if(fp == NULL) {
		fprintf(stderr, "Unable to open file test.c");
		exit(1);
	}
	get_token(fp);
	get_token(fp);
	get_token(fp);
	get_token(fp);
	get_token(fp);
	get_token(fp);
	get_token(fp);
	get_token(fp);
	get_token(fp);
	get_token(fp);
	return 0;
}

