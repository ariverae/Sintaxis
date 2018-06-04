#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;
enum { false, true };

typedef enum {RESERVADO, IDENTIFICADOR, NUMERO, PUNTUACION, OPERADOR} tipoDeToken;

const char * RES_WORDS[] = {
"auto",
"double",
"int",
"struct",
"switch",
"typedef"
};

#define resSize (sizeof (RES_WORDS) / sizeof (const char *))

int isspace(int c);
int isalpha(int c);
int ispunct(int c);
tipoDeToken verificarCadenaTipo(char lexema[], bool containsAlpha);
void printTokenInfo(char lexema[], int lineaPos, bool outputEnabled, FILE *fp, tipoDeToken tokenTipo);
void createHeader(bool outputEnabled, FILE *fp);

int main(int argc, char *argv[])
{
    tipoDeToken tokenTipo;
	char lexema [100];
	char c;
	int i = 0;
	int lineNumber = 1;
	bool enableOutFile = false;
	bool containsAlpha = false;
	FILE *inputFile;
	FILE *outputFile;

	inputFile = fopen(argv[1], "r");
	if(argc > 2){
        outputFile = fopen(argv[2], "w");
        enableOutFile = true;
	}

	if ((inputFile && !enableOutFile ) || (inputFile && enableOutFile && outputFile)) {                                    // Verifies if the file exists
	    createHeader(enableOutFile, outputFile);
		while ((c = getc(inputFile)) != EOF){           // Leaves on EOF
			if(isspace(c) || ispunct(c) ){              // Verifies if it's space or a punctuation character
                if(i > 0){
                    lexema[i] = '\0';
                    tokenTipo = verificarCadenaTipo(lexema, containsAlpha);
                    printTokenInfo(lexema, lineNumber, enableOutFile, outputFile, tokenTipo);
                    i = 0;
                    containsAlpha = false;
                    ungetc(c, inputFile);
                }
                else{
                    if(ispunct(c)){
                        lexema[0] = c;
                        lexema[1] = '\0';
                        tokenTipo = PUNTUACION;
                        printTokenInfo(lexema, lineNumber, enableOutFile, outputFile, tokenTipo);
                    }
                    else if(c == '\n'){
                       lineNumber++;
                    }
                }

			}
			else{
                if(isalpha(c)){
                    containsAlpha = true;
                }
				lexema[i++] = c;
			}
		}

		if(i > 0){
            lexema[i] = '\0';
            tokenTipo = verificarCadenaTipo(lexema, containsAlpha);
            printTokenInfo(lexema, lineNumber, enableOutFile, outputFile,tokenTipo);
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

void printTokenInfo(char lexema[], int lineaPos, bool outputEnabled, FILE *fp, tipoDeToken tokenTipo)
{
    char token[20];

    switch(tokenTipo)
    {
        case NUMERO:
            strcpy(token, "Numero");
            break;
        case IDENTIFICADOR:
            strcpy(token, "Identificador");
            break;
        case RESERVADO:
            strcpy(token, "Reservado");
            break;
        case PUNTUACION:
            strcpy(token, "Puntuacion");
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

tipoDeToken verificarCadenaTipo(char lexema[], bool containsAlpha)
{
    int pos;

    if(!containsAlpha)
    {
        return NUMERO;
    }
    else{
        for(pos = 0; pos < resSize; pos++)
        {
            if(strcmp(RES_WORDS[pos], lexema) == 0)
            {
                return RESERVADO;
            }
        }

        return IDENTIFICADOR;
    }
}
