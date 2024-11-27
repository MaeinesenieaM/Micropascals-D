#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "micro.h"

//Gerenciador de erros.
void print_error(int error, int linha, int coluna, const char *string) {
	printf("!UM ERRO OCORREU NA LINHA [%d] COLUNA [%d]!\n", linha, coluna);

	if (string != NULL) printf("[%s] ", string);

	switch (error) {
		case ERROR_SEMARGUMENTO:
			printf("ARGUMENTOS INSUFICIENTES!\nTente executar o programa como:\n {./main.exe teste.pas} ou {./main.exe teste}");
			exit(ERROR_SEMARGUMENTO);
		case ERROR_LEX_NAOREAL:
			printf("NUMERO REAL INVALIDO! ESPERADO NUMERO!");
			exit(ERROR_LEX_NAOREAL);
		case ERROR_LEX_PONTOS:
			printf("NUMEROS REAIS NAO PODEM TER MAIS DE UM PONTO!");
			exit(ERROR_LEX_PONTOS);
		case ERROR_LEX_LETRAEMNUMERO:
			printf("ENCONTRADO LETRA AO EM VEZ DE NUMERO!");
			exit(ERROR_LEX_LETRAEMNUMERO);
		case ERROR_LEX_SIMBOLOINV:
			printf("OPERADOR OU SIMBOLO INVALIDO!");
			exit(ERROR_LEX_SIMBOLOINV);

		case ERROR_PARSER_SYN_PROGRAM:
			printf("ESPERADO IDENTIFICADOR!");
			exit(ERROR_PARSER_SYN_PROGRAM);
		case ERROR_PARSER_SYN_NOIDENT:
			printf("ESPERADO IDENTIFICADOR!");
			exit(ERROR_PARSER_SYN_NOIDENT);
		case ERROR_PARSER_SYN_NUMTYPE:
			printf("ESPERADO TIPO NUMERO!");
			exit(ERROR_PARSER_SYN_NUMTYPE);
		case ERROR_PARSER_SYN_UNSYMBOL:
			printf("SIMBOLO EM LUGAR ILEGAL!");
			exit(ERROR_PARSER_SYN_UNSYMBOL);
		case ERROR_PARSER_SYN_NOOPMAT:
			printf("ESPERADO OPERADOR MATEMATICO!");
			exit(ERROR_PARSER_SYN_NOOPMAT);
		case ERROR_PARSER_SYN_IDENMAT:
			printf("ESPERADO NUMERO OU IDENTIFICADOR!");
			exit(ERROR_PARSER_SYN_IDENMAT);
		case ERROR_PARSER_SYN_ODDBRACK:
			printf("QUANTIDADE DE ENCAPSULAMENTO ILEGAL!");
			exit(ERROR_PARSER_SYN_ODDBRACK);
		case ERROR_PARSER_SYN_NOTEXPR:
			printf("EXPRESSAO COM PALAVRA INVALIDA!");
			exit(ERROR_PARSER_SYN_NOTEXPR);
		case ERROR_PARSER_SYN_NOTCOMP:
			printf("ESPERADO COMPARADOR LOGICO!");
			exit(ERROR_PARSER_SYN_NOTCOMP);
		case ERROR_PARSER_SYN_NOSEMICO:
			printf("ESPERADO FIM DA EXPRESSAO COM THEN OU DO, NAO {;}!");
			exit(ERROR_PARSER_SYN_NOSEMICO);
		case ERROR_PARSER_SYN_OPSTART:
			printf("OPERADOR ILEGAL NO COMECO DE ESPRESSAO!");
			exit(ERROR_PARSER_SYN_OPSTART);

		case ERROR_PARSER_SYN_NOTDO:
			printf("ESPERADO CHAVE {DO}!");
			exit(ERROR_PARSER_SYN_NOTDO);
		case ERROR_PARSER_SYN_NOTTHEN:
			printf("ESPERADO CHAVE {THEN}!");
			exit(ERROR_PARSER_SYN_NOTTHEN);
		case ERROR_PARSER_SYN_ODDEND:
			printf("QUANTIDADE DE {BEGIN} E {END} ILEGAL!");
			exit(ERROR_PARSER_SYN_ODDEND);

		case ERROR_PARSER_SYN_NOTMATH:
			printf("ESPERADO ESPRESSAO MATEMATICA!");
			exit(ERROR_PARSER_SYN_NOTMATH);

		default:
			printf ("ERRO DESCONHECIDO!");
			exit(1);
	}
}

