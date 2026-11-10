#include "strpp.h"
#include <stdio.h>

int main(void) {
	StrPP *hello = strpp_from_cstr("Hello,");
	StrPP *world = strpp_from_cstr(" World!");

	StrPP *hello_world = strpp_strcat(hello, world);

	printf("%s\n",cstr_from_strpp(hello_world));
	
	return 0;
}
