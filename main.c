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
	if (strstr(arquivo, ".pas") == NULL) strcat(arquivo, ".pas");

	printf("%s", arquivo);

	FILE *codigo = fopen(arquivo, "r");
	if (codigo == NULL) {
		printf("FALHA AO LER ARQUIVO!\n");
		return 2;
	}

	FILE *lex = fopen("trim.lex", "w");

	char pascal = ' ';

	Token token;

	Index *index = NULL;

//(Index*) malloc(sizeof(Index))

	pascal = fgetc(codigo);

	while (pascal != EOF) {

		start:

		token.ID = NIL;
		token.TYPE = DEFAULT;
		strcpy(token.valor, "\0");

		if (isspace(pascal)) {
			pascal = fgetc(codigo);
		}

		//Isso aki ve se o valor da letra lida é letra.
		else if (isalpha(pascal)) {

			do {
				char pointer[2] = {pascal, '\0'};
				strcat(token.valor, pointer);
				pascal = fgetc(codigo);
			} while (isalpha(pascal) || isdigit(pascal));

			token.ID = token_comp(token.valor);
			token.TYPE = token_type(token.ID);

			if (token.TYPE == IDENTIFICADOR) {
				int pos = search_index(index, token.valor);
				if (pos == -1) {
					create_index(&index, token.valor, NONE);
					pos = search_index(index, token.valor);
				}
				strcpy(token.valor, "");
				sprintf(token.valor, "t%d", pos);

				printf ("IDENT: %s POS:%d\n", token.valor, pos);				
			}

			token_print(lex, &token);
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
					printf ("%c NUMERO REAL INVALIDO! [%s] ESPERANDO DIGITO!", pascal, token.valor);
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
		}

		//Isso aki ve se o valor da letra lida é digito.
		else if (ispunct(pascal)) {
			do {
				char pointer[2] = {pascal, '\0'};
				strcat(token.valor, pointer);
				pascal = fgetc(codigo);
			} while (ispunct(pascal));

			token.ID = token_comp(token.valor);
			token.TYPE = token_type(token.ID);

			if (token.TYPE == IDENTIFICADOR) {
				printf("OPERADOR OU SIMBOLO INVALIDO!\n CORRIJA [%s]!", token.valor);
				return 1;
			}

			token_print(lex, &token);
		};
	};

	printf("Tabela final de valores identificadores: ");
	show_index(index);
	fclose(codigo);

	return 0;
}