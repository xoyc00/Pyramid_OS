#include <stdlib.h>
#include <stdint.h>

#if defined(__is_libk)
#include <kernel/liballoc.h>
#endif

void* malloc(size_t size) {
#if defined(__is_libk)
	return kmalloc(size);
#else
	
#endif
}
