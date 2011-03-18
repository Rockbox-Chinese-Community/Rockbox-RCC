#ifndef MWDB_H
#define MWDB_H
#include <inttypes.h>


int mwdb_findarticle(const char* filename,
                     char* artnme,
                     uint16_t artnmelen,
                     uint32_t *res_lo,
                     uint32_t *res_hi,
                     bool progress,
                     bool promptname,
                     bool casesense
                   );

int mwdb_loadarticle(const char * filename,
                     void * scratchmem,
                     void * articlemem,
                     uint32_t articlememlen,
                     uint32_t *articlelen,
                     uint32_t res_lo,
                     uint32_t res_hi);

#endif
