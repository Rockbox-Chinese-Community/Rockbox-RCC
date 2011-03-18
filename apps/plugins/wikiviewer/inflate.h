#ifndef INFLATE_H
#define INFLATE_H
#include <inttypes.h>
#include "plugin.h"

uint32_t decompress(const char *inflenme,char* outbuffer,uint32_t outbuflen,
                    uint32_t offset,char* membuf);
#endif
