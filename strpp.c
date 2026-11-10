#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	size_t capacity;
	size_t written;
	char *data;
} StrPP;

StrPP *strpp_from_cstr(char *string) {
	StrPP *dest = malloc(sizeof(StrPP));

	size_t length = strlen(string);
	char *data_buf = malloc(length - 1);
	if(!data_buf){
		return NULL;
	}

	strcpy(data_buf, string);	

	dest->capacity = length;
	dest->written = length;
	dest->data = data_buf;

	return dest;;
}

char *cstr_from_strpp(StrPP *string) {
	char *dest = malloc(string->written);

	strcpy(dest, string->data);

	return dest;
}

size_t strpp_strlen(StrPP string) {
	return string.written;
}

size_t strpp_strcap(StrPP string) {
	return string.capacity;
}

StrPP *strpp_strcpy(StrPP *restrict dest, StrPP *restrict src) {
	size_t needed_size = src->written;
	if(dest->capacity < needed_size){
		char *tmp = realloc(dest->data, needed_size);
		if(!tmp) return NULL;

		dest->capacity = needed_size;
		dest->data = tmp;
		
		tmp = NULL;
	}

	memcpy(dest->data, src->data, src->written + 1);

	dest->written = src->written;

	return dest;
}

StrPP *strpp_strncpy(StrPP *restrict dest, StrPP *restrict src, size_t n) {
	if(dest->capacity < n){
		char *tmp = realloc(dest->data, n);
		if(!tmp) return NULL;

		dest->capacity = n;
		dest->data = tmp;

		tmp = NULL;
	}

	size_t i = 0;
	while(i < n){
		if(i > src->written){
			dest->data[i] = '\0';
		}else{
			dest->data[i] = src->data[i];
		}
		++dest->written;
	}

	return dest;
}

StrPP *strpp_strcat(StrPP *restrict dest, StrPP *restrict src) {
	size_t new_len = dest->written + src->written;

	if(dest->capacity < new_len) {
		char *tmp = realloc(dest->data, new_len);
		if(!tmp) return NULL;

		dest->capacity = new_len;
		dest-> data = tmp;

		tmp = NULL;
	}

	memcpy(dest->data + dest->written, src->data, src->written + 1);

	dest->written = new_len;

	return dest;
}

int strpp_strcmp(const StrPP *s1, const StrPP *s2) {
	if(s1->written != s2->written) return 1;

	for(size_t i = 0; i < s1->written; ++i){
		if(s1->data[i] != s2->data[i]){
			return 1;
		}
	}

	return 0;	
}

StrPP *strpp_strdup(const StrPP *s) {
	StrPP *duplicated = malloc(sizeof(StrPP));
	if(!duplicated) return NULL;

	duplicated->capacity = s->capacity;
	duplicated->written = s->written;

	size_t size = strpp_strlen(*s) + 1;
	duplicated->data = malloc(size);

	memcpy(duplicated->data, s->data, size);

	return duplicated;	
}
