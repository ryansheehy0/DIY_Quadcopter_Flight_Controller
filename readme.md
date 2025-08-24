# DIY Quadcopter Flight Controller

## Tests
- Test timer on pico
- Test gyro and acc data
- Test controller inputs
- Test motor output

## Future features
1. Safety checks
2. Median filter of acc. Linked list on stack that goes through and inserts and keeps going until you find the middle.
3. Disable motors from controller
4. Sound after init
5. Send back battery voltage/percentage with transmitter
6. FPV Camera
7. Combine cheap thermal camera with regular camera.
	- Pot to change opacity.
	- Use Open CV
8. Flight computer
	- Sends camera feed with different objects detected
	- User selects the object they want to track
	- The drone follows object, keeping its distance(estimate size of object)
9. Changing frequencies for controller and boosting signal strength.
	- Frequency hopping

- Left pot - Change opacity between regular and thermal camera.
	- Left pot could also change the threshold for temps that are gotten, only displaying hot spots.
- Right pot - Select the object you want to track.
	- It can also track using the heat source from the thermal camera.
	- Use switch to select the one you want to track.
		- Switch down means tracking and switch up means not tracking.

- Switches
	- Left
		- Arm/disarm
		- Altitude hold or not
	- Right
		- Don't track object, track object from far away, track object from close(crash into object).
		- Release/activate payload or not
			- Drop something
			- Produce sound
			- Produce lights, etc.

Other
- PID tuning test stand
- Prop guards
- Pico 2

## Compile and flash to raspberry pi pico
- Sources
	- https://github.com/tttapa/pico-cpp/tree/main
	- https://www.raspberrypi.com/documentation/pico-sdk/hardware.html

1. Install dev tools

```sh
sudo apt update
sudo apt install cmake build-essential git wget python3 gcc-arm-none-eabi libnewlib-arm-none-eabi
```

2. Install the Pico SDK

```sh
sdk_version=2.0.0
url=https://github.com/raspberrypi/pico-sdk
mkdir -p ~/pico
git clone $url --branch $sdk_version --recurse-submodules --shallow-submodules ~/pico/pico-sdk
```

3. Create terminal variable to the Pico SDK

```sh
export PICO_SDK_PATH="$HOME/pico/pico-sdk"
```

4. Add CMakeLists.txt file to your project folder. Change hello and hello.cpp to our main cpp file.

```
cmake_minimum_required(VERSION 3.20)

include($ENV{HOME}/pico/pico-sdk/external/pico_sdk_import.cmake)
project(pico-cpp LANGUAGES C CXX ASM)

pico_sdk_init()

add_library(warnings INTERFACE)
target_compile_options(warnings INTERFACE "-Wall" "-Wextra" "-Wno-psabi")

add_executable(hello "hello.cpp")
target_compile_features(hello PRIVATE cxx_std_20)
target_link_libraries(hello PRIVATE warnings pico_stdlib hardware_adc)
# Add other libraries: hardware_spi

pico_enable_stdio_usb(hello 1)
pico_enable_stdio_uart(hello 0)
pico_add_extra_outputs(hello)
```

5. Run cmake commands

```sh
cmake -S . -B build
cmake --build build -j$(nproc)
```

<!-- --toolchain ~/opt/x-tools/arm-pico-eabi/arm-pico-eabi.toolchain.cmake -->

6. Plug in Pico while pressing the program button. The copy and past the compiled UF2 file into the Pico's drive.

7. You can see console output with `screen /dev/ttyACM0 115200`