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

//Junta uma letra com uma string e substitui com a proxima letra do arquivo. Além de aumentar um contador.
void switch_char(FILE *file, char *letra, int *cont, char string[]) {
	strcat_char(string, *letra);
	getchar_plus(&file, letra, cont);
}

//Retorna um token de erro.
Token token_error(int codigo, char *string) {
	Token token = {
		ERR,
		codigo,
		""
	};
	strcpy(token.valor, string);
	return token;
}
//Ler um arquivo e retorna um token identificado, junto com aumento do valor COLUNA a cada CHAR lido do arquivo.
Token token_analyzer(FILE *file, int *coluna, Index **index) {
	Token token = {
		NIL,
		DEFAULT,
		""
	};

	char letra;
	fpos_t pos; //Geralmente a posição da ultima letra lida.

	getchar_plus(&file, &letra, coluna);
	if (isspace(letra)) {
		strcat_char(token.valor, letra);
		return token;
	}
	if (letra == EOF) return token;
	if (isalpha(letra)) {	//Detecta identificadores e constantes
		do {
			fgetpos(file, &pos);
			switch_char(file, &letra, coluna, token.valor);
		} while (isalpha(letra) || isdigit(letra));
		token_update(&token);
		if (token.TYPE == IDENTIFICADOR) {
			int index_pos = search_index(*index, token.valor);
			if (index_pos == -1) {
				create_index(index, token.valor, NONE);
			}
		}
	}

	else if (isdigit(letra)) {	//Detecta números
		token.TYPE = NUMERO;
		const char *pointer = &letra;
		do {
			fgetpos(file, &pos); 
			switch_char(file, &letra, coluna, token.valor); 
		} while (isdigit(letra));

		if (letra == '.') {
			token.ID = NUM_FLT;
			fgetpos(file, &pos); 
			switch_char(file, &letra, coluna, token.valor);

			if (isdigit(letra) == 0) {
				token = token_error(ERROR_LEX_NAOREAL, token.valor);
				goto end;
			}
			do {
				fgetpos(file, &pos); 
				switch_char(file, &letra, coluna, token.valor);
			} while (isdigit(letra));
			if (letra == '.') {
				token = token_error(ERROR_LEX_PONTOS, token.valor);
				goto end;
			}
		} else token.ID = NUM_INT;
		if (isalpha(letra)) token = token_error(ERROR_LEX_LETRAEMNUMERO, token.valor);
	}

	else if (ispunct(letra)) {	//Detecta simbolos.
		do {
			fgetpos(file, &pos); 
			switch_char(file, &letra, coluna, token.valor); 
		} while (ispunct(letra));
		token_update(&token);
		if (token.TYPE == IDENTIFICADOR) token = token_error(ERROR_LEX_SIMBOLOINV, token.valor);
	};

	end:
	if (letra != EOF) {
		fsetpos(file, &pos);
		*coluna = *coluna - 1;
	}
	return token;
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
	Index *index = NULL; //Index é a lista usada para guardar identificadores.
	Token token;

	//#### Loop Principal!
	//Se for editar o codigo por favor crie uma branch com o GIT e faça suas mudanças la, depois peça que ela seja aprovada
	//pelo github!
	do {
		token = token_analyzer(codigo, &coluna, &index);
		switch (token.ID) {
			case NIL:
				if (strcmp(token.valor, "\n") == 0) {
					linha++;
					coluna = 1;
				}
				break;
			case ERR:
				print_error(token.TYPE, linha, coluna, token.valor);
				break;
			default:
				token_print(lex, &token);
		}
	} while (strcmp(token.valor, "") != 0);

	printf("Tabela final de valores identificadores: ");
	show_index(index);
	fclose(codigo);
	fclose(lex);

	return 0;
}