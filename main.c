#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "micro.h"
#include "micro.c"

int main (int argc, char* argv[]) {

	int lex_toggle = 0;

	int linha = 1, coluna = 1;

	if (argc < 2) {
		printf("ARGUMENTOS INSUFICIENTE!\nTente executar o programa como:\n main.exe teste.pas ou main.exe teste");
		return 2;
	}
	char arquivo[256] = "";
	strcpy(arquivo, argv[1]);
	if (argv[2] != NULL) {
		if (strcmp(argv[2], "-lex") == 0) lex_toggle = 1;
	}
	if (strstr(arquivo, ".pas") == NULL) strcat(arquivo, ".pas");

	FILE *codigo = fopen(arquivo, "r");

	if (codigo == NULL) {
		printf("FALHA AO LER ARQUIVO!\n");
		return 2;
	}
	FILE *lex;
	if (lex_toggle == 1) lex = fopen("trim.lex", "w");

	Index *index = NULL; //Index é a lista usada para guardar identificadores.
	Token token;

	int encap = 0; //Valor responsavel por manter a quantidade de BEGIN e END certa.

	//#### Loop Principal!
	//Se for editar o codigo por favor crie uma branch com o GIT e faça suas mudanças la, depois peça que ela seja aprovada
	//pelo github!
	do {
		read_token(codigo, &linha, &coluna, index, &token);
		
		if (lex_toggle == 1) token_print(lex, &token);

		//Abaixo esta a parte do analisador syntaxico
		switch (token.ID) {
			case PROGRAM:
				do { token = token_analyzer(codigo, &coluna, &index); } while (token.ID == NIL);
				if (token.ID != IDENT) print_error(ERROR_PARSER_SYN_PROGRAM, linha, coluna, token.valor);
				break;
			case IDENTIFICADOR: IDENT:
				read_token(codigo, &linha, &coluna, index, &token);
				//SMB_COM = ,
				if (token.ID == SMB_COM) {
					read_token(codigo, &linha, &coluna, index, &token);
					if (token.ID != IDENTIFICADOR) print_error(ERROR_PARSER_SYN_NOIDENT, linha, coluna, token.valor);
					goto IDENT;
				}
				//SMB_DPT = :
				else if (token.ID == SMB_DPT) {
					read_token(codigo, &linha, &coluna, index, &token);
					if (token.TYPE != NUMERO) print_error(ERROR_PARSER_SYN_NUMTYPE, linha, coluna, token.valor);
				}
				else if (token.ID == OP_EQ)
				break;
			case OP_EQ:
				analisador_expressao_matematica(codigo, &linha, &coluna, index, &token);
				break;
			case IF:
				analisador_expressao_logico(codigo, &linha, &coluna, index, &token);
				if (token.ID != THEN) print_error(ERROR_PARSER_SYN_NOTTHEN, linha, coluna, token.valor);
				encap++;
				break;
			case WHILE:
				analisador_expressao_logico(codigo, &linha, &coluna, index, &token);
				if (token.ID != DO) print_error(ERROR_PARSER_SYN_NOTDO, linha, coluna, token.valor);
				encap++;
				break;
			case BEGIN:
				encap++;
				break;
			case END:
				encap--;
				break;
			case ERR:
				print_error(token.TYPE, linha, coluna, token.valor);
				continue;
		}
	} while (strcmp(token.valor, "") != 0);

	if (encap != 0) {
		char num[4];
		print_error(ERROR_PARSER_SYN_ODDEND, linha, coluna, itoa(encap, num, 10));
	}
	if (lex_toggle == 1) {
		printf("Tabela final de valores identificadores: ");
		show_index(index);
	}
	fclose(codigo);
	fclose(lex);

	return 0;
}