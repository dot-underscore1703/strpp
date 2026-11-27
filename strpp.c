#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Null strings are good to have
#define NULL_STRPP (StrPP){.written = 0, .stack_data = (char*){'\0'}};
#define NULL_STRSLICE (StringSlice){NULL, 0}

// These control the metadata flags in the StrPP strings, stored in the length field.
#define STRPP_HEAP_FLAG 0x80
#define STRPP_LENGTH_MASK 0x3F
#define STRPP_IS_HEAP(s) (((s).stack.length & STRPP_HEAP_FLAG) != 0)
#define STRPP_GET_LENGTH(s) ((s).stack.length & STRPP_LENGTH_MASK)
#define STRPP_ENCODE_LENGTH(length, heap, mutable) (((length) & STRPP_LENGTH_MASK) | ((heap) ? STRPP_HEAP_FLAG : 0) | ((mutable) ? STRPP_MUTABLE_FLAG : 0))

// We use this to control the mutability of the string.
// Basically we can lock and unlock the string, swapping it between read-only and read/write.
#define STRPP_MUTABLE_FLAG 0x40
#define STRPP_IS_MUTABLE(s) (((s).stack.length & STRPP_MUTABLE_FLAG) != 0)
#define STRPP_SET_FLAG(s,flag) ((s).stack.length |= (flag))
#define STRPP_UNSET_FLAG(s,flag) ((s).stack.length &= ~(flag))
	
// For functions that return StringSuccess, its slightly easier to read than what its expanding to.
#define STRPP_STRINGSUCCESS(s) (StringSuccess){true, .string = s}
#define STRPP_SLICESUCCESS(s) (StringSuccess){true, .slice = s}
#define STRPP_FAILURE (StringSuccess){false}

/// Unwrap StringSuccess into a string or slice value. The user must check if the StringSuccess did indeed, succeed.
#define STRPP_UNWRAP(s, type) _Generic((type){0}, StrPP: (s).string, StringSlice: (s).slice)

#define STRPP_SIZE (sizeof(StrPP))

//	======================
//	-- TYPE DEFINITIONS --
//	======================

typedef struct {
	union {
		struct {
			char *data;
			size_t length;	// Also contains metadata for mutability, heap/stack state.
			size_t capacity;
		} heap;
		struct {
			char data[23];
			uint8_t length;	// Also contains metadata for mutability, heap/stack state.
		} stack;
	};
} StrPP;

typedef struct {
	const char *start;
	size_t length;
} StringSlice;


typedef struct {
	bool success;
	union {
		StrPP string;
		StringSlice slice;
	};
} StringSuccess;

//	======================
//	-- TYPE CONVERSIONS --
//	======================

/// Create a string slice from char*
StringSuccess strslice_from_cstr(char *string, size_t index, size_t length) {
	StringSlice new_slice = {0};

	new_slice.start = string + index;
	new_slice.length = length;

	return STRPP_SLICESUCCESS(new_slice);
}

/// Create a string slice from StrPP
StringSuccess strslice_from_strpp(StrPP string, size_t index, size_t length) {
	StringSlice new_slice = {0};

	if(STRPP_IS_HEAP(string)) {
		if(length > string.heap.length){
			return STRPP_FAILURE;
		}else {
			new_slice.length = length;
		}
		new_slice.start = string.heap.data + index;
	}else {
		if(length > string.stack.length) 
			return STRPP_FAILURE; 
		else 
			new_slice.length = length;
		
		new_slice.start = string.stack.data + index;
	}

	return STRPP_SLICESUCCESS(new_slice);
}

/// Create new string from string slice.
char* cstr_from_strslice(StringSlice string) {
	char *new_str = malloc(string.length + 1);
	if(!new_str) 
		return NULL;
	
	memcpy(new_str, string.start, string.length);

	new_str[string.length] = '\0';

	return new_str;
}

/// Create new StrPP from string slice.
StringSuccess strpp_from_strslice(StringSlice string) {
	StrPP new_str = {0};

	if(string.length >= sizeof(new_str.stack.data)){
		new_str.heap.data = malloc(string.length + 1);
		if(!new_str.heap.data) 
			return STRPP_FAILURE;

		memcpy(new_str.heap.data, string.start, string.length);
		new_str.heap.data[string.length] = '\0';

		new_str.heap.length = STRPP_ENCODE_LENGTH(string.length, false, false);
		new_str.heap.capacity = string.length;
	}else {
		memcpy(new_str.stack.data, string.start, string.length);
		new_str.stack.data[string.length] = '\0';

		new_str.stack.length = STRPP_ENCODE_LENGTH(string.length, false, false);
	}

	return STRPP_STRINGSUCCESS(new_str);
}

/// Convert char* to StrPP
StringSuccess strpp_from_cstr(char *string) {
	StrPP dest = {0};

	size_t length;

	for(length = 0; string[length] != '\0'; ++length);

	if(length >= sizeof(dest.stack.data)) {
		// We put this thing on the heap
		dest.heap.data = malloc(length + 1); // +1 for terminator
		if(!dest.heap.data) return STRPP_FAILURE;

		memcpy(dest.heap.data, string, length);
		dest.heap.data[length] = '\0';

		dest.heap.length = STRPP_ENCODE_LENGTH(length, false,false);
		dest.heap.capacity = length;
		
	}else {
		// We put it on the stack.
		memcpy(dest.stack.data, string, length);
		dest.stack.data[length] = '\0';

		dest.stack.length = STRPP_ENCODE_LENGTH(length,false,false);
	}
	
	return STRPP_STRINGSUCCESS(dest);
}

// Convert StrPP to char*. The programmer must manually free the string.
char *cstr_from_strpp(StrPP string) {
	size_t length = STRPP_GET_LENGTH(string) + 1;
	char *dest = malloc(length);
	if(!dest) return NULL;

	if(STRPP_IS_HEAP(string)){
		memcpy(dest, string.heap.data, length);
	}else {
		memcpy(dest, string.stack.data, length);
	}

	return dest;
}

//	======================
//	-- STRING UTILITIES --
//	======================

// These functions should closely mimic the libc string.h implementation. (other than the custom funcs defined here)

/// Set the mutability of the string, switch it from read-only and read/write
void strpp_set_mutability(StrPP *string, bool mutability) {
	if(mutability)
		STRPP_SET_FLAG(*string, STRPP_MUTABLE_FLAG);
	else
		STRPP_UNSET_FLAG(*string, STRPP_MUTABLE_FLAG);

	return;
}

size_t strpp_strlen(StrPP string) {
	return STRPP_GET_LENGTH(string);
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
