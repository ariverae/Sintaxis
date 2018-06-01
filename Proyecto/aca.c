#include <stdio.h>
#include <stdlib.h>

int main()
{
	char token [100];
	char c;
	int i = 0;
	int j;
	FILE *file;
	file = fopen("test.txt", "r");
	if (file) {
		while ((c = getc(file)) != EOF){
			if(isalpha(c)){
				token[i] = c;
				i++;
			}
			else{
				token[i] = '\0';
				for(j=0; token[j]!='\0'; j++){
					printf("%c", token[j]);
				}
				printf("\n");
				i=0;
			}
		}
		fclose(file);
	}



	return 1;
}
