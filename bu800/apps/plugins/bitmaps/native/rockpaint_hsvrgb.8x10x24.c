#include "lcd.h"
#include "/home/ubuntu/Desktop/RockboxRCC/bu800/pluginbitmaps/rockpaint_hsvrgb.h"
const unsigned short rockpaint_hsvrgb[] = {
0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000,
0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff, 0x0000, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff, 0x0000, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff, 0x0000, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 
0xf81f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0x0000, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff, 0x0000, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff, 0x0000, 
0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000,
0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 
0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 
0xf81f, 0xf81f, 0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0x0000,
0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0xf81f, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0x0000, 0xf81f, 0x0000, 0xffff, 0x0000, 
0xf81f, 0xf81f, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000,
0xffff, 0xffff, 0x0000, 0x0000, 0x0000, 0xf81f, 
0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0x0000,
0x0000, 0x0000, 0xffff, 0xffff, 0x0000, 0xf81f, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0xf81f, 0x0000, 0x0000, 0xffff, 0x0000, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 
0xf81f, 0xf81f, 0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0x0000,
0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0xf81f, 
0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 
0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000,
0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff, 0x0000, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff, 0x0000, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff, 0x0000, 
0xf81f, 0xf81f, 0x0000, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0x0000,
0xffff, 0x0000, 0x0000, 0xffff, 0x0000, 0xf81f, 
0xf81f, 0xf81f, 0x0000, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0x0000,
0xffff, 0x0000, 0x0000, 0xffff, 0x0000, 0xf81f, 
0xf81f, 0xf81f, 0x0000, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0x0000,
0xffff, 0x0000, 0x0000, 0xffff, 0x0000, 0xf81f, 
0xf81f, 0xf81f, 0xf81f, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
0x0000, 0xffff, 0xffff, 0x0000, 0xf81f, 0xf81f, 
0xf81f, 0xf81f, 0xf81f, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
0x0000, 0xffff, 0xffff, 0x0000, 0xf81f, 0xf81f, 
0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
0xf81f, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f, 
0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 
0xf81f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0xf81f, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff, 0x0000, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 
0xf81f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0xf81f, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xffff,
0x0000, 0x0000, 0xffff, 0x0000, 0xf81f, 0xf81f, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff,
0x0000, 0xf81f, 0x0000, 0xffff, 0x0000, 0xf81f, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff, 0x0000, 
0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000,
0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 
0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 
0xf81f, 0xf81f, 0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0x0000,
0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0xf81f, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xffff,
0x0000, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xffff,
0x0000, 0xf81f, 0x0000, 0x0000, 0x0000, 0xf81f, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0x0000, 0x0000, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0x0000, 0xffff, 0xffff, 0xffff, 0x0000, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0xf81f, 0x0000, 0x0000, 0xffff, 0x0000, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 
0xf81f, 0xf81f, 0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0x0000,
0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0xf81f, 
0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 
0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000,
0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f, 
0xf81f, 0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xffff,
0xffff, 0xffff, 0xffff, 0x0000, 0xf81f, 0xf81f, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff,
0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 0xf81f, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff,
0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 0xf81f, 
0xf81f, 0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xffff,
0xffff, 0xffff, 0xffff, 0x0000, 0xf81f, 0xf81f, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff,
0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 0xf81f, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff, 0x0000, 
0xf81f, 0x0000, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000, 0xf81f, 0x0000, 0xffff,
0x0000, 0x0000, 0x0000, 0x0000, 0xffff, 0x0000, 
0xf81f, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 0x0000, 0xffff,
0xffff, 0xffff, 0xffff, 0xffff, 0x0000, 0xf81f, 
0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x0000,
0x0000, 0x0000, 0x0000, 0x0000, 0xf81f, 0xf81f, 

};

const struct bitmap bm_rockpaint_hsvrgb = { 
    .width = BMPWIDTH_rockpaint_hsvrgb, 
    .height = BMPHEIGHT_rockpaint_hsvrgb, 
    .format = FORMAT_NATIVE, 
    .data = (unsigned char*)rockpaint_hsvrgb,
};
