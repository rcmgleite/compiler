/*
 * tables.h
 *
 *  Created on: Oct 28, 2015
 *      Author: rafael
 */

#ifndef SRC_INCLUDE_TABLES_H_
#define SRC_INCLUDE_TABLES_H_

/*
 *	Reserved words
 */
#define RESERVED_WORDS_SIZE 12
const char** get_reserved_words();

/*
 *	Single operators
 */
#define SINGLE_OPERATORS_SIZE 11
const char* get_single_operators();

/*
 *	Double operators
 */
#define DOUBLE_OPERATORS_SIZE 6
const char** get_double_operators();

/*
 *	delimiters
 */
#define DELIMITERS_SIZE 11
const char* get_delimiters();

/*
 *	String quote
 */
const char get_string_quote();

/*
 *	Dot
 */
const char get_dot();

/*
 *	commentary
 */
const char get_commentary();
#endif /* SRC_INCLUDE_TABLES_H_ */
