# DIY Quadcopter Flight Controller
Raspberry Pi Pico code for a DIY flight controller for a quadcopter.

<!-- Image and video of drone -->
<!-- Image of code diagram -->

## Future features
1. Version 1: Basic Drone
	- Signal loss then hover
	- Stabilized mode
	- Basic FPV camera

2. Version 2: Advanced Drone
	- Custom PCB mounted on top with larger battery mounted in the center
		- Use off the shelf components
		- Use header pins. Female on PCB and male on components.
	- Advanced stabilized algorithms
		- Accelerometer Averaging -> Butterworth filter
		- Complimentary filter -> Madgwick filter
	- Show battery level and other info on fpv video signal

3. Version 3: Thermal drone
	- Switch between regular FPV camera and thermal one from left switch on controller.

4. Version 4: Partially autonomous drone
	- Right pot selects the object to track with a rectangle around the object.
		- When set to 0 it doesn't show rectangles and doesn't track.
		- When you twist the right pot, it shows all the rectangles and highlights the one that's being currently tracked.
		- You have to wait a little before it starts tracking.
	- Left pot selects at what distance to track the object.

- Possible features
	- Waterproof PCB
	- Custom device that quickly changes frequencies to get around jamming.
		- Uses mosfets on an antena to change frequency.
	- Drone swarm

## Capabilities
- Cameras: Visible light, IR, Thermal
- Drop a payload
- Speaker and microphone
- Powerful lights
- Lidar for terrain mapping
- Picking up things/grabbing things

## Sources
- [BMI160 Datasheet](https://www.bosch-sensortec.com/products/motion-sensors/imus/bmi160/)
- [Overview video](https://youtu.be/CHSYgLfhwUo?si=-crvyPy1awxu9ZyM)
- [Accelerometer video](https://youtu.be/7VW_XVbtu9k?si=ISFqyUkHYZV7qJ2G)
- [Simple flight controller video](https://youtu.be/4vpgjjYizVU?si=5Lh_3cyaq6MKLBy_)