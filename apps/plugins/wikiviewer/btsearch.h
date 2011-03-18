#ifndef BTSEARCH_H
#define BTSEARCH_H
#include <inttypes.h>

void search_btree(int fp,const char* key,uint16_t rkeylen, uint32_t globoffs,
                  uint32_t* res_lo,uint32_t* res_hi,const bool casesense);
#endif
