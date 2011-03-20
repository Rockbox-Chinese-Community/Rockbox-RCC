#             __________               __   ___.
#   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
#   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
#   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
#   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
#                     \/            \/     \/    \/            \/
# $Id$
#

BOMBERMAN_SRCDIR := $(APPSDIR)/plugins/bomberman
BOMBERMAN_BUILDDIR := $(BUILDDIR)/apps/plugins/bomberman

BOMBERMAN_SRC := $(call preprocess, $(BOMBERMAN_SRCDIR)/SOURCES)
BOMBERMAN_OBJ := $(call c2obj, $(BOMBERMAN_SRC))

OTHER_SRC += $(BOMBERMAN_SRC)

ifndef SIMVER
ifneq (,$(strip $(foreach tgt,RECORDER ONDIO,$(findstring $(tgt),$(TARGET)))))
    ### lowmem targets
    ROCKS += $(BOMBERMAN_BUILDDIR)/bomberman.ovl
    BOMBERMAN_OUTLDS = $(BOMBERMAN_BUILDDIR)/bomberman.link
    BOMBERMAN_OVLFLAGS = -T$(BOMBERMAN_OUTLDS) -Wl,--gc-sections -Wl,-Map,$(basename $@).map
else
    ### all other targets
    ROCKS += $(BOMBERMAN_BUILDDIR)/bomberman.rock
endif
else
    ### simulator
    ROCKS += $(BOMBERMAN_BUILDDIR)/bomberman.rock
endif

$(BOMBERMAN_BUILDDIR)/bomberman.rock: $(BOMBERMAN_OBJ)

$(BOMBERMAN_BUILDDIR)/bomberman.refmap: $(BOMBERMAN_OBJ)

$(BOMBERMAN_OUTLDS): $(PLUGIN_LDS) $(BOMBERMAN_BUILDDIR)/bomberman.refmap
	$(call PRINTS,PP $(@F))$(call preprocess2file,$<,$@,-DOVERLAY_OFFSET=$(shell \
		$(TOOLSDIR)/ovl_offset.pl $(BOMBERMAN_BUILDDIR)/bomberman.refmap))

$(BOMBERMAN_BUILDDIR)/bomberman.ovl: $(BOMBERMAN_OBJ) $(BOMBERMAN_OUTLDS)
	$(SILENT)$(CC) $(PLUGINFLAGS) -o $(basename $@).elf \
		$(filter %.o, $^) \
		$(filter %.a, $+) \
		-lgcc $(BOMBERMAN_OVLFLAGS)
	$(call PRINTS,LD $(@F))$(OC) -O binary $(basename $@).elf $@
