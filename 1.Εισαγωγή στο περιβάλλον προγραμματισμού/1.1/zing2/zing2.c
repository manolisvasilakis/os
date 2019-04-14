#include <unistd.h>
#include <stdio.h>

void zing(void) {
	char *name;
	name = getlogin();
	if (name == NULL) name = "unknown";
	printf("Hi, %s\n", name);
}
