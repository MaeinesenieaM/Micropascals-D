#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "micro.h"

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

		case IF:		return "if";
		case ELSE:		return "else";
		case ELSE_IF:	return "else if";
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

	Token_TYPE types[] = {CHAVE, NUMERO, COMPARADOR, SIMBOLO};
//	Token_ID id;
	for (int i = 0; i < sizeof(toks) / 8; i++) {
		for (int j = 0; toks[i][j] != NULL; j++) {
			int diferenca = strcmp(string, toks[i][j]);
			if (diferenca == 0) return token_get(j + token_type_id(types[i]));
		}
	}

	return IDENT;

	/*
	for (int i = 0; i < sizeof(keys) / 8; i++) {
		int diferenca = strcmp(string, keys[i]);
		if (diferenca == 0) return token_get(i + IF);
	};
	for (int i = 0; i < sizeof(num) / 8; i++) {
		int diferenca = strcmp(string, num[i]);
		if (diferenca == 0) return token_get(i + NUM_INT);
	}
	for (int i = 0; i < sizeof(comp) / 8; i++) {
		int diferenca = strcmp(string, num[i]);
		if (diferenca == 0) return token_get(i + COMP_EQ);
	}
	for (int i = 0; i < sizeof(simb) / 8; i++) {
		int diferenca = strcmp(string, simb[i]);
		if (diferenca == 0) return token_get(i + SMB_OBC);
	}
	return IDENT;
	*/
}