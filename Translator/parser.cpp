#include "global.h"

static int lookahead;

void parse() {
	lookahead = lexan();
	while (lookahead != DONE) {
		expr(); match(';');
	}
}

void expr() {
	int t;
	term();
	for(;;)
		switch (lookahead) {
		case '+': case '-':
			t = lookahead;
			match(lookahead); term(); emit(t, NONE);
			continue;
		default:
			return;
		}
}

void term() {
	int t;
	factor();
	for(;;)
		switch (lookahead) {
		case '*': case '/': case DIV: case MOD:
			t = lookahead;
			match(lookahead); factor(); emit(t, NONE);
			continue;
		default:
			return;
		}
}

void factor() {
	switch(lookahead) {
	case '(':
		match('('); expr(); match(')'); break;
	case NUM:
		emit(NUM, tokenval); match(NUM); break;
	case ID:
		emit(ID, tokenval); match(ID); break;
	default:
		error("systax error");
	}
}

void match(int symbol) {
	if (lookahead == symbol) lookahead = lexan();
	else error("syntax error");
}
