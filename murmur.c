#include <stdio.h>
#include <limits.h>
#include <stdint.h>

int murmur32(const char *key, uint32_t len, uint32_t seed) {

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;
    const uint32_t r1 = 15;
    const uint32_t r2 = 13;
    const uint32_t m  = 5;
    const uint32_t n  = 0xe6546b64;
    const int chunks = len / 4;
   
    uint32_t hash = (uint32_t) seed;
    const uint32_t *blocks = (const uint32_t *) key;

    //Process the chunks
    for(int i = 0; i < chunks; i++) {
        uint32_t k = blocks[i];
        k *= c1;
        k  = (k << r1) | (k >> (32 - r1));
        k *= c2;

        hash ^= k;
        hash  = ((hash << r2) | (hash >> (32 - r2))) * m + n; 
    }
 
    //Leftover key bytes. 
    const uint8_t *tail = (const uint8_t *) (key + chunks * 4);
    int tail_len = len - (4 * chunks);
    uint32_t t = 0;
    switch (tail_len) {
    case 3:
        t = t ^ tail[2] << 16; 
    case 2:
        t = t ^ tail[1] <<  8;
    case 1:
        t = t ^ tail[0]; 

        t *= c1;
        t  = (t << r1) | (t >> (32 - r1));
        t *= c2;
        hash ^= t;
    }
 
    //Final phase.
    hash ^= len;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);
    
    return (hash & UINT_MAX);
}

int main(void) {
    char *key = "TESTING";
    int result = murmur32(key, 7, 0);
    printf("Result: %x\n", result);
    return result;
}
