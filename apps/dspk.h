long log15(long x) {
    if(x>=0) {
        if(x>=0x40000000) return ((x&0x3fffffff)>>3)+0x78000000;//15
        if(x>=0x20000000) return ((x&0x1fffffff)>>2)+0x70000000;//14
        if(x>=0x10000000) return ((x&0xfffffff)>>1)+0x68000000; //13
        if(x>=0x8000000)  return (x&0x7ffffff)+0x60000000;      //12
        if(x>=0x4000000)  return ((x&0x3ffffff)<<1)+0x58000000; //11
        if(x>=0x2000000)  return ((x&0x1ffffff)<<2)+0x50000000; //10
        if(x>=0x1000000)  return ((x&0xffffff)<<3)+0x48000000;  //9
        if(x>=0x800000)   return ((x&0x7fffff)<<4)+0x40000000;  //8
        if(x>=0x400000)   return ((x&0x3fffff)<<5)+0x38000000;  //7
        if(x>=0x200000)   return ((x&0x1fffff)<<6)+0x30000000;  //6
        if(x>=0x100000)   return ((x&0xfffff)<<7)+0x28000000;   //5
        if(x>=0x80000)    return ((x&0x7ffff)<<8)+0x20000000;   //4
        if(x>=0x40000)    return ((x&0x3ffff)<<9)+0x18000000;   //3
        if(x>=0x20000)    return ((x&0x1ffff)<<10)+0x10000000;  //2
        if(x>=0x10000)    return ((x&0xffff)<<11)+0x8000000;    //1
        return x<<11;
    }
    else {
        x=~x+1;
        if(x>=0x40000000) return ~(((x&0x3fffffff)>>3)+0x78000000)+1;
        if(x>=0x20000000) return ~(((x&0x1fffffff)>>2)+0x70000000)+1;
        if(x>=0x10000000) return ~(((x&0xfffffff)>>1)+0x68000000)+1;
        if(x>=0x8000000)  return ~((x&0x7ffffff)+0x60000000)+1;
        if(x>=0x4000000)  return ~(((x&0x3ffffff)<<1)+0x58000000)+1;
        if(x>=0x2000000)  return ~(((x&0x1ffffff)<<2)+0x50000000)+1;
        if(x>=0x1000000)  return ~(((x&0xffffff)<<3)+0x48000000)+1;
        if(x>=0x800000)   return ~(((x&0x7fffff)<<4)+0x40000000)+1;
        if(x>=0x400000)   return ~(((x&0x3fffff)<<5)+0x38000000)+1;
        if(x>=0x200000)   return ~(((x&0x1fffff)<<6)+0x30000000)+1;
        if(x>=0x100000)   return ~(((x&0xfffff)<<7)+0x28000000)+1;
        if(x>=0x80000)    return ~(((x&0x7ffff)<<8)+0x20000000)+1;
        if(x>=0x40000)    return ~(((x&0x3ffff)<<9)+0x18000000)+1;
        if(x>=0x20000)    return ~(((x&0x1ffff)<<10)+0x10000000)+1;
        if(x>=0x10000)    return ~(((x&0xffff)<<11)+0x8000000)+1;
        return ~(x<<11)+1;
    }
}
