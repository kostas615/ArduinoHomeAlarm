# Arduino Home Alarm

This is a simple solution to home security, using arduino with ON/OFF switches.

The code is configured to work with 2 switches, and it has 2 states: ARMED and DISARMED.

During ARMED mode, the arduino reads the switches until it finds an open (or closed, depending on configuration) switch.
Then a buzzer goes off for 5 seconds, and finally it returns to normal ARMED mode again.

During DISARMED mode, the arduino reads the sensors and displays any opened (or closed...) switches.

# Configuration
All of this configuration can be setup in the first lines of the code.

# Wiring and Installation
Wiring and installation instructions will be included soon.
