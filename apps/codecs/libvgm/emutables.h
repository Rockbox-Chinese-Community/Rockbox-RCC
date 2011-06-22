#ifndef _EMUTABLES_H_
#define _EMUTABLES_H_

/* Precompiled emu2413 tables for use in Rockbox, 
   Calculated for 44Khz sampling rate */

#include "emutypes.h"

static const e_uint16 sin_coeff[] = {
	255,   203,   171,   152,   139,   129,   120, 
	113,   107,   102,    97,    92,    88,    85, 
	 81,    78,    75,    72,    70,    67,    65, 
	 63,    61,    59,    57,    55,    53,    52, 
	 50,    48,    47,    45,    44,    43,    41, 
	 40,    39,    38,    37,    35,    34,    33, 
	 32,    31,    30,    29,    28,    28,    27, 
	 26,    25,    24,    23,    23,    22,    21, 
	 21,    20,    19,    19,    18,    17,    17, 
	 16,    16,    15,    14,    14,    13,    13, 
	 12,    12,    11,    11,    11,    10,    10, 
	  9,     9,     8,     8,     8,     7,     7, 
	  7,     6,     6,     6,     5,     5,     5, 
	  4,     4,     4,     4,     3,     3,     3, 
	  3,     2,     2,     2,     2,     2,     2, 
	  1,     1,     1,     1,     1,     1,     1, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0, 
};

static const e_int32 pm_coeff[] = {
	256,   256,   256,   256,   256,   256,   256, 
	256,   256,   256,   256,   256,   256,   256, 
	256,   256,   256,   256,   256,   256,   256, 
	256,   256,   256,   256,   256,   256,   256, 
	256,   256,   256,   256,   257,   257,   257, 
	257,   257,   257,   257,   257,   257,   257, 
	257,   257,   257,   257,   257,   257,   257, 
	257,   257,   257,   257,   257,   257,   257, 
	257,   257,   257,   257,   257,   257,   257, 
	258,   258,   258,   257,   257,   257,   257, 
	257,   257,   257,   257,   257,   257,   257, 
	257,   257,   257,   257,   257,   257,   257, 
	257,   257,   257,   257,   257,   257,   257, 
	257,   257,   257,   257,   257,   257,   256, 
	256,   256,   256,   256,   256,   256,   256, 
	256,   256,   256,   256,   256,   256,   256, 
	256,   256,   256,   256,   256,   256,   256, 
	256,   256,   256,   256,   256,   256,   256, 
	256,   256,   256,   255,   255,   255,   255, 
	255,   255,   255,   255,   255,   255,   255, 
	255,   255,   255,   255,   255,   255,   255, 
	255,   255,   255,   255,   255,   255,   255, 
	255,   255,   255,   255,   255,   255,   254, 
	254,   254,   254,   254,   254,   254,   254, 
	254,   254,   254,   254,   254,   254,   254, 
	254,   254,   254,   254,   254,   254,   254, 
	254,   254,   254,   254,   254,   254,   254, 
	254,   254,   254,   253,   254,   254,   254, 
	254,   254,   254,   254,   254,   254,   254, 
	254,   254,   254,   254,   254,   254,   254, 
	254,   254,   254,   254,   254,   254,   254, 
	254,   254,   254,   254,   254,   254,   254, 
	254,   255,   255,   255,   255,   255,   255, 
	255,   255,   255,   255,   255,   255,   255, 
	255,   255,   255,   255,   255,   255,   255, 
	255,   255,   255,   255,   255,   255,   255, 
	255,   255,   255,   255, 
};

static const e_int16 db2lin_coeff[] = {
	255,   249,   244,   239,   233,   228,   224, 
	219,   214,   209,   205,   201,   196,   192, 
	188,   184,   180,   176,   172,   169,   165, 
	162,   158,   155,   151,   148,   145,   142, 
	139,   136,   133,   130,   127,   125,   122, 
	119,   117,   114,   112,   109,   107,   105, 
	102,   100,    98,    96,    94,    92,    90, 
	 88,    86,    84,    82,    81,    79,    77, 
	 76,    74,    72,    71,    69,    68,    66, 
	 65,    64,    62,    61,    60,    58,    57, 
	 56,    55,    53,    52,    51,    50,    49, 
	 48,    47,    46,    45,    44,    43,    42, 
	 41,    40,    39,    38,    38,    37,    36, 
	 35,    34,    34,    33,    32,    32,    31, 
	 30,    30,    29,    28,    28,    27,    27, 
	 26,    25,    25,    24,    24,    23,    23, 
	 22,    22,    21,    21,    20,    20,    19, 
	 19,    19,    18,    18,    17,    17,    17, 
	 16,    16,    16,    15,    15,    15,    14, 
	 14,    14,    13,    13,    13,    12,    12, 
	 12,    12,    11,    11,    11,    11,    10, 
	 10,    10,    10,    10,     9,     9,     9, 
	  9,     8,     8,     8,     8,     8,     8, 
	  7,     7,     7,     7,     7,     7,     6, 
	  6,     6,     6,     6,     6,     6,     5, 
	  5,     5,     5,     5,     5,     5,     5, 
	  5,     4,     4,     4,     4,     4,     4, 
	  4,     4,     4,     4,     3,     3,     3, 
	  3,     3,     3,     3,     3,     3,     3, 
	  3,     3,     3,     2,     2,     2,     2, 
	  2,     2,     2,     2,     2,     2,     2, 
	  2,     2,     2,     2,     2,     2,     2, 
	  2,     1,     1,     1,     1,     1,     1, 
	  1,     1,     1,     1,     1,     1,     1, 
	  1,     1,     1,     1,     1,     1,     1, 
	  1,     1,     1,     1,     1,     1,     1, 
	  1,     1,     1,     1,     1,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0,     0,     0,     0,     0,     0,     0, 
	  0, 
};

static const e_uint16 ar_adjust_coeff[] = {
	127,  108,   98,   90,   84,   80,   75, 
	 72,   69,   66,   64,   61,   59,   57, 
	 56,   54,   52,   51,   49,   48,   47, 
	 45,   44,   43,   42,   41,   40,   39, 
	 38,   37,   36,   36,   35,   34,   33, 
	 33,   32,   31,   30,   30,   29,   29, 
	 28,   27,   27,   26,   26,   25,   24, 
	 24,   23,   23,   22,   22,   21,   21, 
	 21,   20,   20,   19,   19,   18,   18, 
	 17,   17,   17,   16,   16,   15,   15, 
	 15,   14,   14,   14,   13,   13,   13, 
	 12,   12,   12,   11,   11,   11,   10, 
	 10,   10,    9,    9,    9,    9,    8, 
	  8,    8,    7,    7,    7,    7,    6, 
	  6,    6,    6,    5,    5,    5,    4, 
	  4,    4,    4,    4,    3,    3,    3, 
	  3,    2,    2,    2,    2,    1,    1, 
	  1,    1,    1,    0,    0,    0,    0, 
	  0, 
};

#endif
