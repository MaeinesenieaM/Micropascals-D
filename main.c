#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "micro.h"
#include "micro.c"

int main (int argc, char* argv[]) {

	if (argc < 2) {
		printf("ARGUMENTOS INSUFICIENTE!\nTente executar o programa como:\n main.exe teste.pas ou main.exe teste");
		return 2;
	}

	char arquivo[256] = "";
	strcpy(arquivo, argv[1]);
	if (strstr(arquivo, ".lex") == NULL) strcat(arquivo, ".pas");

	FILE *codigo = fopen(arquivo, "r");
	if (codigo == NULL) {
		printf("FALHA AO LER ARQUIVO!\n");
		return 2;
	}

	FILE *lex = fopen("trim.lex", "w");

	char pascal = ' ';

	while (pascal != EOF) {
		pascal = fgetc(codigo);

		Token token = {
			NIL,
			DEFAULT,
			"\0"
		};

		if (isspace(pascal)) continue;

		if (isalpha(pascal)) {
			do {
				char pointer[2] = {pascal, '\0'};
				strcat(token.valor, pointer);
				pascal = fgetc(codigo);
			} while (isalpha(pascal));
			token.ID = token_comp(token.valor);
			token.TYPE = token_type(token.ID);
			printf("%s : %s\n", token.valor, token_type_s(token.TYPE));
			token_print(lex, &token);

			continue;
		}
		else if (isdigit(pascal)) {

			token.TYPE = NUMERO;

			const char *pointer = &pascal;
			do {
				char pointer[2] = {pascal, '\0'};
				strcat(token.valor, pointer);
				pascal = fgetc(codigo);
			} while (isdigit(pascal));

			if (pascal == '.') {
				token.ID = NUM_FLT;
				do {
					char pointer[2] = {pascal, '\0'};
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
				token_print(lex, &token);
				continue;
		}
		else if (ispunct(pascal)) {
			char pointer[2] = {pascal, '\0'};
			strcat(token.valor, pointer);
			if (pascal == ':') {
				pascal = fgetc(codigo);
				if (pascal == '=') {
					token.ID = OP_EQ;
					token.TYPE = OPERADOR;
					char pointer[2] = {pascal, '\0'};
					strcat(token.valor, pointer);
					printf("%s : %s\n", token.valor, token_type_s(token.TYPE));
					token_print(lex, &token);
					continue;
				}
			}
			token.ID = token_comp(token.valor);
			token.TYPE = token_type(token.ID);
			printf("%s : %s\n", token.valor, token_type_s(token.TYPE));
			token_print(lex, &token);
			continue;
		};
	};

	return 0;
}