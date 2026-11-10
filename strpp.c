#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	size_t length;
	uint8_t *data;
} StrPP;

StrPP *cstr_to_strpp(char *string) {
	size_t length = strlen(string);
	uint8_t *data_buf = malloc(length);
	if(!data_buf){
		return NULL;
	}

	memcpy(data_buf, string, length);	

	StrPP *converted = malloc(sizeof(StrPP));
	if(!converted){
		free(data_buf);
		return NULL;
	}
	converted->length = length;
	converted->data = data_buf;

	return converted;
}

char *strpp_to_cstr(StrPP *string) {
	char *converted = malloc(string->length + 1);

	memcpy(converted, string->data, string->length);

	converted[string->length + 1] = '\0';

	return converted;
}

StrPP *strpp_strcpy(StrPP *restrict dest, StrPP *restrict src) {
	dest->length = src->length;

	StrPP *saved = dest;
	while((*dest->data++ = *src->data++));

	return saved;
}

StrPP *strpp_strncpy(StrPP *restrict dest, StrPP *restrict src, size_t n) {
	dest->length = src->length;

	StrPP *saved = dest;
	size_t i = 0;
	while((*dest->data++ = *src->data++) && i++ < n);

	return saved;
}

StrPP *strpp_strcat(StrPP *restrict dest, StrPP *restrict src) {
	size_t new_len = dest->length + src->length;
	StrPP *catenated = malloc(sizeof(StrPP));

	catenated->data = malloc(new_len);
	catenated->length = new_len;

	memcpy(catenated->data, dest->data, dest->length);
	size_t written = dest->length;
	for(size_t i = 0; i < src->length; ++i) {
		catenated->data[written++] = src->data[i];
	}

	return catenated;
}

int strpp_strcmp(const StrPP *s1, const StrPP *s2) {
	if(s1->length != s2->length) return 1;

	for(size_t i = 0; i < s1->length; ++i){
		if(s1->data[i] != s2->data[i]){
			return 1;
		}
	}

	return 0;	
}

StrPP *strpp_strdup(const StrPP *s) {
	StrPP *duplicated = malloc(sizeof(StrPP));
	duplicated->length = s->length;
	duplicated->data = s->data;

	return duplicated;	
}
