#             __________               __   ___.
#   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
#   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
#   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
#   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
#                     \/            \/     \/    \/            \/
# $Id: $
#

CC = $(PREFIX)gcc
CFLAGS=-Wall -W -g

ifeq ($(findstring CYGWIN,$(shell uname)),CYGWIN)
EXT=.exe
CFLAGS+=-mno-cygwin
else ifeq ($(findstring mingw32msvc,$(PREFIX)),mingw32msvc)
EXT=.exe
else
EXT=
endif

all: xmlconv$(EXT) btcreate$(EXT) btsearch$(EXT)

utf8_aux.o: utf8_aux.h
	$(CC) $(CFLAGS) -c -o $@ utf8_aux.c

btcreate$(EXT): btcreate.c btree.c btree.h utf8_aux.o
	$(CC) $(CFLAGS) -o $@ btcreate.c btree.c utf8_aux.o

btsearch$(EXT): btsearch.c utf8_aux.o
	$(CC) $(CFLAGS) -o $@ btsearch.c utf8_aux.o

xmlconv$(EXT): xmlconv.c xmlconv.h xmlentities.h
	$(CC) $(CFLAGS) -o $@ -O3 xmlconv.c -lpcre -lz -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE

clean:
	rm -f btcreate$(EXT) btsearch$(EXT) xmlconv$(EXT) utf8_aux.o
