BUILD_ROOT = $(shell pwd)
include Make.defines

DIRS = test

all:
	@echo $(ROOT)
	@echo $(CC)
	@echo $(OUT_DIR)
	@rm -rf $(OUT_DIR)
	@ [ ! -d out/bin ] && mkdir -p out/bin
	@ [ ! -d out/lib ] && mkdir -p out/lib
	@for i in $(DIRS); do \
		(cd $$i && echo "making $$i" && $(MAKE) ) || exit 1; \
	done
	@echo "Success"

clean:
	@for i in $(DIRS); do \
		(cd $$i && echo "Cleaning $$i" && $(MAKE) clean) || exit 1; \
	done
	rm -rf $(OUT_DIR)