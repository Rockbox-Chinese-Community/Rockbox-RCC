#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include "utf8_aux.h"

#define ROCKBOX_LITTLE_ENDIAN
#define KEY_MAXLEN 200

static inline unsigned short swap16(unsigned short value)
    /*
      result[15..8] = value[ 7..0];
      result[ 7..0] = value[15..8];
    */
{
    return (value >> 8) | (value << 8);
}

static inline unsigned long swap32(unsigned long value)
    /*
      result[31..24] = value[ 7.. 0];
      result[23..16] = value[15.. 8];
      result[15.. 8] = value[23..16];
      result[ 7.. 0] = value[31..24];
    */
{
    unsigned long hi = swap16(value >> 16);
    unsigned long lo = swap16(value & 0xffff);
    return (lo << 16) | hi;
}

#ifdef ROCKBOX_LITTLE_ENDIAN
#define letoh16(x) (x)
#define letoh32(x) (x)
#else
#define letoh16(x) swap16(x)
#define letoh32(x) swap32(x)
#endif

static void search_btree(FILE *file,const char* key,uint16_t rkeylen, long globoffs, uint32_t* res_lo,uint32_t* res_hi);

#ifndef NO_BTS_MAIN 
int main(int argc,char * argv[]){
    if(argc<3){
        printf("Usage: btsearch <file> <key>\n");
    }
    else
    {
        FILE *fd=fopen(argv[1],"r");
        uint32_t res_lo;
        uint32_t res_hi;
        fseek(fd,0,SEEK_SET);
        //printf("SL:%d\n",strlen(argv[2]));
        search_btree(fd,argv[2],strlen(argv[2]),0,&res_lo,&res_hi);
        printf("Result:%d,%d\n",res_lo,res_hi);
    }

    return 0;
}
#endif

static void search_btree(FILE *file,const char* key,uint16_t rkeylen, long globoffs, uint32_t* res_lo,uint32_t* res_hi) {
    unsigned char nd_key[KEY_MAXLEN];
    uint8_t node_flags;
    uint16_t node_nr_active,i,keylen;
    uint32_t chldptr;
    uint64_t dtaptr_lo,dtaptr_hi;
    fread(&node_flags,sizeof(uint8_t),1,file);
    fread(&node_nr_active,sizeof(uint16_t),1,file);
    node_nr_active=letoh16(node_nr_active);
    if(node_nr_active<1){//error
        *res_lo=*res_hi=0;
        return;
    }
    for(i=0;i<node_nr_active;i++){
        fread(&dtaptr_lo,sizeof(uint32_t),1,file);
        dtaptr_lo=letoh32(dtaptr_lo);
        fread(&dtaptr_hi,sizeof(uint32_t),1,file);
        dtaptr_hi=letoh32(dtaptr_hi);
        fread(&chldptr,sizeof(uint32_t),1,file);
        chldptr=letoh32(chldptr);
        fread(&keylen,sizeof(uint16_t),1,file);
        keylen=letoh32(keylen);
        fread(&nd_key,sizeof(unsigned char),(keylen<KEY_MAXLEN)?keylen:KEY_MAXLEN,file);
        if(keylen-((keylen<KEY_MAXLEN)?keylen:KEY_MAXLEN)>0)
            fseek(file,keylen-((keylen<KEY_MAXLEN)?keylen:KEY_MAXLEN),SEEK_CUR);
        keylen=(keylen<KEY_MAXLEN)?keylen:KEY_MAXLEN;

        nd_key[keylen]=0;
        printf("CMP%s,%s\n",((const unsigned char*)key),((const unsigned char*)nd_key));
        if(utf8strcnmp(((const unsigned char*)key),((const unsigned char*)nd_key),rkeylen,keylen)>0){
            continue;
        }
        if(utf8strcnmp(((const unsigned char*)key),((const unsigned char*)nd_key),rkeylen,keylen)==0) {
            *res_lo=dtaptr_lo;
            *res_hi=dtaptr_hi;
            return;
        }
        if(chldptr==0||node_flags==1){
            *res_lo=*res_hi=0;
            return;
            }
        fseek(file,globoffs+chldptr,SEEK_SET);
        search_btree(file,key,rkeylen,globoffs,res_lo,res_hi);
        return;
    }
    if(node_flags!=1){//node not leaf
        fread(&chldptr,sizeof(uint32_t),1,file);
        chldptr=letoh32(chldptr);
        if(chldptr==0){//leaf
            *res_lo=*res_hi=0;
            return;
        }
        fseek(file,globoffs+chldptr,SEEK_SET);
        search_btree(file,key,rkeylen,globoffs,res_lo,res_hi);
        return;
    }
    *res_lo=*res_hi=0;
    return;
}
