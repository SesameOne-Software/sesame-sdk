#ifndef SDK_UTLVEC_H
#define SDK_UTLVEC_H

typedef struct {
	void* ptr;
	int allocation_count;
	int grow_size;
} utlmemory;

typedef struct {
	utlmemory memory;
	int size;
	void* elements_ptr;
} utlvector;

#endif SDK_UTLVEC_H