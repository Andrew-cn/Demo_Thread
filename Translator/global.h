#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BSIZE  128
#define NONE   -1
#define EOS    '\0'

#define NUM  256
#define DIV  257
#define MOD  258
#define ID   259
#define DONE 260

struct entry {
	char * lexptr;
	int token;
};

extern int tokenval;
extern int lineno;

extern struct entry symtable[];

void emit(int t, int tval);
void error(char * m);
void init();
int lexan();
void parse();
void expr();
void term();
void factor();
void match(int symbol);
int lookup(char s[]);
int insert(char s[], int tok);
