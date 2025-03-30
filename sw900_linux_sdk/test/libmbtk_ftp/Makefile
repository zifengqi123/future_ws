BUILD_ROOT = $(shell pwd)/../..
include $(BUILD_ROOT)/Make.defines

INC_DIR +=

LIB_DIR +=

LIBS += -lmbtk_lib -lmbtk_net -lmbtk_ftp

CFLAGS += 

DEFINE +=

LOCAL_SRC_FILES = $(wildcard *.c) $(wildcard *.cpp)

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
