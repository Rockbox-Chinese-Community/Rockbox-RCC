// Library configuration. Modify this file as necessary.

#ifndef BLARGG_CONFIG_H
#define BLARGG_CONFIG_H

// Uncomment to enable platform-specific optimizations
//#define BLARGG_NONPORTABLE 1

// Uncomment if automatic byte-order determination doesn't work
#ifdef ROCKBOX_BIG_ENDIAN
	#define BLARGG_BIG_ENDIAN 1
#endif

// Uncomment if you get errors in the bool section of blargg_common.h
#define BLARGG_COMPILER_HAS_BOOL 1

// To handle undefined reference to assert
#define NDEBUG 1

// Use standard config.h if present
#define HAVE_CONFIG_H 1

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#endif
