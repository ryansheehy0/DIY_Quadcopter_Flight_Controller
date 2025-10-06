# DIY Quadcopter Flight Controller
Raspberry Pi Pico code for a DIY flight controller for a quadcopter.

<!-- Image and video of drone -->
<!-- Image of code diagram -->

## Future features
1. Basic FPV Drone
	1. Safety checks
		- LEDs on init and stick down.
		- Signal loss then hover.
	1. Stabilized mode
	1. FPV camera

2. Advanced FPV drone
	1. Upgrade to Pico 2
	1. Send back battery voltage on a different thread(INA219).
	1. Advanced stabilized algos
		- Acc Averaging -> Butterworth filter
		- Complimentary filter -> Madgwick filter
	1. Custom PCB
		- Mount in center with battery up top to allow for larger batteries.
		- Buy new frame.
	1. Better FPV camera
	1. Thermal camera with opacity filter from left pot.

3. Partially autonomous
	- Right pot selects the object to track with a rectangle around the object.
		- When set to 0 it doesn't show rectangles and doesn't track.
		- When you twist the right pot, it shows all the rectangles and highlights the one that's being currently tracked.
		- You have to wait a little before it starts tracking.

4. Fully autonomous
	- Give general location from UI
	- It knows its location from dead reckoning and from recognizing the terrain.
	- It searches for objects to track.
	- When it's low battery, it makes its way back home.

## Sources
- [Pico Documentation](https://www.raspberrypi.com/documentation/pico-sdk/hardware.html)
- [BMI160 Datasheet](https://www.bosch-sensortec.com/products/motion-sensors/imus/bmi160/)
- [How PWM works on the pico](https://forums.raspberrypi.com/viewtopic.php?t=309632)
- [Overview video](https://youtu.be/CHSYgLfhwUo?si=-crvyPy1awxu9ZyM)
- [Accelerometer video](https://youtu.be/7VW_XVbtu9k?si=ISFqyUkHYZV7qJ2G)
- [Simple flight controller video](https://youtu.be/4vpgjjYizVU?si=5Lh_3cyaq6MKLBy_)
- [How to compile for pico](https://github.com/tttapa/pico-cpp/tree/main)

## Compile and flash to raspberry pi pico
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

add_executable(hello
	hello.cpp
)
target_compile_features(hello PRIVATE cxx_std_20)
target_link_libraries(hello PRIVATE warnings pico_stdlib hardware_adc)
# Add other libraries: hardware_i2c

pico_enable_stdio_usb(hello 1)
pico_enable_stdio_uart(hello 0)
pico_add_extra_outputs(hello)
```

5. Run cmake commands

```sh
cmake -S . -B build
cmake --build build -j$(nproc)
```

6. Plug in Pico while pressing the program button. The copy and past the compiled UF2 file into the Pico's drive.

7. You can see console output with:

```sh
screen /dev/ttyACM0 115200
```