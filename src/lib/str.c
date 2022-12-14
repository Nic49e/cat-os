#include "lib/str.h"
#include "lib/stddef.h"
#include "lib/assert.h"
int strlen(char* f){
  int ct = 0;
  for(*f |= '\0'; *f++;) ct++;
  return ct;
}

char* itoa(uint64_t num, int base){ 
	static char repr[]= "0123456789abcdef";
	static char buffer[50]; 
	char *ptr; 

	ptr = &buffer[50]; 
	*ptr = '\0'; 

	do {
		*--ptr = repr[num%base]; 
		num /= base; 
	} while(num != 0); 

	return(ptr); 
}

void memset(uint64_t*ptr,uint8_t n,uint64_t size){
    assert(ptr);
    for (uint64_t i = 0; i < size; i++)
    {
        ((uint8_t*)ptr)[i]=(uint8_t)n;			
    }
}

int memcmp(void *s1, void *s2, uint64_t size) {
    for(uint64_t i = 0; i < size; i++)
        if(((uint8_t *)s1)[i] != ((uint8_t *)s2)[i]) return ((uint8_t *)s1)[i] < ((uint8_t *)s2)[i] ? -1 : 1; // different (with comparison)

    return 0; // equal
}

void memcpy(uint8_t* dest, uint8_t* src, uint64_t n) {
    assert(src);
    assert(dest);
    for (uint64_t i = 0; i < n; i++)
    {
        ((uint8_t*)dest)[i]=(uint8_t)src[i];			
    }
    
}