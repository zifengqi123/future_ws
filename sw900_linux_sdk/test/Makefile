BUILD_ROOT = $(shell pwd)/..
include $(BUILD_ROOT)/Make.defines

#exclude_dirs := include bin
#test_dirs := $(shell find . -maxdepth 1 -type d)
#test_dirs := $(basename $(patsubst ./%,%,$(test_dirs)))
#test_dirs := $(filter-out $(exclude_dirs),$(test_dirs))

SUBDIRS := $(wildcard */)
SUBDIRS_WITH_MAKEFILES := $(foreach dir,$(SUBDIRS),$(if $(wildcard $(dir)Makefile),$(dir),))
 
all:
	@echo DIR=$(SUBDIRS_WITH_MAKEFILES)
	@for dir in $(SUBDIRS_WITH_MAKEFILES); do \
		echo "Building $$dir"; \
		$(MAKE) -C $$dir || exit 1; \
	done

clean:
	@echo DIR=$(SUBDIRS_WITH_MAKEFILES)
	@for i in $(SUBDIRS_WITH_MAKEFILES); do \
		(cd $$i && echo "Cleaning $$i" && $(MAKE) clean) || exit 1; \
	done
	rm -rf $(OUT_DIR)
