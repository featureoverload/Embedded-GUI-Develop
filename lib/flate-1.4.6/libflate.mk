#############################################################
#
#libflate
#
#############################################################

LIBFLATE_CONFIGURE_DIR:=$(shell pwd)/package/libflate
LIBFLATE_LIB_DIR:=$(STAGING_DIR)/usr/lib
LIBFLATE_INC_DIR:=$(STAGING_DIR)/include
LIB_DIR:=$(TARGET_DIR)/lib

#############################################################
#
# Build recipes
#
#############################################################

EXTRA_WARNINGS = -Wall -Wshadow -Werror

# define _GNU_SOURCE to get prototypes for strcasecmp and strncasecmp
# Programs are present in the C library; this #define just insures their prototype is defined.
# EXTRA_DEFINES = -D_GNU_SOURCE
STRIP = $(TARGET_CROSS)strip

libflate:
	make -C $(LIBFLATE_CONFIGURE_DIR) TARGET="$(TARGET_DIR)" BUILD="$(BUILD_DIR)" \
		CC=$(TARGET_CC) LD=$(TARGET_CROSS)ld STRIP=$(STRIP) $(EXTRA_WARNINGS) all;
	(cd $(LIBFLATE_CONFIGURE_DIR); \
		cp -fa $(LIBFLATE_CONFIGURE_DIR)/libflate.so $(LIBFLATE_LIB_DIR); \
		cp -fa $(LIBFLATE_CONFIGURE_DIR)/libflate.so $(LIB_DIR); \
		cp -fa $(LIBFLATE_CONFIGURE_DIR)/flate.h $(LIBFLATE_INC_DIR); \
	);

libflate-clean: 
	-$(MAKE) -C $(LIBFLATE_CONFIGURE_DIR) clean
	rm -rf $(LIBFLATE_CONFIGURE_DIR)/*.o

libflate-dirclean: 


#############################################################
#
# Toplevel Makefile options
#
#############################################################
ifeq ($(strip $(BR2_PACKAGE_LIBFLATE)),y)
TARGETS+=libflate
endif
	
