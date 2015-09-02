/*
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

void expr();
void term();
void match(int symbol);
void error();

int lookahead;

int main() {
	lookahead = getchar();
	expr();
	putchar('\n');

	return 0;
}

void expr() {
	term();
	for(;;)
		if (lookahead == '+') {
			match('+'); term(); putchar('+');
		} else if (lookahead == '-') {
			match('-'); term(); putchar('-');
		} else break;
}

void term() {
	if (isdigit(lookahead)) {
		putchar(lookahead); match(lookahead);
	} else error();
}

void match(int symbol) {
	if (lookahead == symbol) lookahead = getchar();
	else error();
}

void error() {
	printf("systax error\n");
	exit(1);
}
*/