#ifndef MICRO_H
#define MICRO_H

typedef enum {
	OP_EQ, 	// :=	0
	OP_ADD,	// +	1
	OP_SUB,	// -	2
	OP_MUL, // *	3
	OP_DIV, // /	4

	COMP_EQ, // =	5
	COMP_GT, // >	6
	COMP_GE, // >=	7
	COMP_LT, // <	8
	COMP_LE, // <=	9
	COMP_NE, // <> É um comparador com a mesma logica de !=		10

	NUM_INT, // inteiro			11
	NUM_FLT, // float ou real	12

	SMB_OBC, // { OPEN			13
	SMB_CBC, // } CLOSED		14
	SMB_OPA, // (				15
	SMB_CPA, // )				16

	SMB_COM, // ,				17
	SMB_SEM, // ;				18
	SMB_PNT, // .
	SMB_DPT, // :

	//CHAVES
	IF,		 //		19
	ELSE,	 //		20
	ELSE_IF, //		21
	THEN,	 //		22
	WHILE,	 //		23
	DO,		 //		24

	VAR,	 //		25
	PROGRAM, //		26
	BEGIN,	 //		27
	END,	 //		28

	IDENT,	 // identificador para valores ou funções.	29
	LITER,   // literais para valores tipo string.		30
	NIL,	 // significa praticamente nada.			31
} Token_ID;

typedef enum {
	OPERADOR,
	COMPARADOR,
	NUMERO,
	SIMBOLO,
	CHAVE,
	IDENTIFICADOR,
	LITERAL,
	DEFAULT
} Token_TYPE;

typedef struct {
	Token_ID ID;
	Token_TYPE TYPE;
	char valor[64];
} Token;

const char *keys[] = {"if", "else", "else if", "then", "while", "do", "var", "program", "begin", "end", NULL};
const char *num[] = {"integer", "real", NULL};
const char *comp[] = {"=", ">", ">=", "<", "<=", "<>", NULL};
const char *simb[] = {"{", "}", "(", ")", ",", ";", ".", ":", NULL};
const char *oper[] = {":=", "+", "-", "*", "/", NULL};

//NUNCA MUDE A ORDEM DESTES VALORES!!!
const char * const *toks[] = {keys, num, comp, simb, oper};

char *token_typeid_s(Token_ID ID);	//Retorna uma string que representa o token. Ex: "=" ou "int".
char *token_type_s(Token_TYPE type);
Token_TYPE token_type(Token_ID ID);	//Retona o tipo do token.

Token_ID token_get(int id); //Retorna o enum do ID. Se o ID estiver fora do alcance, retornara NIL.
Token_ID token_comp(const char *string); //Compara com o todos tipos de strings e retorna o token correto. Caso contrario ira retornar IDENT.

void token_print(FILE *file, Token *token);

#endif