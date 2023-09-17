#include <stdio.h>
#include <string.h>
#define MAX_LIMIT 256
int main()
{
	char str[MAX_LIMIT];
	fgets(str, MAX_LIMIT, stdin);
	char* dot_address=strchr(str, '.');
	if (!dot_address)
		dot_address=str+strlen(str);
	printf("\"");
	for (char * char_iterator=dot_address-1;char_iterator>=str;char_iterator--)
		printf("%c", *char_iterator);
	printf("\"");
	return 0;
}
