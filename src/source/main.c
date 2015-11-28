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

int main(int argc, char **argv) {
	if(argc != 2) {
		fprintf(stdout, "[INFO] Usage: compiler <file>\n");
		return 0;
	}

	FILE* fp = fopen(argv[1], "r");
	if(fp == NULL) {
		fprintf(stderr, "[ERROR] Unable to open file.\n");
		exit(1);
	}

	analyze(fp);

	fclose(fp);
	printf("\n>> FINISHED!\n\n");
	return 0;
}