//#####

char *token_typeid_s(Token_ID ID) {
	switch (ID) {
		case OP_EQ:	  return "=";
		case OP_ADD:  return "+";
		case OP_SUB:  return "-";
		case OP_MUL:  return "*";
		case OP_DIV:  return "/";

		case COMP_EQ: return "=";
		case COMP_GT: return ">";
		case COMP_GE: return ">=";
		case COMP_LT: return "<";
		case COMP_LE: return "<=";
		case COMP_NE: return "<>";

		case NUM_INT: return "int";
		case NUM_FLT: return "float";

		case SMB_OBC: return "{";
		case SMB_CBC: return "}";
		case SMB_OPA: return "(";
		case SMB_CPA: return ")";

		case SMB_COM: return ",";
		case SMB_SEM: return ";";
		case SMB_PNT: return ".";
		case SMB_DPT: return ":";

		case IF:		return "if";
		case ELSE:		return "else";
		case THEN:		return "then";
		case WHILE:		return "while";
		case DO:		return "do";

		case VAR:		return "var";
		case PROGRAM:	return "program";
		case BEGIN:		return "begin";
		case END:		return "end";

		case IDENT:		return "NIL IDENT";
		case LITER:		return "NIL LITER";
		case NIL:		return "NIL";

		default: return "TOKENERROR";
	}
}

char *token_type_s (Token_TYPE type) {
	switch (type) {
		case OPERADOR:		return "OPERADOR";
		case COMPARADOR:	return "COMPARADOR";
		case NUMERO:		return "NUMERO";
		case SIMBOLO:		return "SIMBOLO";
		case CHAVE:			return "CHAVE";
		case IDENTIFICADOR:	return "IDENTIFICADOR";
		case LITERAL:		return "LITERAL";
		default:			return "DEFAULT";
	}
}

//Retorna o id do inicia da lista de cada tipo.
Token_ID token_type_id (Token_TYPE type) {
	switch (type) {
		case OPERADOR:		return OP_EQ;
		case COMPARADOR:	return COMP_EQ;
		case NUMERO:		return NUM_INT;
		case SIMBOLO:		return SMB_OBC;
		case CHAVE:			return IF;
		case IDENTIFICADOR:	return IDENT;
		case LITERAL:		return LITER;
		default:			return NIL;
	}
}

Token_TYPE token_type (Token_ID ID) {
	if (ID < COMP_EQ) return OPERADOR;
	else if(ID < NUM_INT) return COMPARADOR;
	else if(ID < SMB_OBC) return NUMERO;
	else if(ID < IF		) return SIMBOLO;
	else if(ID < IDENT 	) return CHAVE;
	else if(ID == IDENT ) return IDENTIFICADOR;
	else if(ID == LITER ) return LITERAL;
	else return DEFAULT;
}

Token_ID token_get(int id) {
	if (id < NIL) return (Token_ID) id;
	else return NIL;
}

Token_ID token_comp(const char *string) {

	Token_TYPE types[] = {CHAVE, NUMERO, COMPARADOR, SIMBOLO, OPERADOR};

	for (int i = 0; i < sizeof(toks) / sizeof(toks[0]); i++) {
		for (int j = 0; toks[i][j] != NULL; j++) {
			int diferenca = strcmp(string, toks[i][j]);
			if (diferenca == 0) {
				 return token_get(j + token_type_id(types[i]));
			}
		}
	}
	return IDENT;
}

void token_update(Token *token) {
	token->ID = token_comp(token->valor);
	token->TYPE = token_type(token->ID);
}

//#####

int search_index(Index *list, const char *string) {
	int count = 0;
	while (list != NULL) {
		count++;
		if (strcmp(string, list->ident) == 0) return count - 1;
		list = list->next;
	}
	return -1;
}

void change_index_t(Index *list, Index_TYPE type, int pos) {
	while (pos > 0) {
		list->next;
		pos--;
	};
	list->TYPE = type;
}

