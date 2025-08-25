cmake_minimum_required(VERSION 3.20)

include($ENV{HOME}/pico/pico-sdk/external/pico_sdk_import.cmake)
project(pico-cpp LANGUAGES C CXX ASM)

pico_sdk_init()

add_library(warnings INTERFACE)
target_compile_options(warnings INTERFACE "-Wall" "-Wextra" "-Wno-psabi")

add_executable(main
	main.cpp
	imu.cpp
	moving_average.h
	imu.h
)
target_compile_features(main PRIVATE cxx_std_20)
target_link_libraries(main PRIVATE warnings pico_stdlib hardware_adc hardware_i2c)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)
pico_add_extra_outputs(main)