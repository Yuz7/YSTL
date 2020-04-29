#include <assert.h>
void* memcpy(void*dest, const void*src, size_t count){
	assert(dest && src && count > 0);
	char* dst = (char *)dest;
	char* sc = (char *)src;

	if (dst <= sc || dst >= sc + count){
		while (count--){
			dst = sc;
			dst++;
			sc++;
		}
	}
	else{
		sc = sc + count - 1;
		dst = dst + count - 1;
		while (count--){
			dst = sc;
			dst--;
			sc--;
		}
	}
	return dst;
}