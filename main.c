#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "micro.h"
#include "micro.c"

int main () {

	FILE *codigo = fopen("micropascal.pas", "r");
	FILE *lex = fopen("trim.lex", "w");

	char string [256] = "";
	char pascal;

	pascal = fgetc(codigo);

	Token token = {
		NIL,
		DEFAULT,
		""
	};

	printf("%d\n\n", sizeof(keys) / 8);

	while (pascal != EOF) {


		if (isspace(pascal)) {
			pascal = fgetc(codigo);
			continue;
		};

		if (isalpha(pascal)) {
			do {

				const char *pointer = &pascal;
				strcat(token.valor, pointer);
				pascal = fgetc(codigo);
			} while (isalpha(pascal));
				token.ID = token_comp(token.valor);
				token.TYPE = token_type(token.ID);
				printf("%s : %s\n", token.valor, token_type_s(token.TYPE));
				strcpy(token.valor, "");
				continue;
		}
		else if (isdigit(pascal)) {
			do {
				const char *pointer = &pascal;
				strcat(token.valor, pointer);
				pascal = fgetc(codigo);
			} while (isdigit(pascal));
				token.ID = token_comp(token.valor);
				token.TYPE = token_type(token.ID);
				printf("%s : %s\n", token.valor, token_type_s(token.TYPE));
				strcpy(token.valor, "");
				continue;
		}
		else if (ispunct(pascal)) {
			const char *pointer = &pascal;
			strcat(token.valor, pointer);
			token.ID = token_comp(token.valor);
			token.TYPE = token_type(token.ID);
			printf("%s : %s\n", token.valor, token_type_s(token.TYPE));
			pascal = fgetc(codigo);
			strcpy(token.valor, "");
			continue;
		};

		printf("%c\n", pascal);
		pascal = fgetc(codigo);

		/*	CODIGO MORTO! NAO VE!!!

		printf("I saw [%s]\n", teste);	
		if (strchr(teste, ' ') == NULL) {
			string[strlen(string)] = teste[0];
			printf("I copied [%s] And the string is [%s]\n", teste, string);	
//			printf("%s\n", string);
		}
		else {
			printf("%s\n", string);
			strcpy(string, "");
		};*/
	};

	return 0;
}