#include "strpp.h"
#include <stdio.h>

int main(void) {
	StrPP *hello = cstr_to_strpp("Hello,");
	StrPP *world = cstr_to_strpp(" World!");

	StrPP *hello_world = strpp_strcat(hello, world);

	printf("%s\n",strpp_to_cstr(hello_world));
	
	return 0;
}
