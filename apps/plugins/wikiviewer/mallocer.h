#define MEMPOOL_MAX 10
#include <inttypes.h>
#include "plugin.h"

int wpw_init_mempool(const struct plugin_api* rb,unsigned char mempool);
int wpw_init_mempool_pdm(const struct plugin_api* rb,unsigned char mempool,
                         unsigned char* mem,long memsize);

void wpw_reset_mempool(unsigned char mempool);
void wpw_destroy_mempool(unsigned char mempool);
void* wpw_malloc(unsigned char mempool,size_t size);
void* wpw_calloc(unsigned char mempool,size_t nmemb, size_t size);
void codec_free(unsigned char mempool,void* ptr);
void* wpw_realloc(unsigned char mempool,void* ptr, size_t size);
long wpw_available(unsigned char mempool);
