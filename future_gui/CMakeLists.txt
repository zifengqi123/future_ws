cmake_minimum_required(VERSION 3.10)
project(future_gui)

# Usage:
# cmake -B build -S .
# make  -C build -j

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(tools /usr/local/toolchain/linux64)
set(CMAKE_C_COMPILER ${tools}/bin/arm-openwrt-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/arm-openwrt-linux-gnueabi-g++)

# If necessary, set STAGING_DIR
# if not work, please try(in shell command): export STAGING_DIR=/home/ubuntu/Your_SDK/out/xxx/openwrt/staging_dir/target
#set(ENV{STAGING_DIR} "/home/ubuntu/Your_SDK/out/xxx/openwrt/staging_dir/target")

# Uncomment if the program needs debugging
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -ggdb")

set(CMAKE_C_STANDARD 99) # LVGL officially supports C99 and above
set(CMAKE_CXX_STANDARD 17) #C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)


include_directories(
    include
    ${PROJECT_SOURCE_DIR}/../lvgl_v9.2/lv_port_linux/
    )

add_library(future_gui SHARED 
    ${PROJECT_SOURCE_DIR}/src/status_ui.cpp
)


# Install the lvgl_linux library and its headers
install(DIRECTORY build/lib/
    DESTINATION include
    FILES_MATCHING
    PATTERN "*.h")

install(TARGETS future_gui
    ARCHIVE DESTINATION lib
)

