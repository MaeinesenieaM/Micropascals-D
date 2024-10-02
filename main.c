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

	Token token;

//	Index *index = (Index*) malloc(sizeof(Index));

	while (pascal != EOF) {
		pascal = fgetc(codigo);

		start:

		token.ID = NIL;
		token.TYPE = DEFAULT;
		strcpy(token.valor, "\0");

		if (isspace(pascal)) continue;

		//Isso aki ve se o valor da letra lida é letra.
		if (isalpha(pascal)) {
			do {
				char pointer[2] = {pascal, '\0'};
				strcat(token.valor, pointer);
				pascal = fgetc(codigo);
			} while (isalpha(pascal));
			token.ID = token_comp(token.valor);
			token.TYPE = token_type(token.ID);
			token_print(lex, &token);
			goto start;
		}
		//Isso aki ve se o valor da letra lida é digito.
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
				char pointer[2] = {pascal, '\0'};
				strcat(token.valor, pointer);
				pascal = fgetc(codigo);
				if (isdigit(pascal) == 0) {
					printf ("%c NUMERO REAL INVALIDO! [%s] ESPERADO DIGITO!", pascal, token.valor);
					return 1;
				}
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
				token_print(lex, &token);
				continue;
		}
		//Isso aki ve se o valor da letra lida é digito.
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
					token_print(lex, &token);
					continue;
				}
			}
			token.ID = token_comp(token.valor);
			token.TYPE = token_type(token.ID);
			token_print(lex, &token);
		};
	};

	return 0;
}