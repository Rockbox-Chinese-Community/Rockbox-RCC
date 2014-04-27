#include "lcd.h"
#include "/home/ubuntu/Desktop/RockboxRCC/bu800/pluginbitmaps/invadrox_fire.h"
const unsigned char invadrox_fire[] = {
0x91, 0x24, 0x7e, 0x7f, 0xff, 0x7e, 0x22, 0x89, 

};

const struct bitmap bm_invadrox_fire = { 
    .width = BMPWIDTH_invadrox_fire, 
    .height = BMPHEIGHT_invadrox_fire, 
    .data = (unsigned char*)invadrox_fire,
};
