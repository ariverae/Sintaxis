#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
enum { false, true };

typedef enum {NUMERO, IDORES, IDFUNCION, IDENTIFICADOR, CARACTER, RESERVADO, OPERADOR} tipoDeToken;

int array[5][7] = {
    {1   ,2   ,3   ,4   ,4   ,106 ,107},
    {1   ,101 ,101 ,101 ,101 ,101 ,101},
    {3   ,2   ,3   ,103 ,102 ,102 ,102},
    {3   ,3   ,3   ,103 ,104 ,104 ,104},
    {105 ,105 ,105 ,105 ,105 ,105 ,105}
};

const char * RES_WORDS[] = {
"auto",
"double",
"int",
"struct",
"break",
"else",
"long",
"switch",
"case",
"enum",
"register",
"typedef",
"char",
"extern",
"return",
"union",
"const",
"float",
"short",
"unsigned",
"continue",
"for",
"signed",
"void",
"default",
"goto",
"sizeof",
"volatile",
"do",
"if",
"static",
"while",
"bool",
"FILE",
"true",
"false"
};


#define resSize (sizeof (RES_WORDS) / sizeof (const char *))

int isspace(int c);
int isdigit(int c);
int isalpha(int c);
int isOneOperator(char c);
int isDoubleOperator(char c);
int isSelfAssigned(char c);
int ispunct(int c);
tipoDeToken verificarPuntuacionTipo(char lexema[], FILE *fp);
int isEndLexema(int c);
int setColumn(char c);
tipoDeToken verificarCadenaTipo(char lexema[]);
void printTokenInfo(char lexema[], int lineaPos, bool outputEnabled, FILE *fp, int tokenAEvaluar);
void createHeader(bool outputEnabled, FILE *fp);

int main(int argc, char *argv[])
{
	char lexema [100];
	char c;
	int column;
	int row = 0;
	int i = 0;
	int lineNumber = 1;
	bool enableOutFile = false;
	FILE *inputFile;
	FILE *outputFile;

	inputFile = fopen(argv[1], "r");
	if(argc > 2){
        outputFile = fopen(argv[2], "w");
        enableOutFile = true;
	}

	if ((inputFile && !enableOutFile ) || (inputFile && enableOutFile && outputFile)) {                                    // Verifies if the file exists
	    createHeader(enableOutFile, outputFile);
		while (row != 107){
            c = getc(inputFile);// Leaves on EOF
            column = setColumn(c);
            row = array[row][column];
            if(isEndLexema(row)){
                if(row != 107){
                    if(row == 106){
                        if(c == 10){
                            lineNumber += 1;
                        }
                    }
                    else if(row == 105){
                        ungetc(c,inputFile);
                        lexema[i] = '\0';
                        row = verificarPuntuacionTipo(lexema, inputFile);
                        printTokenInfo(lexema, lineNumber, enableOutFile, outputFile, row);
                    }
                    else{
                        lexema[i] = '\0';
                        printTokenInfo(lexema, lineNumber, enableOutFile, outputFile, row);
                        ungetc(c,inputFile);
                    }
                row = 0;
                i = 0;
                }
            }else{
                lexema[i] = c;
                i++;
            }
		}

		fclose(inputFile);
        if( enableOutFile ){
            fclose(outputFile);
        }
	}
	else{
        printf("Error al leer el archivo");
        return 0;
	}

	return 1;
}

int isEndLexema(int c){
    if(c > 100)
        return true;
    else
        return false;
}

int isDoubleOperator(char c){
    switch(c){
        case '+':
        case '-':
        case '=':
        case '&':
        case '|':
        case '>':
        case '<':
            return true;
    }
    return false;
}

int isOneOperator(char c){
    switch(c){
        case '.':
        case '~':
            return true;
    }

    return false;
}

int isSelfAssigned(char c){
    switch(c){
        case '/':
        case '*':
        case '%':
        case '!':
            return true;
    }

    return false;
}

int setColumn(char c){
    if(isdigit(c))
        return 0;
    else if(isalpha(c))
        return 1;
    else if(c == '_')
        return 2;
    else if(c == '(')
        return 3;
    else if(c == EOF)
        return 6;
    else if(isspace(c))
        return 5;
    else
        return 4;
}

tipoDeToken verificarPuntuacionTipo(char lexema[], FILE *inputFile){
    char nextChar;
    if(isOneOperator(lexema[0]))
        return OPERADOR;
    else{
        nextChar = getc(inputFile);
        if(isDoubleOperator(lexema[0]) && lexema[0] == nextChar ){
            lexema[1] = nextChar;
            lexema[2] = '\0';
            if(strcmp(lexema, ">>") == 0 || strcmp(lexema, "<<") == 0){
                if((nextChar = getc(inputFile)) == '=' ){
                    lexema[2] = nextChar;
                    lexema[3] = '\0';
                }
                else{
                    ungetc(nextChar, inputFile);
                }
            }
            return OPERADOR;
        }
        else if( (isDoubleOperator(lexema[0]) || isSelfAssigned(lexema[0])) && nextChar == '='){
            lexema[1] = nextChar;
            lexema[2] = '\0';
            return OPERADOR;
        }
        else if( isDoubleOperator(lexema[0]) || isSelfAssigned(lexema[0]) ){
            ungetc(nextChar, inputFile);
            return OPERADOR;
        }
        else{
            ungetc(nextChar, inputFile);
            return CARACTER;
        }
    }
}

// Create Template
void createHeader(bool outputEnabled, FILE *fp)
{
    if(outputEnabled)
    {
        fprintf(fp, "%s", "Nro de Línea");
        fprintf(fp, "\t%s", "Lexema");
        fprintf(fp, "\t%s\n", "Token");
    }
    else
    {
        printf("%s", "Nro de Linea");
        printf("\t%s", "Lexema");
        printf("\t%s\n", "Token");
    }
}

void printTokenInfo(char lexema[], int lineaPos, bool outputEnabled, FILE *fp, int tokenAEvaluar)
{
    char token[20];

    tipoDeToken tokenTipo;

    if(tokenAEvaluar > 100){
        tokenTipo = tokenAEvaluar - 101;
    }
    else{
        tokenTipo = tokenAEvaluar;
    }

    switch(tokenTipo)
    {
        case NUMERO:
            strcpy(token, "Numero");
            break;
        case IDENTIFICADOR:
            strcpy(token, "Identificador");
            break;
        case IDFUNCION:
            if(verificarCadenaTipo(lexema) == RESERVADO){
                strcpy(token, "Reservado");
            }else{
                strcpy(token, "Identificador Funcion");
            }
            break;
        case CARACTER:
            strcpy(token, "Caracter De Puntuacion");
            break;
        case OPERADOR:
            strcpy(token, "Operador");
            break;
        case IDORES:
            if(verificarCadenaTipo(lexema) == RESERVADO){
                strcpy(token, "Reservado");
            }else{
                strcpy(token, "Identificador");
            }
            break;

    }

    if(outputEnabled){
        fprintf(fp, "%-8d", lineaPos );
        fprintf(fp, "\t%s", lexema);
        fprintf(fp, "\t%s\n", token);
    }
    else {
        printf("%-8d", lineaPos);
        printf("\t%s", lexema);
        printf("\t%s\n", token);
    }
}

tipoDeToken verificarCadenaTipo(char lexema[])
{
    int pos;

    for(pos = 0; pos < resSize; pos++)
    {
            if(strcmp(RES_WORDS[pos], lexema) == 0)
            {
                return RESERVADO;
            }
    }

        return IDENTIFICADOR;
}
