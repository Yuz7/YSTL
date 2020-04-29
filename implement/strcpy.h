#include <assert.h>

void* strcpy(char* dst, const char* src){
	assert(dst && src);
	if (dst == nullptr || src == nullptr) return nullptr;
	char* sc = src;
	while (sc != '\0'){
		dst = sc;
		sc++;
		dst++;
	}
	dst = '\0';
	return dst;
}