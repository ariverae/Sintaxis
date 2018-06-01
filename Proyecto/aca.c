#include <stdio.h>
#include <stdlib.h>

int main()
{
	char c;
	FILE *file;
	file = fopen("test.txt", "r");
	if (file) {
		while ((c = getc(file)) != EOF)
			putchar(c);
		fclose(file);
	}



	return 1;
}
