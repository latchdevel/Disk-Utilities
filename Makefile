ROOT := .
include $(ROOT)/Rules.mk

SUBDIRS := libdisk disk-analyse

all:
	@set -e; for subdir in $(SUBDIRS); do \
		$(MAKE) -C $$subdir all; \
	done

install:
	@set -e; for subdir in $(SUBDIRS); do \
		$(MAKE) -C $$subdir install; \
	done

clean::
	@set -e; for subdir in $(SUBDIRS); do \
		$(MAKE) -C $$subdir clean; \
	done
