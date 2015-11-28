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
const char* reserved_words[] = { "if", "else", "while", "int", "float", "string",
		"return", "const", "break", "continue" };

const char**  get_reserved_words() {
	return reserved_words;
}

/*
 *	Single operators
 */
const char single_operators[] = {'=', '>', '<', '!', '+', '-', '*', '/', '^', '&', '|' };
const char* get_single_operators() {
	return single_operators;
}

/*
 *	Double operators
 */
const char* double_operators[] = {"==", ">=", "<=", "!=", "&&", "||" };
const char** get_double_operators() {
	return double_operators;
}

/*
 *	Delimiters
 */
const char delimiters[DELIMITERS_SIZE] = { '{', '}', '[', ']', '(', ')', ',', ';', ' ', '\n', '\t' };
const char* get_delimiters() {
	return delimiters;
}

/*
 *	String quote
 */
const char string_quote = '"';
const char get_string_quote() {
	return string_quote;
}

/*
 *	Dot
 */
const char dot = '.';
const char get_dot() {
	return dot;
}

/*
 *	comment begin
 */
const char commentary = '#';
const char get_commentary() {
	return commentary;
}
