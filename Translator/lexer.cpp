#include "global.h"

static FILE * in = stdin;

static char lexbuf[BSIZE];

int lineno = 1;
int tokenval = NONE;

int lexan() {
	char ch;
	for(;;) {
		ch = fgetc(in);
		if (ch == ' ' || ch == '\t')
			continue;
		else if (ch == '\n')
			lineno = lineno + 1;
		else if (isdigit(ch)) {
			ungetc(ch, in);
			scanf("%d", &tokenval);
			return NUM;
		} else if (isalpha(ch)) {
			int p, b = 0;
			while (isalnum(ch)) {
				lexbuf[b++] = ch;
				ch = fgetc(in);
				if (b >= BSIZE)
					error("compiler error");
			}
			lexbuf[b] = EOS;
			if (ch != EOF) ungetc(ch, in);
			p = lookup(lexbuf);
			if (p == 0)
				p = insert(lexbuf, ID);
			tokenval = p;
			return symtable[p].token;
		} else if (ch == EOF)
			return DONE;
		else {
			tokenval = NONE;
			return ch;
		}
	}
}
