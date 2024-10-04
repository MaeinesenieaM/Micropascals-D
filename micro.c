#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "micro.h"

//Gerenciador de erros.
void print_error(int error, int linha, int coluna) {
	printf("!UM ERRO OCORREU NA LINHA [%d] COLUNA [%d]!\n", linha, coluna);

	switch (error) {
		case ERROR_SEMARGUMENTO:
			printf ("ARGUMENTOS INSUFICIENTE!\nTente executar o programa como:\n [./main.exe teste.pas] ou [./main.exe teste]");
			exit(ERROR_SEMARGUMENTO);
		case ERROR_LEX_NAOREAL:
			printf ("NUMERO REAL INVALIDO! ESPERADO DIGITO!");
			exit(ERROR_LEX_NAOREAL);
		case ERROR_LEX_PONTOS:
			printf ("NUMEROS NAO PODEM TER MAIS DE UM PONTO!");
			exit(ERROR_LEX_PONTOS);
		case ERROR_LEX_LETRAEMNUMERO:
			printf ("OCORREU UM ERRO AO IDENTIFICAR O NUMERO!\nENCONTRADO LETRA AO EM VEZ DE DIGITO!");
			exit(ERROR_LEX_LETRAEMNUMERO);
		case ERROR_LEX_SIMBOLOINV:
			printf ("OPERADOR OU SIMBOLO INVALIDO!");
			exit(ERROR_LEX_SIMBOLOINV);
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

//#####