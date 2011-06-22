
# libgbs
VGMLIB := $(CODECDIR)/libvgm.a
VGMLIB_SRC := $(call preprocess, $(APPSDIR)/codecs/libvgm/SOURCES)
VGMLIB_OBJ := $(call c2obj, $(VGMLIB_SRC))
OTHER_SRC += $(VGMLIB_SRC)

$(VGMLIB): $(VGMLIB_OBJ)
	$(SILENT)$(shell rm -f $@)
	$(call PRINTS,AR $(@F))$(AR) rcs $@ $^ >/dev/null

VGMFLAGS = $(filter-out -O%,$(CODECFLAGS)) -fno-strict-aliasing
ifeq ($(CPU),arm)
   VGMFLAGS += -O3
else
   VGMFLAGS += -O2
endif

$(CODECDIR)/libvgm/%.o: $(ROOTDIR)/apps/codecs/libvgm/%.c
	$(SILENT)mkdir -p $(dir $@)
	$(call PRINTS,CC $(subst $(ROOTDIR)/,,$<))$(CC) $(VGMFLAGS) -c $< -o $@
