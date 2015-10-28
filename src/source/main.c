/*
 * main.c
 *
 *  Created on: Sep 22, 2015
 *      Author: rafael
 */
#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "syntatic.h"

int main() {
	FILE* fp = fopen("test.cmm", "r");
	if(fp == NULL) {
		fprintf(stderr, "Unable to open file test.cmm");
		exit(1);
	}

	analyze(fp);

	fclose(fp);
	printf("\n>> FINISHED!\n\n");
	return 0;
}

