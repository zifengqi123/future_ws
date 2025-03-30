BUILD_ROOT = $(shell pwd)/../..
include $(BUILD_ROOT)/Make.defines

INC_DIR +=

LIB_DIR +=

LIBS += -lmbtk_lib -lmbtk_ril

CFLAGS +=

DEFINE +=

#LOCAL_SRC_FILES = $(wildcard *.c) $(wildcard *.cpp)
ifeq ($(MBTK_SOURCE_VERSION), 2)
LOCAL_SRC_FILES = mbtk_ril_test.c
else
LOCAL_SRC_FILES = mbtk_info_test.c
endif

$(info LOCAL_SRC_FILES = $(LOCAL_SRC_FILES))

OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(LOCAL_SRC_FILES)))
BINS = $(patsubst %.o,%,$(OBJS))

all: $(BINS)

$(BINS):$(OBJS)
	@echo "  BIN     $@"
	$(CC) $(CFLAGS) $(LIB_DIR) $(LIBS) $@.o -o $(OUT_DIR)/bin/$@

%.o:%.c
	$(CC) $(CFLAGS) $(INC_DIR) $(DEFINE) -c $< -o $@

%.o:%.cpp
	$(CC) $(CFLAGS) $(INC_DIR) $(DEFINE) -c $< -o $@

clean:
	rm -f $(OBJS)
