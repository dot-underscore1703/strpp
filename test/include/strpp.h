#ifndef STRPP_H
#define STRPP_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
	size_t length;
	uint8_t *data;
} StrPP;

StrPP *strpp_from_cstr(char *string);
char *cstr_from_strpp(StrPP *string);
size_t strpp_strlen(StrPP string);
size_t strpp_strcap(StrPP string);
StrPP *strpp_strcpy(StrPP *restrict dest, StrPP *restrict src);
StrPP *strpp_strncpy(StrPP *restrict dest, StrPP *restrict src, size_t n);
StrPP *strpp_strcat(StrPP *restrict dest, StrPP *restrict src);
int strpp_strcmp(const StrPP *s1, const StrPP *s2);
StrPP *strpp_strdup(const StrPP *s);

#endif
