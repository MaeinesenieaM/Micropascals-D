#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "micro.h"
#include "micro.c"

//Diferente de strcat normal, essa função faz com que seja possível cocatenar strings com letras sem
//ter problemas de memoria.
void strcat_char(char *string, char letra) {
	char temp[2] = {letra, '\0'};
	strcat(string, temp);
}

//Essa função aumenta o valor int a cada fgetc de um arquivo.
void getchar_plus(FILE **file, char *letra, int *cont) {
	*letra = fgetc(*file);
	*cont = *cont + 1;
}

void token_read(FILE *file, char *letra, int *cont, char string[]) {
	getchar_plus(&file, letra, cont);
	strcat_char(string, *letra);
}

Token token_analyzer(FILE *file, int *linha, int *coluna, Index *index) {
	Token token = {
		NIL,
		DEFAULT,
		""
	};

	char letra = '';
	fpos_t pos;

	token_read(file, &letra, coluna, token.valor);
	if (isspace(letra)) return token;
	strcat_char(token.valor, letra);

	if (isalpha(letra)) {
		do {
			fgetpos(file, &pos); 
			token_read(file, &letra, coluna, token.valor); 
		} while (isalpha(pascal) || isdigit(pascal));
		fsetpos(file &pos);
		*coluna = *coluna - 1;
		token_update(&token);

		if (token.TYPE == IDENTIFICADOR) {
			int index_pos = search_index(index, token.valor);
			if (index_pos == -1) {
				create_index(&index, token.valor, NONE);
			}
		}
		return token;
	}

	else if (isdigit(pascal)) {

		token.TYPE = NUMERO;

		const char *pointer = &pascal;
		do {
			fgetpos(file, &pos); 
			token_read(file, &letra, coluna, token.valor); 
		} while (isdigit(pascal));

		fsetpos(file &pos);
		*coluna = *coluna - 1;

		if (pascal == '.') {
			token.ID = NUM_FLT;
			strcat_char(token.valor, pascal);
			getchar_plus(&codigo, &pascal, &coluna);
			if (isdigit(pascal) == 0) print_error(ERROR_LEX_NAOREAL, linha, coluna);
			do {
				strcat_char(token.valor, pascal);
				getchar_plus(&codigo, &pascal, &coluna);
			} while (isdigit(pascal));
			if (pascal == '.') print_error(ERROR_LEX_PONTOS, linha, coluna);
		} else token.ID = NUM_INT;
		if (isalpha(pascal)) print_error(ERROR_LEX_LETRAEMNUMERO, linha, coluna);
		token_print(lex, &token);
	}
}

int main (int argc, char* argv[]) {

	int linha = 1, coluna = 1;

	if (argc < 2) {
		printf("ARGUMENTOS INSUFICIENTE!\nTente executar o programa como:\n main.exe teste.pas ou main.exe teste");
		return 2;
	}
	char arquivo[256] = "";
	strcpy(arquivo, argv[1]);
	if (strstr(arquivo, ".pas") == NULL) strcat(arquivo, ".pas");

	FILE *codigo = fopen(arquivo, "r");
	if (codigo == NULL) {
		printf("FALHA AO LER ARQUIVO!\n");
		return 2;
	}
	FILE *lex = fopen("trim.lex", "w");

	char pascal;

	Token token;

	//Index é a lista usada para guardar identificadores.
	Index *index = NULL;

	getchar_plus(&codigo, &pascal, &coluna);

	while (pascal != EOF) {
		token.ID = NIL;
		token.TYPE = DEFAULT;
		strcpy(token.valor, "\0");

		if (isspace(pascal)) {
			if (pascal == '\n') {
				linha++;
				coluna = 1;
			}
			getchar_plus(&codigo, &pascal, &coluna);
		}

		//Isso aki ve se o valor da letra lida é letra.
		else if (isalpha(pascal)) {
			do {
				strcat_char(token.valor, pascal);
				getchar_plus(&codigo, &pascal, &coluna);
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
				sprintf(token.valor, "t%d", pos); //Um print so que dentro de uma string.
			}
			token_print(lex, &token);
		}

		//Isso aki ve se o valor da letra lida é digito.
		else if (isdigit(pascal)) {

			token.TYPE = NUMERO;

			const char *pointer = &pascal;
			do {
				strcat_char(token.valor, pascal);
				getchar_plus(&codigo, &pascal, &coluna);
			} while (isdigit(pascal));

			if (pascal == '.') {
				token.ID = NUM_FLT;
				strcat_char(token.valor, pascal);
				getchar_plus(&codigo, &pascal, &coluna);
				if (isdigit(pascal) == 0) print_error(ERROR_LEX_NAOREAL, linha, coluna);
				do {
					strcat_char(token.valor, pascal);
					getchar_plus(&codigo, &pascal, &coluna);
				} while (isdigit(pascal));
				if (pascal == '.') print_error(ERROR_LEX_PONTOS, linha, coluna);
			} else token.ID = NUM_INT;
			if (isalpha(pascal)) print_error(ERROR_LEX_LETRAEMNUMERO, linha, coluna);
				token_print(lex, &token);
		}

		//Isso aki ve se o valor da letra lida é um simbolo ou pontuação.
		else if (ispunct(pascal)) {
			do {
				strcat_char(token.valor, pascal);
				getchar_plus(&codigo, &pascal, &coluna);
			} while (ispunct(pascal));

			token.ID = token_comp(token.valor);
			token.TYPE = token_type(token.ID);

			if (token.TYPE == IDENTIFICADOR) print_error(ERROR_LEX_SIMBOLOINV, linha, coluna);

			token_print(lex, &token);
		};
	};

	printf("Tabela final de valores identificadores: ");
	show_index(index);
	fclose(codigo);

	return 0;
}