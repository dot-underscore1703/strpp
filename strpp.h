#ifndef STRPP_H
#define STRPP_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
	size_t length;
	uint8_t *data;
} StrPP;

StrPP *cstr_to_strpp(char *string);
char *strpp_to_cstr(StrPP *string);
StrPP *strpp_strcpy(StrPP *restrict dest, StrPP *restrict src);
StrPP *strpp_strncpy(StrPP *restrict dest, StrPP *restrict src, size_t n);
StrPP *strpp_strcat(StrPP *restrict dest, StrPP *restrict src);
int strpp_strcmp(const StrPP *s1, const StrPP *s2);
StrPP *strpp_strdup(const StrPP *s);

#endif