void create_index(Index **list, const char *string, Index_TYPE type) {	

	if (*list == NULL) {
		*list = (Index*) malloc(sizeof(Index));
		strcpy ((*list)->ident, string);
		(*list)->TYPE = type;
		(*list)->next = NULL;
		return;
	}
	Index *temp = (Index*) malloc(sizeof(Index));
	strcpy (temp->ident, string);
	temp->TYPE = type;
	temp->next = NULL;

	Index *atual = *list;

	while (atual->next != NULL) atual = atual->next;
	atual->next = temp;
}

void show_index(Index *list) {
	printf("[");
	while (list != NULL) {
		printf ("%s ", list->ident);
		if (list->next == NULL) break;
		list = list->next;
	}
	printf("]\n");
}

void token_print(FILE *file, Token *token) {
	fprintf(file, "<%d|%s|%s|%s>\n", token->ID, token_typeid_s(token->ID), token_type_s(token->TYPE), token->valor);
	printf("<%d|%s|%s|%s>\n", token->ID, token_typeid_s(token->ID), token_type_s(token->TYPE), token->valor);
}

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
					*linha = *linha + 1;
					*coluna = 1;
		}
	} while (token->ID == NIL && strcmp(token->valor, "") != 0);
}


int analisador_expressao_matematica(FILE *codigo, int *linha, int *coluna, Index *index, Token *token) {
	int parenteses = 0; //Esse valor diz quantos parenteses faltam para fechar a expressão.

	int ident = 0;
	//Esse codigo talvez seja complicado demais para entender, cuidado ao oq ta embaixo.
repeat:
	read_token(codigo, linha, coluna, index, token);

	switch(token->TYPE) {
		case SIMBOLO: SIMB:
			ident++;
			//SMB_OPA = ( SMB_CPA = ) SMB_SEM = ;
			if (token->ID == SMB_OPA) {
				parenteses++;
				goto repeat;
			}
			else if (token->ID == SMB_CPA) {
				parenteses = parenteses - 1;
				goto repeat;
			}
			else if (token->ID == SMB_SEM) break; //Garante que o analisador termine com {;}
			else print_error(ERROR_PARSER_SYN_NUMTYPE, *linha, *coluna, token->valor);
		case IDENTIFICADOR: case NUMERO:
			ident++;
			read_token(codigo, linha, coluna, index, token);

			//Quando detecta um identificador e espera OPERADORES validos.
			if (token->TYPE == OPERADOR && token->ID != OP_EQ) goto repeat;
			else if (token->TYPE == SIMBOLO) goto SIMB;
			else if (token->ID == SMB_SEM || token->ID == THEN || token->ID == DO || token->TYPE == COMPARADOR) break;
			else print_error(ERROR_PARSER_SYN_NOOPMAT, *linha, *coluna, token->valor);
		case OPERADOR:
			ident++;
			read_token(codigo, linha, coluna, index, token);

			if (token->TYPE == NUMERO || token->TYPE == IDENTIFICADOR) goto repeat;
			else print_error(ERROR_PARSER_SYN_IDENMAT, *linha, *coluna, token->valor);
		case CHAVE:
			if (token->ID == THEN || token->ID == DO) break; //Garante que o analisador termine com THEN e DO.
		default:
			print_error(ERROR_PARSER_SYN_NOTEXPR, *linha, *coluna, token->valor);
	}

	if (parenteses != 0) print_error(ERROR_PARSER_SYN_ODDBRACK, *linha, *coluna, token->valor);
	return ident;
}

//expressões logicas não podem ser usadas além de codições.
void analisador_expressao_logico(FILE *codigo, int *linha, int *coluna, Index *index, Token *token) {
	if(analisador_expressao_matematica(codigo, linha, coluna, index, token) == 0) print_error(ERROR_PARSER_SYN_NOTMATH, *linha, *coluna, token->valor);

	switch(token->TYPE) {
		case COMPARADOR:
			if(analisador_expressao_matematica(codigo, linha, coluna, index, token) == 0) print_error(ERROR_PARSER_SYN_NOTMATH, *linha, *coluna, token->valor);
			if (token->ID == THEN || token->ID == DO) break;
		default:
			print_error(ERROR_PARSER_SYN_NOTCOMP, *linha, *coluna, token->valor);			
	}
}

//#####