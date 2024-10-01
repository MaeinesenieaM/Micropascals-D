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
	char pascal = ' ';

	while (pascal != EOF) {
		pascal = fgetc(codigo);

		Token token = {
			NIL,
			DEFAULT,
			""
		};

		if (isspace(pascal)) continue;

		if (isalpha(pascal)) {
			const char *pointer = &pascal;
			do {
				strcat(token.valor, pointer);
				pascal = fgetc(codigo);
			} while (isalpha(pascal));
				
			token.ID = token_comp(token.valor);
			token.TYPE = token_type(token.ID);
			printf("%s : %s\n", token.valor, token_type_s(token.TYPE));
			continue;
		}
		else if (isdigit(pascal)) {

			token.TYPE = NUMERO;

			const char *pointer = &pascal;
			do {
				strcat(token.valor, pointer);
				pascal = fgetc(codigo);
			} while (isdigit(pascal));

			if (pascal == '.') {
				token.ID = NUM_FLT;
				do {
					strcat(token.valor, pointer);
					pascal = fgetc(codigo);
				} while (isdigit(pascal));
				if (pascal == '.') {
					printf ("O NUMERO [%s] NAO PODE TER DOIS PONTOS!\n", token.valor);
					return 1;
				}
			} else token.ID = NUM_INT;
			if (isalpha(pascal)) {
				printf("OCORREU UM ERRO AO IDENTIFICAR O NUMERO [%s]\nENCONTRADO LETRA AO EM VEZ DE DIGITO!\n", token.valor);
				return 1;
			}
				printf("%s : %s | %s\n", token.valor, token_type_s(token.TYPE), token_typeid_s(token.ID));
				continue;
		}
		else if (ispunct(pascal)) {
			const char *pointer = &pascal;
			strcat(token.valor, pointer);
			if (pascal == ':') {
				pascal = fgetc(codigo);
				if (pascal == '=') {
					token.ID = OP_EQ;
					token.TYPE = OPERADOR;
					strcat(token.valor, pointer);
					printf("%s : %s\n", token.valor, token_type_s(token.TYPE));
					continue;
				}
			}
			token.ID = token_comp(token.valor);
			token.TYPE = token_type(token.ID);
			printf("%s : %s\n", token.valor, token_type_s(token.TYPE));
			continue;
		};

		printf("%c\n", pascal);
	};

	return 0;
}