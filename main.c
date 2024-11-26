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

//AVISO! USE SOMENTE read_token PARA ANALISE DE UMA LINHA SÓ!
void read_token(FILE *codigo, int *linha, int *coluna, Index *index, Token *token) {
	do { 
		*token = token_analyzer(codigo, coluna, &index);
		if (strcmp(token->valor, "\n") == 0) {
					*linha++;
					*coluna = 1;
		}
	} while (token->ID == NIL && strcmp(token->valor, "") != 0);
}

void analisador_expressao_matematica(FILE *codigo, int *linha, int *coluna, Index *index, Token *token) {
	int parenteses = 0; //Esse valor diz quantos parenteses faltam para fechar a expressão.


	//Esse codigo talvez seja complicado demais para entender, cuidado ao oq ta embaixo.
repeat:

	read_token(codigo, linha, coluna, index, token);

	switch(token->TYPE) {
		case SIMBOLO:
			//SMB_OPA = ( SMB_CPA = ) SMB_SEM = ;
			if (token->ID == SMB_OPA) {
				parenteses++;
				goto repeat;
			}
			else if (token->ID == SMB_CPA) {
				parenteses--;
				goto repeat;
			}
			else if (token->ID == SMB_SEM) break; //Garante que o analisador termine com {;}
			else print_error(ERROR_PARSER_SYN_NUMTYPE, *linha, *coluna, token->valor);
		case IDENTIFICADOR: case NUMERO:
			read_token(codigo, linha, coluna, index, token);

			//Quando detecta um identificador e espera OPERADORES validos.
			if (token->TYPE == OPERADOR && token->ID != OP_EQ) {
				read_token(codigo, linha, coluna, index, token);
				if (token->TYPE == IDENTIFICADOR || token->TYPE == NUMERO || token->ID == SMB_OPA) goto repeat;
			} else if (token->ID == SMB_SEM || token->ID == THEN || token->ID == DO) break;
			else print_error(ERROR_PARSER_SYN_NOOPMAT, *linha, *coluna, token->valor);
		case OPERADOR:
			if (token->ID != OP_SUB) print_error(ERROR_PARSER_SYN_OPSTART, *linha, *coluna, token->valor);
			read_token(codigo, linha, coluna, index, token);

			if (token->TYPE == NUMERO || token->TYPE == IDENTIFICADOR) goto repeat;
			else print_error(ERROR_PARSER_SYN_IDENMAT, *linha, *coluna, token->valor);
		case CHAVE:
			if (token->ID == THEN || token->ID == DO) break; //Garante que o analisador termine com THEN e DO.
		default:
			print_error(ERROR_PARSER_SYN_NOTEXPR, *linha, *coluna, token->valor);
	}

	if (parenteses != 0) print_error(ERROR_PARSER_SYN_ODDBRACK, *linha, *coluna, token->valor);
}

//expressões logicas não podem ser usadas além de codições.
void analisador_expressao_logico(FILE *codigo, int *linha, int *coluna, Index *index, Token *token) {
	int parenteses = 0; //Esse valor diz quantos parenteses faltam para fechar a expressão.

repeat:	
	read_token(codigo, linha, coluna, index, token);

	switch(token->TYPE) {
		case SIMBOLO:
			//SMB_OPA = ( SMB_CPA = ) SMB_SEM = ;
			if (token->ID == SMB_OPA) {
				parenteses++;
				goto repeat;
			}
			else if (token->ID == SMB_CPA) {
				parenteses--;
				goto repeat;
			}
			//Esta parte esta comentada já que expressoes logicas não usam {;} para terminar
			//a analise, mas caso qualquer coisa descomenta o codigo se for preciso.
		//	else if (token->ID == SMB_SEM) break;
			else print_error(ERROR_PARSER_SYN_IDENMAT, *linha, *coluna, token->valor);
		case IDENTIFICADOR: case NUMERO:
			read_token(codigo, linha, coluna, index, token);

			if (token->TYPE != COMPARADOR) print_error(ERROR_PARSER_SYN_NOTCOMP, *linha, *coluna, token->valor);

			analisador_expressao_matematica(codigo, linha, coluna, index, token);
			if (token->ID == SMB_SEM) print_error(ERROR_PARSER_SYN_NOSEMICO, *linha, *coluna, token->valor);
			break;
		case OPERADOR:
			if (token->ID != OP_SUB) print_error(ERROR_PARSER_SYN_OPSTART, *linha, *coluna, token->valor);
			read_token(codigo, linha, coluna, index, token);

			if (token->TYPE == NUMERO || token->TYPE == IDENTIFICADOR) goto repeat;
			else print_error(ERROR_PARSER_SYN_IDENMAT, *linha, *coluna, token->valor);
		case CHAVE:
			if (token->ID == THEN || token->ID == DO) break;
		default:
			print_error(ERROR_PARSER_SYN_NOTEXPR, *linha, *coluna, token->valor);			
	}


	if (parenteses != 0) print_error(ERROR_PARSER_SYN_ODDBRACK, *linha, *coluna, token->valor);
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
		//Pra fazer! Infelizmente o switch abaixo tem que tornar em uma função para poder fazer o
		//Analaisador Sintatico de uma forma mais comfortavel.
//		token = token_analyzer(codigo, &coluna, &index);
		read_token(codigo, &linha, &coluna, index, &token);
		switch (token.ID) {
			case NIL:
				if (strcmp(token.valor, "\n") == 0) {
					linha++;
					coluna = 1;
				}
				continue;
			case ERR:
				print_error(token.TYPE, linha, coluna, token.valor);
				continue;
			default:
				token_print(lex, &token);
		}
		//switch temporario enquanto eu penso em uma forma melhor de programa isto.
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
			case OP_EQ: COMP_EQ:
				analisador_expressao_matematica(codigo, &linha, &coluna, index, &token);
				break;
		}
	} while (strcmp(token.valor, "") != 0);

	printf("Tabela final de valores identificadores: ");
	show_index(index);
	fclose(codigo);
	fclose(lex);

	return 0;
}