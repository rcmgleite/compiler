/*
 * tables.c
 *
 *  Created on: Oct 28, 2015
 *      Author: rafael
 */
#include "tables.h"

/*
 *	Reserved words
 */
const char* const reserved_words[] = { "if", "else", "while", "int", "float",
		"return", "const", "break", "continue" };

const char** const get_reserved_words() {
	return reserved_words;
}

/*
 *	Single operators
 */
const char* const single_operators[] = {"=", ">", "<", "!", "+", "-", "*", "/", "^", "&", "|" };
const char** const get_single_operators() {
	return single_operators;
}

/*
 *	Double operators
 */
const char* const double_operators[] = {"==", ">=", "<=", "!=", "&&", "||" };
const char** const get_double_operators() {
	return double_operators;
}

/*
 *	delimiters
 */
const char delimiters[DELIMITERS_SIZE] = { '{', '}', '[', ']', '(', ')', ',', ';', ' ', '\n', '\t' };
const char* const get_delimiters() {
	return delimiters;
}

/*
 *	comment begin
 */
const char commentary = '#';
const char get_commentary() {
	return commentary;
}
