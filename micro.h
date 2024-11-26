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

	SMB_COM, // ,	17
	SMB_SEM, // ;	18
	SMB_PNT, // .	19
	SMB_DPT, // :	20

	//CHAVES
	IF,		 //		21
	ELSE,	 //		22
	THEN,	 //		23
	WHILE,	 //		24
	DO,		 //		25

	VAR,	 //		26
	PROGRAM, //		27
	BEGIN,	 //		28
	END,	 //		29

	IDENT,	 // identificador para valores ou funções.	30
	LITER,   // literais para valores tipo string.		31
	NIL,	 // significa praticamente nada.			32
	ERR,	 // ERROR 33
} Token_ID;

typedef enum {
	OPERADOR,
	COMPARADOR,
	NUMERO,
	SIMBOLO,
	CHAVE,
	IDENTIFICADOR,
	LITERAL,
	DEFAULT,
	ERR_TYPE,
} Token_TYPE;

typedef struct {
	Token_ID ID;
	Token_TYPE TYPE;
	char valor[64];
} Token;

typedef enum {
	INT,
	REAL,
	CHAR,
	NONE
} Index_TYPE;

typedef struct Index {
	char ident[64];
	Index_TYPE TYPE;
	void *valor;
	struct Index *next;
} Index;

const char *keys[] = {"if", "else", "then", "while", "do", "var", "program", "begin", "end", NULL};
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

void token_update(Token *token); //Atualiza o ID e o TIPO do token.

void token_print(FILE *file, Token *token); //responsavel por dar print no console e salvar o token em .lex

int search_index(Index *list, const char *string);
void change_index_t(Index *list, Index_TYPE type, int pos);
void create_index(Index **list, const char *string, Index_TYPE type);
void show_index(Index *list);

//Responsavel por escrever em um arquivo e no console os tokens detectados.
void token_print(FILE *file, Token *token);

//Abaixo a uma lista de possíveis erros.
#define ERROR_SEMARGUMENTO			001 //ARGUMENTOS INSUFICIENTES!

#define ERROR_LEX_NAOREAL			101 //NUMERO REAL INVALIDO! ESPERADO NUMERO!
#define ERROR_LEX_PONTOS			102 //NUMEROS REAIS NAO PODEM TER MAIS DE UM PONTO!
#define ERROR_LEX_LETRAEMNUMERO		103 //ENCONTRADO LETRA AO EM VEZ DE NUMERO!
#define ERROR_LEX_SIMBOLOINV		104 //OPERADOR OU SIMBOLO INVALIDO!

#define ERROR_PARSER_SYN_PROGRAM	201 //ESPERADO IDENTIFICADOR!
#define ERROR_PARSER_SYN_NOIDENT	202 //ESPERADO IDENTIFICADOR!
#define ERROR_PARSER_SYN_NUMTYPE	203 //ESPERADO TIPO NUMERO!
#define ERROR_PARSER_SYN_UNSYMBOL	204 //SIMBOLO EM LUGAR ILEGAL!
#define ERROR_PARSER_SYN_NOOPMAT	205 //ESPERADO OPERADOR MATEMATICO!
#define ERROR_PARSER_SYN_IDENMAT	206 //ESPERADO NUMERO OU IDENTIFICADOR!
#define ERROR_PARSER_SYN_ODDBRACK	207	//QUANTIDADE DE ENCAPSULAMENTO ILEGAL!
#define ERROR_PARSER_SYN_NOTEXPR	208 //EXPRESSAO COM PALAVRA INVALIDA!
#define ERROR_PARSER_SYN_NOTCOMP	209 //ESPERADO COMPARADOR LOGICO!
#define ERROR_PARSER_SYN_NOSEMICO	210	//ESPERADO FIM DA EXPRESSAO COM THEN OU DO, NAO {;}!
#define ERROR_PARSER_SYN_OPSTART	211 //OPERADOR ILEGAL NO COMECO DE ESPRESSAO!

void print_error(int error, int linha, int coluna, const char *string);

//A função pricipal para obter token atraves de um arquivo.
Token token_analyzer(FILE *file, int *coluna, Index **index);

#endif