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
7. Flight computer
	- Sends camera feed with different objects detected
	- User selects the object they want to track
	- The drone follows object, keeping its distance(estimate size of object)
8. Changing frequencies for controller and boosting signal strength.

Other
- PID tuning test stand
- Prop guards

## Compile and flash to raspberry pi pico
- 