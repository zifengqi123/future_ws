#
# Makefile
#
BUILD_TOOLCHAIN_DIR = "/home/pos/pos_ws/LYNQ_SDK_ASR_L508_X6_ZY-001_V04.01.02.01/mbtk/toolchain/linux64/"

# CC ?= gcc
CC=$(BUILD_TOOLCHAIN_DIR)/bin/arm-openwrt-linux-gcc
CXX=$(BUILD_TOOLCHAIN_DIR)/bin/arm-openwrt-linux-g++

TARGET = libfuture_devices.so

INC_DIR += -I./include
SRC_DIR = ./src

SERIAL_DIR = ./src/serial
BIN_DIR = ./bin

LIB_DIR += -L$(BIN_DIR)

LIBS += -lpthread

CFLAGS += -g -Wall -fPIC -shared -fexceptions 

LOCAL_SRC_FILES = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SERIAL_DIR)/*.cpp)

$(info LOCAL_SRC_FILES = $(LOCAL_SRC_FILES))

OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(LOCAL_SRC_FILES)))

all: $(TARGET) future_test

$(TARGET): $(OBJS)
	rm -f $(BIN_DIR)/$(TARGET) $(BIN_DIR)/future_test
	@echo "  Building shared library $(TARGET)..."
	$(CXX) -shared $(OBJS) $(CFLAGS) $(LIB_DIR) $(LIBS) -o $(BIN_DIR)/$(TARGET)
	@echo "  Done building $(TARGET)"
	rm -f $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) $(INC_DIR) -c $< -o $@

%.o:%.cpp
	$(CXX) $(CFLAGS) $(INC_DIR) -c $< -o $@

future_test: $(TARGET)
	$(CXX) $(INC_DIR) $(LIB_DIR) -lfuture_devices -o $(BIN_DIR)/future_test test/future_test.cpp

clean:
	rm -f $(OBJS) $(BIN_DIR)/$(TARGET) $(BIN_DIR)/future_test

.PHONY: all clean
